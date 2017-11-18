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

Slice* getEmptySlice(Slice* slice) {
    Slice* target = slice;
    Slice* prev = NULL;
    while (target != NULL && target->type != EdgeType::unknown) {
        prev = target;
        target = target->next;
    }
    if (target == NULL){
        prev->next = new Slice();
        target = prev->next;
        target->next = NULL;
        target->prev = prev;
    }
    return target;
}

Slice* addCircle(Slice* slice, double centerX, double centerY, double startAngle, double endAngle, double radius) {
    Slice* target = getEmptySlice(slice);
    target->type = EdgeType::circle;
    target->data = new Circle();
    Circle* circle = (Circle*)(target->data);
    circle->center.x = centerX;
    circle->center.y = centerY;
    circle->startAngle = startAngle;
    circle->endAngle = endAngle;
    circle->radius = radius;
    return target;
}

void addCircleVertex(Slice* slice, double beginX, double beginY, double endX, double endY) {
    Circle* circle = (Circle*)(slice->data);
    circle->start.x = beginX;
    circle->start.y = beginY;
    circle->end.x = endX;
    circle->end.y = endY;
}

Slice* addLine(Slice* slice, double beginX, double beginY, double endX, double endY) {
    Slice* target = getEmptySlice(slice);
    target->type = EdgeType::line;
    target->data = new Line();
    Line* line = (Line*)(target->data);
    line->start.x = beginX;
    line->start.y = beginY;
    line->end.x = endX;
    line->end.y = endY;
    return target;
}

void addBSpline(Slice* slice, double beginX, double beginY, double endX, double endY, Point* bSplinePoles, int polesCnt) {
    Slice* target = getEmptySlice(slice);
    target->type = EdgeType::bSplice;
    target->data = new BSpline();
    BSpline* b = (BSpline*)(target->data);
    b->start.x = beginX;
    b->start.y = beginY;
    b->end.x = endX;
    b->end.y = endY;
    bSplinePoles[polesCnt - 1].x = endX;
    bSplinePoles[polesCnt - 1].y = endY;
    b->poles = bSplinePoles;
    b->polesCnt = polesCnt;
}

void addVertex(TopoDS_Shape shape, Slice* slice, Point* bSplinePoles, int polesCnt) {
    int i = 0;
    double beginX, beginY, endX, endY;
    double z;
    for (TopoDS_Iterator anIt(shape); anIt.More(); anIt.Next()) {
        i++;
        TopoDS_Shape child = anIt.Value();
        if (child.ShapeType() == TopAbs_ShapeEnum::TopAbs_VERTEX){
            TopoDS_Vertex vertex = TopoDS::Vertex(child);
            gp_Pnt pt = BRep_Tool::Pnt(vertex);
            switch (i)
            {
            case 1:
                beginX = pt.X();
                beginY = pt.Y();
                z = pt.Z();
                break;
            case 2:
                endX = pt.X();
                endY = pt.Y();
                break;
            default:
                printf("error on add line with %d vertex\n", i);
                break;
            }
        }
    }
    if (bSplinePoles != NULL) {
        addBSpline(slice, beginX, beginY, endX, endY, bSplinePoles, polesCnt);
    }
    else {
        if (polesCnt == 0) {
            Slice* target = addLine(slice, beginX, beginY, endX, endY);
            target->z = z;
        }
        if (polesCnt == 1)
            addCircleVertex(slice, beginX, beginY, endX, endY);
    }
}

void showType(TopoDS_Shape shape, ofstream& file, Slice* slice) {
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
                Slice* target = addCircle(slice, center.X(), center.Y(), first, last, circle.Radius());
                for (int i = 0; i < layer; i++)
                    file << '\t';
                file << "circle radius is " << circle.Radius() << 
                    ", center is (" << center.X() << "," << center.Y() << "," << center.Z() << ")" 
                    ", start angle is " << (first/M_PI*180) << ", end angle is " << (last/M_PI*180) << endl;
                addVertex(child, target, NULL, 1);
                target->z = center.Z();
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
                Point* pointArray = new Point[polesCount];
                for (int i = 1; i < polesCount; i++) {
                    gp_Pnt pt = Poles(i);
                    pointArray[i - 1].x = pt.X();
                    pointArray[i - 1].y = pt.Y();
                    file << "(" << pt.X() << "," << pt.Y() << "," << pt.Z() << ") ";
                }
                file << endl;
                addVertex(child, slice, pointArray, polesCount);
                slice->z = Poles(1).Z();
            }
            else if (adpCurve.GetType() == GeomAbs_CurveType::GeomAbs_Line) {
                addVertex(child, slice, NULL, 0);
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
        showType(child, file, slice);
    }
    --layer;
}

EXPORT bool ImportStl(char* theFileName, void** shapes) {
    OSD_Path aFile(theFileName);
    Standard_Boolean ReturnValue = Standard_True;

    Handle(StlMesh_Mesh) aSTLMesh = RWStl::ReadFile(aFile);

    TopoDS_Compound ResultShape;
    BRep_Builder CompoundBuilder;
    CompoundBuilder.MakeCompound(ResultShape);

    Standard_Integer NumberDomains = aSTLMesh->NbDomains();
    Standard_Integer iND;
    gp_XYZ p1, p2, p3;
    TopoDS_Vertex Vertex1, Vertex2, Vertex3;
    TopoDS_Face AktFace;
    TopoDS_Wire AktWire;
    BRep_Builder B;
    Standard_Real x1, y1, z1;
    Standard_Real x2, y2, z2;
    Standard_Real x3, y3, z3;

    StlMesh_MeshExplorer aMExp(aSTLMesh);

    //	BRepBuilderAPI_MakeWire WireCollector;
    for (iND = 1; iND <= NumberDomains; iND++) {
        for (aMExp.InitTriangle(iND); aMExp.MoreTriangle(); aMExp.NextTriangle())
        {
            aMExp.TriangleVertices(x1,y1,z1,x2,y2,z2,x3,y3,z3);
                p1.SetCoord(x1,y1,z1);
                p2.SetCoord(x2,y2,z2);
                p3.SetCoord(x3,y3,z3);

                if ((!(p1.IsEqual(p2,0.0))) && (!(p1.IsEqual(p3,0.0))))
                {
                    Vertex1 = BRepBuilderAPI_MakeVertex(p1);
                        Vertex2 = BRepBuilderAPI_MakeVertex(p2);
                    Vertex3 = BRepBuilderAPI_MakeVertex(p3);

                    AktWire = BRepBuilderAPI_MakePolygon(Vertex1, Vertex2, Vertex3, Standard_True);

                    if (!AktWire.IsNull())
                    {
                        AktFace = BRepBuilderAPI_MakeFace(AktWire);
                        if (!AktFace.IsNull())
                            CompoundBuilder.Add(ResultShape,AktFace);
                    }
                }
        }
    }
    TopoDS_Shape aShape = ResultShape;
    Handle(TopTools_HSequenceOfShape) aHSequenceOfShape = new TopTools_HSequenceOfShape;
    aHSequenceOfShape->Append(aShape);
    double Xmin, Ymin, Zmin, Xmax, Ymax, Zmax;
    Bnd_Box Box;
    BRepBndLib::Add(aShape, Box);
    Box.Get(Xmin, Ymin, Zmin, Xmax, Ymax, Zmax);
    ShapeContainer* sc = new ShapeContainer(aHSequenceOfShape, Zmin);
    *shapes = sc;
    return true;
}

EXPORT bool ImportStep(char* theFileName, int* cnt, void** shapes, bool isSlice, void** slice)
{
    *cnt = 0;
    STEPControl_Reader aReader;
    if (aReader.ReadFile(theFileName) != IFSelect_RetDone)
    {
        return false;
    }

    bool isFailsonly = false;
    aReader.PrintCheckLoad(isFailsonly, IFSelect_ItemsByEntity);

    int aNbRoot = aReader.NbRootsForTransfer();
    aReader.PrintCheckTransfer(isFailsonly, IFSelect_ItemsByEntity);
    ofstream file("log.txt");
    Slice** ss = (Slice**)slice;
    *ss = new Slice[aNbRoot];
    double Xmin, Ymin, Zmin, Xmax, Ymax, Zmax;
    for (Standard_Integer aRootIter = 1; aRootIter <= aNbRoot; ++aRootIter)
    {
        aReader.TransferRoot(aRootIter);
        int aNbShap = aReader.NbShapes();
        if (aNbShap > 0)
        {
            Handle(TopTools_HSequenceOfShape) aHSequenceOfShape = new TopTools_HSequenceOfShape;
            TopoDS_Shape shape = aReader.Shape(aNbShap);
            if (isSlice) {
                file << "begin shape" << endl;
                Slice* sss = &(*ss)[(*cnt)];
                sss->data = sss->next = sss->prev = NULL;
                sss->type = EdgeType::unknown;
                Bnd_Box B;
                BRepBndLib::Add(shape, B);
                B.Get(Xmin, Ymin, Zmin, Xmax, Ymax, Zmax);
                showType(shape, file, sss);
                file << "end shape" << endl;
            }
            else
                Zmin = 0;
            aHSequenceOfShape->Append(shape);
            ShapeContainer* sc = new ShapeContainer(aHSequenceOfShape, Zmin);
            *(shapes + (*cnt)*sizeof(void*)) = sc;
            (*cnt)++;
        }
    }
    aReader.ClearShapes();
    file.close();
    return true;
}

EXPORT void* getSliceFromShape(void** shapes, int index, double* height) {
    ShapeContainer* container = *((ShapeContainer**)shapes + index*sizeof(void*));
    *height = container->height;
    return container;
}

EXPORT bool rotateShape(void** pt, void** rotateResult, int count, double x, double y, double z) {
    gp_Trsf xTrsf;
    xTrsf.SetRotation(gp::OX(), x * M_PI / 180);
    gp_Trsf yTrsf;
    yTrsf.SetRotation(gp::OY(), y * M_PI / 180);
    gp_Trsf zTrsf;
    zTrsf.SetRotation(gp::OZ(), z * M_PI / 180);
    double Xmin, Ymin, Zmin, Xmax, Ymax, Zmax;
    for (int i = 0; i < count; i++) {
        ShapeContainer* shape = (ShapeContainer*)(*(pt + i * sizeof(void*)));
        BRepBuilderAPI_Transform xform1(shape->getShape(1), xTrsf*yTrsf*zTrsf);
        TopoDS_Shape transShape = xform1.Shape();
        Handle(TopTools_HSequenceOfShape) aHSequenceOfShape = new TopTools_HSequenceOfShape;
        aHSequenceOfShape->Append(transShape);
        Bnd_Box B;
        BRepBndLib::Add(transShape, B);
        B.Get(Xmin, Ymin, Zmin, Xmax, Ymax, Zmax);
        ShapeContainer* transContainer = new ShapeContainer(aHSequenceOfShape, Zmin);
        *(rotateResult + i * sizeof(void*)) = transContainer;
    }
    return true;
}

EXPORT bool moveShape(void** pt, void** moveResult, int count, double x, double y, double z) {
    gp_Trsf trsf;
    trsf.SetTranslation(gp_Vec(x, y, z));
    double Xmin, Ymin, Zmin, Xmax, Ymax, Zmax;
    for (int i = 0; i < count; i++) {
        ShapeContainer* shape = (ShapeContainer*)(*(pt + i * sizeof(void*)));
        BRepBuilderAPI_Transform xform(shape->getShape(1), trsf);
        TopoDS_Shape transShape = xform.Shape();
        Handle(TopTools_HSequenceOfShape) aHSequenceOfShape = new TopTools_HSequenceOfShape;
        aHSequenceOfShape->Append(transShape);
        Bnd_Box B;
        BRepBndLib::Add(transShape, B);
        B.Get(Xmin, Ymin, Zmin, Xmax, Ymax, Zmax);
        ShapeContainer* transContainer = new ShapeContainer(aHSequenceOfShape, Zmin);
        *(moveResult + i * sizeof(void*)) = transContainer;
    }
    return true;
}

EXPORT bool deleteShape(void** pt, int count)
{
    for (int i = 0; i < count; i++)
    {
        ShapeContainer* shape = (ShapeContainer*)(*(pt + i * sizeof(void*)));
        if (shape != NULL) {
            shape->shapeSequence->Clear();
            // 这里不要delete 否则析构函数会再次delete，造成崩溃
            //shape->shapeSequence->Delete();
        }
        delete shape;
        *(pt + i * sizeof(void*)) = NULL;
    }
    return true;
}

EXPORT void getShapeBoundary(void** pt, int index, double* Zmin, double* Zmax, double* Ymin, double* Ymax, double* Xmin, double* Xmax)
{
    ShapeContainer* shape = ShapeContainer::getContainer(pt, index);
    Bnd_Box B;
    BRepBndLib::Add(shape->getShape(1), B);
    B.Get(*Xmin, *Ymin, *Zmin, *Xmax, *Ymax, *Zmax);
}

EXPORT void deleteSlice(void* pt)
{
    ShapeContainer* container = (ShapeContainer*)pt;
    delete container;
}

Handle_TopTools_HSequenceOfShape getWires(TopoDS_Shape shape) {
    Handle(TopTools_HSequenceOfShape) Edges = new TopTools_HSequenceOfShape();
    TopoDS_Shape wireShape = shape;
    BRepBuilderAPI_MakeWire mkWire;
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

EXPORT ShapeContainer* SliceShape(void** pt, int index, double height)
{
    ShapeContainer* shape = ShapeContainer::getContainer(pt, index);
    // 生成水平面，进行逐层切割
    TopoDS_Shape face = shape->getShape(1);
    double Xmin, Ymin, Zmin, Xmax, Ymax, Zmax;
    Bnd_Box C;
    BRepBndLib::Add(face, C);
    C.Get(Xmin, Ymin, Zmin, Xmax, Ymax, Zmax);
    if (Zmax - Zmin < 0.0001) {
        // 水平面，不存在交点
        return NULL;
    }
    gp_Pln originPlane = gp_Pln(0, 0, 1, -height);
    BRepAlgo_Section section(shape->getShape(1), originPlane, Standard_True);
    TopoDS_Shape sectionShape = section.Shape();
    if (sectionShape.IsNull())
        return NULL;
    Handle_TopTools_HSequenceOfShape Wires = getWires(sectionShape);
    ShapeContainer* container = new ShapeContainer(Wires);
    return container;
}

EXPORT void combineShapes(void** pt, void** pt1, void** pt2) {
    ShapeContainer* container1 = ShapeContainer::getContainer(pt1, 0);
    ShapeContainer* container2 = ShapeContainer::getContainer(pt2, 0);
    TopoDS_Shape shape1 = container1->getShape(1);
    TopoDS_Shape shape2 = container2->getShape(1);
    TopoDS_Shape shape = BRepAlgoAPI_Fuse(shape1, shape2);
    double Xmin, Ymin, Zmin, Xmax, Ymax, Zmax;
    Bnd_Box B;
    BRepBndLib::Add(shape, B);
    B.Get(Xmin, Ymin, Zmin, Xmax, Ymax, Zmax);
    Handle(TopTools_HSequenceOfShape) aHSequenceOfShape = new TopTools_HSequenceOfShape;
    aHSequenceOfShape->Append(shape);
    ShapeContainer* container = new ShapeContainer(aHSequenceOfShape, Zmin);
    *pt = container;
}

EXPORT ShapeContainer** getLocatPlane(void** pt, int index, int* count) {
    // 显示定位面
    ShapeContainer* containers[128] = {0};
    ShapeContainer* shape = ShapeContainer::getContainer(pt, index);
    TopExp_Explorer explorer;
    double Xmin, Ymin, Zmin, Xmax, Ymax, Zmax;
    int counter = 0;
    for (explorer.Init(shape->getShape(1), TopAbs_FACE); explorer.More(); explorer.Next()) {
        TopoDS_Shape currentFace = explorer.Current();
        Bnd_Box C;
        BRepBndLib::Add(currentFace, C);
        C.Get(Xmin, Ymin, Zmin, Xmax, Ymax, Zmax);
        if (Zmax - Zmin < 0.0001) {
            Handle_TopTools_HSequenceOfShape wires = getWires(currentFace);
            containers[counter++] = new ShapeContainer(wires, Zmin);
        }
    }
    *count = counter;
    if (counter == 0)
        return NULL;
    ShapeContainer** result = new ShapeContainer*[counter];
    for (int i = 0; i < counter; i++) {
        result[i] = containers[i];
    }
    return result;
}

EXPORT ShapeContainer* getShapeContainer(void** pt, int index, double* height) {
    ShapeContainer** containers = (ShapeContainer**)pt;
    *height = containers[index]->height;
    return containers[index];
}

EXPORT bool exportStep(char* fileName, ShapeContainer** slices, int length) {
    STEPControl_StepModelType aType = STEPControl_AsIs;
    STEPControl_Writer        aWriter;
    for (int i = 0; i < length; i++) {
        int length = slices[i]->shapeSequence->Length();
        if (length == 0)
            continue;
        TopoDS_Compound aCompound;
        BRep_Builder aBuilder;
        aBuilder.MakeCompound(aCompound);
        for (int j = 1; j <= length; j++) {
            aBuilder.Add(aCompound, slices[i]->shapeSequence->Value(j));
        }
        if (aWriter.Transfer(aCompound, aType) != IFSelect_RetDone)
            return false;
    }
    return aWriter.Write(fileName) == IFSelect_RetDone;
}

EXPORT bool exportTransformStep(char* fileName, ShapeContainer** slices, int length) {
    return exportStep(fileName, slices, length);
}

EXPORT Slice* exportSlice(Slice** ss, int layerNum, Slice** current, int* type, double* x, double* y, double* z, double* p) {
    Slice* slice = NULL;
    if (*current == NULL) {
        slice = &(*ss)[layerNum];
        *current = &(*ss)[layerNum];
    }
    else
        slice = *current;
    *type = slice->type;
    *z = slice->z;
    switch (slice->type)
    {
    case EdgeType::circle: {
        Circle* circle = (Circle*)(slice->data);
        *x = circle->center.x;
        *y = circle->center.y;
        p[0] = circle->radius;
        p[1] = circle->startAngle;
        p[2] = circle->endAngle;
        p[3] = circle->start.x;
        p[4] = circle->start.y;
        p[5] = circle->end.x;
        p[6] = circle->end.y;
    }
        break;
    case EdgeType::bSplice: {
        BSpline* bspline = (BSpline*)(slice->data);
        *x = bspline->start.x;
        *y = bspline->start.y;
        p[0] = bspline->polesCnt;
        p[1] = bspline->end.x;
        p[2] = bspline->end.y;
    }
        break;
    case EdgeType::line: {
        Line* line = (Line*)(slice->data);
        *x = line->start.x;
        *y = line->start.y;
        p[0] = line->end.x;
        p[1] = line->end.y;
    }
        break;
    default:
        break;
    }
    return slice->next;
}

EXPORT bool exportBspline(Slice* current, int index, double* x, double* y) {
    BSpline* bspline = (BSpline*)(current->data);
    *x = bspline->poles[index].x;
    *y = bspline->poles[index].y;
    return true;
}