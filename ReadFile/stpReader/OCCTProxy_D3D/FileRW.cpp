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

void addCircle(Slice* slice, double centerX, double centerY, double startAngle, double endAngle, double radius) {
    Slice* target = getEmptySlice(slice);
    target->type = EdgeType::circle;
    target->data = new Circle();
    Circle* circle = (Circle*)(target->data);
    circle->center.x = centerX;
    circle->center.y = centerY;
    circle->startAngle = startAngle;
    circle->endAngle = endAngle;
    circle->radius = radius;
}

void addLine(Slice* slice, double beginX, double beginY, double endX, double endY) {
    Slice* target = getEmptySlice(slice);
    target->type = EdgeType::line;
    target->data = new Line();
    Line* line = (Line*)(target->data);
    line->start.x = beginX;
    line->start.y = beginY;
    line->end.x = endX;
    line->end.y = endY;
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
    b->poles = bSplinePoles;
    b->polesCnt = polesCnt;
}

void addVertex(TopoDS_Shape shape, Slice* slice, Point* bSplinePoles, int polesCnt) {
    int i = 0;
    double beginX, beginY, endX, endY;
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
    else
        addLine(slice, beginX, beginY, endX, endY);
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
                addCircle(slice, center.X(), center.Y(), first, last, circle.Radius());
                for (int i = 0; i < layer; i++)
                    file << '\t';
                file << "circle radius is " << circle.Radius() << 
                    ", center is (" << center.X() << "," << center.Y() << "," << center.Z() << ")" 
                    ", start angle is " << (first/M_PI*360) << ", end angle is " << (last/M_PI*360) << endl;
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
                for (int i = 1; i <= polesCount; i++) {
                    gp_Pnt pt = Poles(i);
                    pointArray[i - 1].x = pt.X();
                    pointArray[i - 1].y = pt.Y();
                    file << "(" << pt.X() << "," << pt.Y() << "," << pt.Z() << ") ";
                }
                file << endl;
                addVertex(child, slice, pointArray, polesCount);
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

EXPORT bool ImportStep(char* theFileName, int* cnt, void** shapes, bool isSlice, void** slice)
{
    int length = *cnt;
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
    for (Standard_Integer aRootIter = 1; aRootIter <= aNbRoot; ++aRootIter)
    {
        aReader.TransferRoot(aRootIter);
        int aNbShap = aReader.NbShapes();
        if (aNbShap > 0)
        {
            Handle(TopTools_HSequenceOfShape) aHSequenceOfShape = new TopTools_HSequenceOfShape;
            //for (int aShapeIter = 1; aShapeIter <= aNbShap; ++aShapeIter)
            {
                TopoDS_Shape shape = aReader.Shape(aNbShap);
                if (isSlice) {
                    file << "begin shape" << endl;
                    Slice* sss = &(*ss)[(*cnt)];
                    sss->data = sss->next = sss->prev = NULL;
                    sss->type = EdgeType::unknown;
                    showType(shape, file, sss);
                    file << "end shape" << endl;
                    /*
                    for (TopoDS_Iterator anIt(shape); anIt.More(); anIt.Next()) {
                        TopoDS_Shape aChild = anIt.Value();
                        for (TopoDS_Iterator childIt(aChild); childIt.More(); childIt.Next()) {
                            const TopoDS_Shape childChild = childIt.Value();
                            printf("child type is %d", childChild.ShapeType());
                            TopoDS_Vertex vertex = TopoDS::Vertex(childChild);
                            gp_Pnt pt = BRep_Tool::Pnt(vertex);
                            printf(" (%f, %f, %f)\n", pt.X(), pt.Y(), pt.Z());
                        }
                        Standard_Real first = 0, last = 0;
                        Handle_Geom_Curve theCurve = BRep_Tool::Curve(TopoDS::Edge(aChild), first, last);
                        BRepAdaptor_Curve adpCurve = BRepAdaptor_Curve(TopoDS::Edge(aChild));
                        printf("type is %d, %d, (%f, %f)\n", aChild.ShapeType(), adpCurve.GetType(), first, last);
                    }
                    */
                }
                aHSequenceOfShape->Append(shape);
            }
            ShapeContainer* sc = new ShapeContainer(aHSequenceOfShape);
            *(shapes + (*cnt)*sizeof(void*)) = sc;
            (*cnt)++;
        }
    }
    if ((*cnt) == 1) {
        ShapeContainer* shape = ShapeContainer::getContainer(shapes, 0);
        shape->Shape = shape->shapeSequence->Value(1);
    }
    return true;
}

EXPORT void* getSliceFromShape(void** shapes, int index) {
    return *(shapes + index*sizeof(void*));
}

EXPORT bool deleteShape(void** pt, int count)
{
    for (int i = 0; i < count; i++)
    {
        ShapeContainer* shape = (ShapeContainer*)(*(pt + i * sizeof(void*)));
        if (shape->type == ShapeContainer::Slice) {
            shape->shapeSequence->Clear();
            shape->shapeSequence->Delete();
        }
        delete shape;
    }
    return true;
}

EXPORT void getShapeBoundary(void** pt, int index, double* Zmin, double* Zmax, double* Ymin, double* Ymax, double* Xmin, double* Xmax)
{
    ShapeContainer* shape = ShapeContainer::getContainer(pt, index);
    Bnd_Box B;
    BRepBndLib::Add(shape->Shape, B);
    B.Get(*Xmin, *Ymin, *Zmin, *Xmax, *Ymax, *Zmax);
}

EXPORT void deleteSlice(void* pt)
{
    ShapeContainer* container = (ShapeContainer*)pt;
    if (container->type == ShapeContainer::Entity)
        delete container;
}

EXPORT ShapeContainer* SliceShape(void** pt, int index, double Zmax, double Zmin, double height)
{
    ShapeContainer* shape = ShapeContainer::getContainer(pt, index);
    // 生成水平面，进行逐层切割
    gp_Pln originPlane = gp_Pln(0, 0, 1, -height);
    BRepAlgo_Section section(shape->Shape, originPlane, Standard_True);
    TopoDS_Shape sectionShape = section.Shape();
    if (sectionShape.IsNull())
        return NULL;
    ShapeContainer* container = new ShapeContainer(sectionShape);
    return container;

    // 显示定位面
    /*
    TopExp_Explorer explorer;
    for (explorer.Init(shape->Shape, TopAbs_WIRE); explorer.More(); explorer.Next()) {
    TopoDS_Shape currentFace = explorer.Current();
    Bnd_Box C;
    BRepBndLib::Add(currentFace, C);
    C.Get(Xmin, Ymin, Zmin, Xmax, Ymax, Zmax);
    if (Zmax - Zmin < 0.0001)
    myAISContext()->Display(new AIS_Shape(currentFace), Standard_True);
    }
    */
}

EXPORT bool exportStep(char* fileName, ShapeContainer** slices, int length) {
    STEPControl_StepModelType aType = STEPControl_AsIs;
    STEPControl_Writer        aWriter;
    for (int i = 0; i < length; i++){
        if (aWriter.Transfer(slices[i]->Shape, aType) != IFSelect_RetDone)
            return false;
    }
    return aWriter.Write(fileName) == IFSelect_RetDone;
}