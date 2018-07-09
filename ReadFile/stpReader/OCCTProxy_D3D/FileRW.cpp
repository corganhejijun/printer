#include "BridgeFBO.hxx"
#include "slice.h"
#include "DxfReader.h"
#include "DxfWriter.h"

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
//step I/E
#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>
//for stl export
#include <StlAPI_Writer.hxx>
//wrapper of pure C++ classes to ref classes

#include <BRepAlgo_Section.hxx>
#include <gp_Pln.hxx>
#include <BRepBndLib.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <gp_Circ.hxx>
#include <Geom_BSplineCurve.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColgp_Array1OfPnt.hxx>
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
#include <TopTools_HSequenceOfShape.hxx>
#include <ShapeAnalysis_FreeBounds.hxx>
#include <GeomAPI_PointsToBSpline.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeSegment.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <ShapeFix_Shape.hxx>

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

#define EXPORT extern "C" __declspec( dllexport )
#define MAX_SLICE_DATA_LENGTH 256

typedef void(_stdcall *OnGetEdge)(void* shape, EdgeType type, double* data, int length);
typedef void*(_stdcall *GetNextEdge)(EdgeType* type, int index);
typedef void*(_stdcall *GetFaceHole)(double height, int index);

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

void showType(ShapeContainer* container, TopoDS_Shape shape, ofstream& file, OnGetEdge getEdge) {
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
                double circleData[MAX_SLICE_DATA_LENGTH] = {center.Z(), center.X(), center.Y(), first, last, circle.Radius(), beginX, beginY, endX, endY};
                getEdge(container, EdgeType::circle, circleData, 10);
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
                // 后面for循环是从1开始的，因此需要-2
                int length = 5 + polesCount * 2 - 2;
                double* bSplineData = new double[MAX_SLICE_DATA_LENGTH];
                bSplineData[0] = beginZ; bSplineData[1] = beginX; bSplineData[2] = beginY; bSplineData[3] = endX; bSplineData[4] = endY;
                // TODO:这里PolesCount过大的话要精简
                for (int i = 1; i < polesCount; i++) {
                    gp_Pnt pt = Poles(i);
                    bSplineData[4 + i * 2 - 1] = pt.X();
                    bSplineData[4 + i * 2] = pt.Y();
                    file << "(" << pt.X() << "," << pt.Y() << "," << pt.Z() << ") ";
                }
                file << endl;
                getEdge(container, EdgeType::bSplice, bSplineData, length);
                delete bSplineData;
            }
            else if (adpCurve.GetType() == GeomAbs_CurveType::GeomAbs_Line) {
                double beginX, beginY, beginZ, endX, endY, endZ;
                addVertex(child, beginX, beginY, beginZ, endX, endY, endZ);
                double lineData[MAX_SLICE_DATA_LENGTH] = {beginZ, beginX, beginY, endX, endY};
                getEdge(container, EdgeType::line, lineData, 5);
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
        showType(container, child, file, getEdge);
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

EXPORT void* ImportStep(char* theFileName)
{
    STEPControl_Reader aReader;
    if (aReader.ReadFile(theFileName) != IFSelect_RetDone)
        return false;

    bool isFailsonly = false;
    aReader.PrintCheckLoad(isFailsonly, IFSelect_ItemsByEntity);

    int aNbRoot = aReader.NbRootsForTransfer();
    ShapeContainer* singleShape = NULL;
    if (aNbRoot > 0) {
        aReader.PrintCheckTransfer(isFailsonly, IFSelect_ItemsByEntity);
        ofstream file("logReadStep.txt");
        // 可能有多个实体，只读取第一个
        aReader.TransferRoot(1);
        int aNbShap = aReader.NbShapes();
        if (aNbShap > 0)
        {
            TopoDS_Shape shape = aReader.Shape(aNbShap);
            singleShape = new ShapeContainer(shape);
        }
        file.close();
    }
    aReader.ClearShapes();
    return singleShape;
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
            showType(new ShapeContainer(shape), shape, file, getEdge);
            file << "end shape" << endl;
        }
    }
    aReader.ClearShapes();
    file.close();
    return true;
}

EXPORT bool onGetSlice(ShapeContainer* shape, OnGetEdge getEdge) {
    ofstream file("onGetSlice.txt");
    showType(shape, shape->getShape(), file, getEdge);
    file.close();
    return true;
}

EXPORT bool ImportDxfSlice(wchar_t* fileName, OnGetEdge getEdge) {
    DxfReader dxfReader(fileName);
    TopTools_HSequenceOfShape shapeList = dxfReader.GetShape();
    ofstream file("logReadDxfSlice.txt");
    for (int i = 1; i <= shapeList.Length(); i++) {
        file << "begin shape" << endl;
        TopoDS_Shape shape = shapeList.Value(i);
        showType(new ShapeContainer(shape), shape, file, getEdge);
        file << "end shape" << endl;
    }
    file.close();
    return true;
}

EXPORT void* ImportDxf(wchar_t* fileName) {
    DxfReader dxfReader(fileName);
    TopTools_HSequenceOfShape shapeList = dxfReader.GetShape();
    TopoDS_Shape shape = shapeList.Value(1);
    for (int i = 2; i <= shapeList.Length(); i++) {
        TopoDS_Shape shape2 = shapeList.Value(i);
        shape = BRepAlgoAPI_Fuse(shape, shape2);
    }
    return new ShapeContainer(shape);
}

EXPORT void del(void* pt)
{
    ShapeContainer* container = (ShapeContainer*)pt;
    delete container;
}

EXPORT bool getBoundary(ShapeContainer* shape, double* Zmin, double* Zmax, double* Ymin, double* Ymax, double* Xmin, double* Xmax)
{
    if (shape == NULL)
        return false;
    Bnd_Box B;
    TopoDS_Shape topoShape = shape->getShape();
    if (topoShape.IsNull())
        return false;
    BRepBndLib::Add(shape->getShape(), B);
    B.Get(*Xmin, *Ymin, *Zmin, *Xmax, *Ymax, *Zmax);
    return true;
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

EXPORT void* getLocatPlane(ShapeContainer* shape) {
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
            return new ShapeContainer(currentFace);
        }
    }
    return NULL;
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

EXPORT bool exportStl(char* fileName, ShapeContainer* shapeList) {
    StlAPI_Writer STLwriter;
    TopoDS_Shape shape = shapeList->getShape();
    STLwriter.Write(shape, fileName);
    return true;
}

EXPORT void* exportDxfBegin(wchar_t* fileName) {
    DxfWriter* writer = new DxfWriter(fileName);
    writer->writeHead();
    return writer;
}

EXPORT void exportDxfEnd(void* writerPt) {
    DxfWriter* writer = (DxfWriter*)writerPt;
    writer->writeEnd();
}

EXPORT void writeDxfLine(void* writerPt, double beginX, double beginY, double endX, double endY, double z) {
    DxfWriter* writer = (DxfWriter*)writerPt;
    writer->writeLine(beginX, beginY, z, endX, endY, z);
}

EXPORT void writeDxfCircle(void* writerPt, double x, double y, double z, double r, int normal) {
    DxfWriter* writer = (DxfWriter*)writerPt;
    writer->writeCircle(x, y, z, r, normal);
}

EXPORT void writerDxfArc(void* writerPt, double x, double y, double z, double r, double startAngle, double endAngle, int normal) {
    DxfWriter* writer = (DxfWriter*)writerPt;
    writer->writeArc(x, y, z, r, startAngle, endAngle, normal);
}

TopoDS_Edge getBspline(void* data, double height) {
    BSpline* bs = (BSpline*)data;
    TColgp_Array1OfPnt array1(1, bs->polesCnt);
    for (int i = 0; i < bs->polesCnt; i++) {
        gp_Pnt P(bs->poles[i].x, bs->poles[i].y, height);
        array1.SetValue(i, P);
    }
    Handle(Geom_BSplineCurve) bsc1 = GeomAPI_PointsToBSpline(array1).Curve();
    TopoDS_Edge E1 = BRepBuilderAPI_MakeEdge(bsc1);
    return E1;
}

TopoDS_Edge getCircle(void* data, double height) {
    Circle* c = (Circle*)data;
    if (abs(c->endAngle - c->startAngle - M_PI * 2) < 0.0001) {
        gp_Circ circle;
        circle.SetLocation(gp_Pnt(c->center.x, c->center.y, height));
        circle.SetRadius(c->radius);
        TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(circle);
        return edge;
    }
    double middleAngle = (c->startAngle + c->endAngle) / 2;
    gp_Pnt middle(c->radius*cos(middleAngle) + c->center.x, c->radius*sin(middleAngle) + c->center.y, height);
    Handle(Geom_TrimmedCurve) aArcOfCircle = GC_MakeArcOfCircle(gp_Pnt(c->start.x, c->start.y, height), middle, gp_Pnt(c->end.x, c->end.y, height));
    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(aArcOfCircle);
    return edge;
}

TopoDS_Edge getLine(void* data, double height) {
    Line* l = (Line*)data;
    gp_Pnt pt1(l->start.x, l->start.y, height);
    gp_Pnt pt2(l->end.x, l->end.y, height);
    Handle(Geom_TrimmedCurve) segment = GC_MakeSegment(pt1, pt2);
    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(segment);
    return edge;
}

EXPORT void* slice2Wires(double height, GetNextEdge onGetEdge, int edgeCount) {
    Handle(TopTools_HSequenceOfShape) edges = new TopTools_HSequenceOfShape();
    EdgeType* type = new EdgeType();
    for (int i = 0; i < edgeCount; i++){
        void* edge = onGetEdge(type, i);
        switch (*type) {
        case EdgeType::bSplice:
            edges->Append(getBspline(edge, height));
            break;
        case EdgeType::circle:
            edges->Append(getCircle(edge, height));
            break;
        case EdgeType::line:
            edges->Append(getLine(edge, height));
            break;
        default:continue;
        }
    }
    SequenceContainer* sqContainer = new SequenceContainer(edges);
    return sqContainer;
}

EXPORT void* makeFaceFromWire(void* wirePt, double height, int childCnt, GetFaceHole getHole) {
    if (wirePt == NULL)
        return NULL;
    SequenceContainer* seqContainer = (SequenceContainer*)wirePt;
    Handle(TopTools_HSequenceOfShape) edges= seqContainer->sequence;
    if (edges->Length() < 1)
        return NULL;
    BRepBuilderAPI_MakeWire mkWire;
    for (int i = 1; i <= edges->Length(); i++) {
        mkWire.Add(TopoDS::Edge(edges->Value(i)));
    }
    TopoDS_Wire myWireProfile = mkWire.Wire();
    TopoDS_Face face = BRepBuilderAPI_MakeFace(myWireProfile);
    ShapeContainer* sc = NULL;
    if (childCnt > 0)
    {
        BRepBuilderAPI_MakeFace	MF(face);
        for (int i = 0; i < childCnt; i++) {
            SequenceContainer* sq = (SequenceContainer*)getHole(height, i);
            Handle(TopTools_HSequenceOfShape) hs = sq->sequence;
            BRepBuilderAPI_MakeWire mk;
            for (int i = 1; i <= hs->Length(); i++) {
                mk.Add(TopoDS::Edge(hs->Value(i)));
            }
            TopoDS_Wire hole = mk.Wire();
            MF.Add(hole);
            delete sq;
        }
        if (!MF.IsDone()) {
            return NULL;
        }
        Handle(ShapeFix_Shape) sfs = new ShapeFix_Shape();
        sfs->Init(MF.Shape());
        sfs->Perform();
        sc = new ShapeContainer(sfs->Shape());
    }
    else
    {
        Handle(ShapeFix_Shape) sfs = new ShapeFix_Shape();
        sfs->Init(face);
        sfs->Perform();
        sc = new ShapeContainer(sfs->Shape());
    }
    delete seqContainer;
    return sc;
}