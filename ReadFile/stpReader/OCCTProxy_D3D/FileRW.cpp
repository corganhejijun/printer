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


#define EXPORT extern "C" __declspec( dllexport )

// ============================================
// Import / export functionality
// ============================================
/// <summary>
///Import Step file
/// </summary>
/// <param name="theFileName">Name of import file</param>
EXPORT bool ImportStep(char* theFileName, int* cnt, void** shapes)
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
    for (Standard_Integer aRootIter = 1; aRootIter <= aNbRoot; ++aRootIter)
    {
        aReader.TransferRoot(aRootIter);
        int aNbShap = aReader.NbShapes();
        if (aNbShap > 0)
        {
            for (int aShapeIter = 1; aShapeIter <= aNbShap; ++aShapeIter)
            {
                TopoDS_Shape shape = aReader.Shape(aShapeIter);
                ShapeContainer* sc = new ShapeContainer(shape);
                *(shapes + (*cnt)*sizeof(void*)) = sc;
                (*cnt)++;
                if (*cnt >= length)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

EXPORT bool deleteShape(void** pt, int count)
{
    for (int i = 0; i < count; i++)
    {
        ShapeContainer* shape = (ShapeContainer*)(*(pt + i * sizeof(void*)));
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