#include "BridgeFBO.hxx"

// include required OCCT headers
#include <Standard_Version.hxx>
#include <Message_ProgressIndicator.hxx>
//for OCC graphic
#include <Aspect_DisplayConnection.hxx>
#include <WNT_Window.hxx>
#include <Graphic3d_CView.hxx>
#include <Graphic3d_Camera.hxx>
#include <Graphic3d_TextureParams.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <OpenGl_CView.hxx>
//for object display
#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>
#include <Visual3d_View.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
//topology
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
//brep tools
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <MgtBRep.hxx>
#include <PTColStd_PersistentTransientMap.hxx>
//csfdb I/E
#include <FSD_File.hxx>
#include <ShapeSchema.hxx>
#include <Storage_Data.hxx>
#include <Storage_HSeqOfRoot.hxx>
#include <Storage_Root.hxx>
// iges I/E
#include <IGESControl_Reader.hxx>
#include <IGESControl_Controller.hxx>
#include <IGESControl_Writer.hxx>
#include <IFSelect_ReturnStatus.hxx>
#include <Interface_Static.hxx>
//step I/E
#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>
//for stl export
#include <StlAPI_Writer.hxx>
//for vrml export
#include <VrmlAPI_Writer.hxx>
//wrapper of pure C++ classes to ref classes
#include <NCollection_Haft.h>

#include <BRepAlgo_Section.hxx>
#include <gp_Pln.hxx>
#include <BRepBndLib.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <sstream>
#include <TopTools_HSequenceOfShape.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <gp_Circ.hxx>
#include <Geom_BSplineCurve.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <ShapeAnalysis_FreeBounds.hxx>
#include <BRep_Builder.hxx>
#include <gp_Trsf.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <RWStl.hxx>
#include <StlMesh_Mesh.hxx>
#include <StlMesh_MeshExplorer.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

#include <iostream>
#include <fstream>
#include <math.h>
#include "slice.h"

#define EXPORT extern "C" __declspec( dllexport )

typedef void(_stdcall *OnGetShape)(void* shape);
typedef void(_stdcall *OnGetEdge)(EdgeType type, double* data, int length);

// ============================================
// Import / export functionality
// ============================================
/// <summary>
///Import Step file
/// </summary>
/// <param name="theFileName">Name of import file</param>
int layer = 1;
const char* TopAbs_ShapeEnum_str[] = { "COMPOUND", "COMPSOLID", "SOLID", "SHELL", "FACE", "WIRE", "EDGE", "VERTEX", "SHAPE" };
const char* GeomAbs_CurveType_str[] = { "Line", "Circle", "Ellipse", "Hyperbola", "Parabola", "BezierCurve", "BSplineCurve", "OtherCurve" };

void addVertex(TopoDS_Shape shape, double& beginX, double& beginY, double& beginZ, double& endX, double& endY, double& endZ) {
    int i = 0;
    for (TopoDS_Iterator anIt(shape); anIt.More(); anIt.Next()) {
        i++;
        TopoDS_Shape child = anIt.Value();
        if (child.ShapeType() == TopAbs_ShapeEnum::TopAbs_VERTEX) {
            TopoDS_Vertex vertex = TopoDS::Vertex(child);
            gp_Pnt pt = BRep_Tool::Pnt(vertex);
            switch (i)
            {
            case 1:
                beginX = pt.X();
                beginY = pt.Y();
                beginZ = pt.Z();
                break;
            case 2:
                endX = pt.X();
                endY = pt.Y();
                endZ = pt.Z();
                break;
            default:
                printf("error on add line with %d vertex\n", i);
                break;
            }
        }
    }
}

void showType(TopoDS_Shape shape, ofstream& file, OnGetEdge getEdge) {
    layer++;
    for (TopoDS_Iterator anIt(shape); anIt.More(); anIt.Next()) {
        TopoDS_Shape child = anIt.Value();
        if (child.ShapeType() == TopAbs_ShapeEnum::TopAbs_EDGE) {
            BRepAdaptor_Curve adpCurve = BRepAdaptor_Curve(TopoDS::Edge(child));
            if (adpCurve.GetType() == GeomAbs_CurveType::GeomAbs_Circle) {
                gp_Circ circle = adpCurve.Circle();
                gp_Pnt center = circle.Location();
                Standard_Real first = 0, last = 0;
                Handle_Geom_Curve theCurve = BRep_Tool::Curve(TopoDS::Edge(child), first, last);
                for (int i = 0; i < layer; i++)
                    file << '\t';
                file << "circle radius is " << circle.Radius() << ", center is (" << center.X() << "," << center.Y() << "," << center.Z() << "), start angle is " << (first/M_PI*180) << ", end angle is " << (last/M_PI*180) << endl;
                double beginX, beginY, beginZ, endX, endY, endZ;
                addVertex(child, beginX, beginY, beginZ, endX, endY, endZ);
                double circleData[10] = {center.Z(), center.X(), center.Y(), first, last, circle.Radius(), beginX, beginY, endX, endY};
                getEdge(EdgeType::circle, circleData, 10);
            } else if (adpCurve.GetType() == GeomAbs_CurveType::GeomAbs_BSplineCurve) {
                Handle_Geom_BSplineCurve bSpline = adpCurve.BSpline();
                for (int i = 0; i < layer; i++)
                    file << '\t';
                file << "B spline ";
                if (bSpline->IsPeriodic())
                    file << "is periodic";
                if (bSpline->IsRational())
                    file << " weights are same";
                int knotsCount = bSpline->NbKnots();
                int polesCount = bSpline->NbPoles();
                file << " degree is " << bSpline->Degree() << " has " << knotsCount << " knots " << polesCount << " poles " << endl;
                TColStd_Array1OfReal knots(1, knotsCount);
                bSpline->Knots(knots);
                file << " knots value ";
                for (int i = 1; i < knotsCount; i++)
                    file << knots(i) << " ";
                file << endl;
                TColgp_Array1OfPnt Poles(1, polesCount);
                bSpline->Poles(Poles);
                file << " poles value";

                double beginX, beginY, beginZ, endX, endY, endZ;
                addVertex(child, beginX, beginY, beginZ, endX, endY, endZ);
                int length = 5 + polesCount * 2;
                double* bSplineData = new double[length];
                bSplineData[0] = beginZ; bSplineData[1] = beginX; bSplineData[2] = beginY; bSplineData[3] = endX; bSplineData[4] = endY;
                for (int i = 1; i < polesCount; i++) {
                    gp_Pnt pt = Poles(i);
                    bSplineData[4 + i * 2 - 1] = pt.X();
                    bSplineData[4 + i * 2] = pt.Y();
                    file << "(" << pt.X() << "," << pt.Y() << "," << pt.Z() << ") ";
                }
                file << endl;
                getEdge(EdgeType::bSplice, bSplineData, length);
                delete bSplineData;
            }
            else if (adpCurve.GetType() == GeomAbs_CurveType::GeomAbs_Line) {
                double beginX, beginY, beginZ, endX, endY, endZ;
                addVertex(child, beginX, beginY, beginZ, endX, endY, endZ);
                double lineData[5] = {beginZ, beginX, beginY, endX, endY};
                getEdge(EdgeType::line, lineData, 5);
            }
            else {
                for (int i = 0; i < layer; i++)
                    file << '\t';
                file << "edge type is " << GeomAbs_CurveType_str[adpCurve.GetType()] << endl;
            }
        } else if (child.ShapeType() == TopAbs_ShapeEnum::TopAbs_VERTEX) {
            TopoDS_Vertex vertex = TopoDS::Vertex(child);
            gp_Pnt pt = BRep_Tool::Pnt(vertex);
            for (int i = 0; i < layer; i++)
                file << '\t';
            file << "vertex postion is (" << pt.X() << "," << pt.Y() << "," << pt.Z() << ")" << endl;
        } else {
            for (int i = 0; i < layer; i++)
                file << '\t';
            file << "type is " << TopAbs_ShapeEnum_str[child.ShapeType()] << endl;
        }
        showType(child, file, getEdge);
    }
    --layer;
}

EXPORT void* ImportStl(char* theFileName) {
    TopoDS_Compound ResultShape;
    BRep_Builder CompoundBuilder;
    CompoundBuilder.MakeCompound(ResultShape);

    OSD_Path aFile(theFileName);
    Handle(StlMesh_Mesh) aSTLMesh = RWStl::ReadFile(aFile);
    Standard_Integer NumberDomains = aSTLMesh->NbDomains();
    StlMesh_MeshExplorer aMExp(aSTLMesh);
    for (Standard_Integer iND = 1; iND <= NumberDomains; iND++) {
        for (aMExp.InitTriangle(iND); aMExp.MoreTriangle(); aMExp.NextTriangle())
        {
            Standard_Real x1, y1, z1, x2, y2, z2, x3, y3, z3;
            aMExp.TriangleVertices(x1,y1,z1,x2,y2,z2,x3,y3,z3);
            gp_XYZ p1, p2, p3;
            p1.SetCoord(x1,y1,z1);
            p2.SetCoord(x2,y2,z2);
            p3.SetCoord(x3,y3,z3);
            if ((!(p1.IsEqual(p2,0.0))) && (!(p1.IsEqual(p3,0.0)))) {
                TopoDS_Vertex Vertex1 = BRepBuilderAPI_MakeVertex(p1);
                TopoDS_Vertex Vertex2 = BRepBuilderAPI_MakeVertex(p2);
                TopoDS_Vertex Vertex3 = BRepBuilderAPI_MakeVertex(p3);
                TopoDS_Wire AktWire = BRepBuilderAPI_MakePolygon(Vertex1, Vertex2, Vertex3, Standard_True);
                if (!AktWire.IsNull()) {
                    TopoDS_Face AktFace = BRepBuilderAPI_MakeFace(AktWire);
                    if (!AktFace.IsNull())
                        CompoundBuilder.Add(ResultShape, AktFace);
                }
            }
        }
    }
    if (ResultShape.IsNull())
        return NULL;
    ShapeContainer* sc = new ShapeContainer(ResultShape);
    return sc;
}

EXPORT bool ImportStep(char* theFileName, OnGetShape getShape)
{
    STEPControl_Reader aReader;
    if (aReader.ReadFile(theFileName) != IFSelect_RetDone)
        return false;

    bool isFailsonly = false;
    aReader.PrintCheckLoad(isFailsonly, IFSelect_ItemsByEntity);

    int aNbRoot = aReader.NbRootsForTransfer();
    aReader.PrintCheckTransfer(isFailsonly, IFSelect_ItemsByEntity);
    ofstream file("logReadStep.txt");
    for (Standard_Integer aRootIter = 1; aRootIter <= aNbRoot; ++aRootIter)
    {
        aReader.TransferRoot(aRootIter);
        int aNbShap = aReader.NbShapes();
        if (aNbShap > 0)
        {
            TopoDS_Shape shape = aReader.Shape(aNbShap);
            ShapeContainer* singleShape = new ShapeContainer(shape);
            getShape(singleShape);
        }
    }
    aReader.ClearShapes();
    file.close();
    return true;
}

EXPORT bool ImportSlice(char* fileName, OnGetEdge getEdge) {
    STEPControl_Reader aReader;
    if (aReader.ReadFile(fileName) != IFSelect_RetDone)
        return false;

    bool isFailsonly = false;
    aReader.PrintCheckLoad(isFailsonly, IFSelect_ItemsByEntity);

    int aNbRoot = aReader.NbRootsForTransfer();
    aReader.PrintCheckTransfer(isFailsonly, IFSelect_ItemsByEntity);
    ofstream file("logReadSlice.txt");
    for (Standard_Integer aRootIter = 1; aRootIter <= aNbRoot; ++aRootIter)
    {
        aReader.TransferRoot(aRootIter);
        int aNbShap = aReader.NbShapes();
        if (aNbShap > 0)
        {
            TopoDS_Shape shape = aReader.Shape(aNbShap);
            file << "begin shape" << endl;
            showType(shape, file, getEdge);
            file << "end shape" << endl;
        }
    }
    aReader.ClearShapes();
    file.close();
    return true;
}

EXPORT void del(void* pt)
{
    ShapeContainer* container = (ShapeContainer*)pt;
    delete container;
}

EXPORT void getBoundary(ShapeContainer* shape, double* Zmin, double* Zmax, double* Ymin, double* Ymax, double* Xmin, double* Xmax)
{
    Bnd_Box B;
    BRepBndLib::Add(shape->getShape(), B);
    B.Get(*Xmin, *Ymin, *Zmin, *Xmax, *Ymax, *Zmax);
}

EXPORT void* rotate(ShapeContainer* shape, double x, double y, double z) {
    gp_Trsf xTrsf, yTrsf, zTrsf;
    xTrsf.SetRotation(gp::OX(), x * M_PI / 180);
    yTrsf.SetRotation(gp::OY(), y * M_PI / 180);
    zTrsf.SetRotation(gp::OZ(), z * M_PI / 180);
    BRepBuilderAPI_Transform xform1(shape->getShape(), xTrsf*yTrsf*zTrsf);
    TopoDS_Shape transShape = xform1.Shape();
    return new ShapeContainer(transShape);
}

EXPORT void* move(ShapeContainer* shape, double x, double y, double z) {
    gp_Trsf trsf;
    trsf.SetTranslation(gp_Vec(x, y, z));
    BRepBuilderAPI_Transform xform(shape->getShape(), trsf);
    TopoDS_Shape transShape = xform.Shape();
    return new ShapeContainer(transShape);
}

EXPORT ShapeContainer* scale(ShapeContainer* shape, double ratio, double centerX, double centerY, double centerZ) {
    TopoDS_Shape slice = shape->getShape();
    gp_Trsf xTrsf;
    gp_Pnt scaleCenter(centerX, centerY, centerZ);
    xTrsf.SetScale(scaleCenter, ratio);
    BRepBuilderAPI_Transform myTrans(slice, xTrsf, true);
    TopoDS_Shape newSlice = myTrans.Shape();
    return new ShapeContainer(newSlice);
}

Handle_TopTools_HSequenceOfShape getWires(TopoDS_Shape shape) {
    Handle(TopTools_HSequenceOfShape) Edges = new TopTools_HSequenceOfShape();
    for (TopoDS_Iterator anIt(shape); anIt.More(); anIt.Next()) {
        for (TopExp_Explorer edgeExp(anIt.Value(), TopAbs_EDGE); edgeExp.More(); edgeExp.Next()) {
            TopoDS_Edge edge = TopoDS::Edge(edgeExp.Current());
            Edges->Append(edge);
        }
    }
    Handle(TopTools_HSequenceOfShape) Wires = new TopTools_HSequenceOfShape();
    ShapeAnalysis_FreeBounds::ConnectEdgesToWires(Edges, Precision::Confusion(), Standard_False, Wires);
    return Wires;
}

EXPORT void* slice(ShapeContainer* shape, double height)
{
    // 生成水平面，进行逐层切割
    TopoDS_Shape face = shape->getShape();
    double Xmin, Ymin, Zmin, Xmax, Ymax, Zmax;
    Bnd_Box C;
    BRepBndLib::Add(face, C);
    C.Get(Xmin, Ymin, Zmin, Xmax, Ymax, Zmax);
    if (Zmax - Zmin < 0.0001) {
        // 水平面，不存在交点
        return NULL;
    }
    gp_Pln originPlane = gp_Pln(0, 0, 1, -height);
    BRepAlgo_Section section(shape->getShape(), originPlane, Standard_True);
    TopoDS_Shape sectionShape = section.Shape();
    if (sectionShape.IsNull())
        return NULL;
    return new ShapeContainer(sectionShape);
}

EXPORT void* combine(ShapeContainer* shape1, ShapeContainer* shape2) {
    TopoDS_Shape shape = BRepAlgoAPI_Fuse(shape1->getShape(), shape2->getShape());
    return new ShapeContainer(shape);
}

EXPORT void getLocatPlane(ShapeContainer* shape, OnGetShape getShape) {
    // 获取与XY平面平行的定位面
    TopExp_Explorer explorer;
    double Xmin, Ymin, Zmin, Xmax, Ymax, Zmax;
    int counter = 0;
    for (explorer.Init(shape->getShape(), TopAbs_FACE); explorer.More(); explorer.Next()) {
        TopoDS_Shape currentFace = explorer.Current();
        Bnd_Box C;
        BRepBndLib::Add(currentFace, C);
        C.Get(Xmin, Ymin, Zmin, Xmax, Ymax, Zmax);
        if (Zmax - Zmin < 0.0001) {
            getShape(new ShapeContainer(currentFace));
        }
    }
}

EXPORT bool exportStep(char* fileName, ShapeContainer** shapeList, int length) {
    STEPControl_Writer aWriter;
    for (int i = 0; i < length; i++) {
        TopoDS_Compound aCompound;
        BRep_Builder aBuilder;
        aBuilder.MakeCompound(aCompound);
        aBuilder.Add(aCompound, shapeList[i]->getShape());
        if (aWriter.Transfer(aCompound, STEPControl_AsIs) != IFSelect_RetDone)
            return false;
    }
    return aWriter.Write(fileName) == IFSelect_RetDone;
}