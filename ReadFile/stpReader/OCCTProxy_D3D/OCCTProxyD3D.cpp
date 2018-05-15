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
#include <AIS_ListIteratorOfListOfInteractive.hxx>
#include <AIS_ListOfInteractive.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <TopoDS.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <ShapeAnalysis_FreeBounds.hxx>
#include <BRepTools_WireExplorer.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepAlgoAPI_Cut.hxx>

/// <summary>
/// Proxy class encapsulating calls to OCCT C++ classes within
/// C++/CLI class visible from .Net (CSharp)
/// </summary>
public ref class OCCTProxyD3D
{
public:

    OCCTProxyD3D() : myBridgeFBO(NULL)
    {
        //
    }

    // ============================================
    // Viewer functionality
    // ============================================

    /// <summary>
    ///Initialize a viewer
    /// </summary>
    /// <param name="theWnd">System.IntPtr that contains the window handle (HWND) of the control</param>
    bool InitViewer(System::IntPtr theWnd)
    {
        myGraphicDriver() = new OpenGl_GraphicDriver(Handle(Aspect_DisplayConnection)());
        //myGraphicDriver()->ChangeOptions().contextDebug = true;

        TCollection_ExtendedString a3DName("Visu3D");
        myViewer() = new V3d_Viewer(myGraphicDriver(), a3DName.ToExtString(), "", 1000.0,
            V3d_XposYnegZpos, Quantity_NOC_GRAY30,
            V3d_ZBUFFER, V3d_GOURAUD, V3d_WAIT,
            Standard_True, Standard_False);

        myViewer()->SetDefaultLights();
        myViewer()->SetLightOn();
        myView() = myViewer()->CreateView();
        Handle(WNT_Window) aWNTWindow = new WNT_Window(reinterpret_cast<HWND> (theWnd.ToPointer()));
        myView()->SetWindow(aWNTWindow);
        if (!aWNTWindow->IsMapped())
        {
            aWNTWindow->Map();
        }
        myAISContext() = new AIS_InteractiveContext(myViewer());
        myAISContext()->UpdateCurrentViewer();
        myAISContext()->SelectionColor(Quantity_NOC_BLUE1);
        myView()->MustBeResized();
        myView()->ZBufferTriedronSetup(Quantity_NOC_RED, Quantity_NOC_GREEN, Quantity_NOC_BLUE1, 0.8, 0.05, 12);
        myView()->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_BLACK, 0.05, V3d_ZBUFFER);

        gp_Pnt axPt(0, 0, 0);
        gp_Dir axDir(0, 0, 1);
        gp_Ax3 ax3(axPt, axDir);
        myViewer()->SetPrivilegedPlane(ax3);
        myViewer()->SetRectangularGridValues(0, 0, 3, 3, 0);
        myViewer()->SetRectangularGridGraphicValues(3, 3, 0);
        myViewer()->ActivateGrid(Aspect_GT_Rectangular, Aspect_GDM_Lines);
        myView()->Zoom(0, 0, 800, 0);
        return true;
    }

    /// <summary> Initializes OCCT viewer for OpenGL-Direct3D interoperability. </summary>
    bool InitViewer(System::IntPtr theHWND, System::IntPtr theD3DDevice)
    {
        if (!InitViewer(theHWND))
        {
            return false;
        }

        Graphic3d_CView*       aCView = reinterpret_cast<Graphic3d_CView*> (myView()->View()->CView());
        OpenGl_CView*          aCViewGl = reinterpret_cast<OpenGl_CView*>    (aCView->ptrView);
        Handle(OpenGl_Context) aGlContext = aCViewGl->WS->GetGlContext();
        if (aGlContext.IsNull())
        {
            return false;
        }
        if (!aGlContext->IsCurrent())
        {
            aGlContext->MakeCurrent();
        }

        myBridgeFBO = new BridgeFBO();
        if (!myBridgeFBO->Init(aGlContext, theD3DDevice.ToPointer()))
        {
            return false;
        }

        aCView->ptrFBO = myBridgeFBO;
        return true;
    }

    /// <summary> Resizes custom FBO for Direct3D output. </summary>
    bool ResizeBridgeFBO(int theWinSizeX, int theWinSizeY, System::IntPtr theColorSurf, System::IntPtr theColorSurfShare)
    {
        if (myBridgeFBO == NULL)
        {
            return false;
        }

        OpenGl_CView* aCView = reinterpret_cast<OpenGl_CView*> (reinterpret_cast<Graphic3d_CView*> (myView()->View()->CView())->ptrView);
        Handle(OpenGl_Context) aGlContext = aCView->WS->GetGlContext();
        if (aGlContext.IsNull()
            || !aGlContext->MakeCurrent())
        {
            return false;
        }

        myBridgeFBO->Resize(aGlContext, theWinSizeX, theWinSizeY);
        if (!myBridgeFBO->RegisterD3DColorBuffer(aGlContext, theColorSurf.ToPointer(), theColorSurfShare.ToPointer()))
        {
            return false;
        }

        myView()->Camera()->SetAspect(Standard_Real(theWinSizeX) / Standard_Real(theWinSizeY));
        return true;
    }

    /// <summary>
    ///Redraw view
    /// </summary>
    void RedrawView()
    {
        if (!myView().IsNull())
        {
            myView()->Redraw();
        }
    }

    /// <summary>
    ///Update view
    /// </summary>
    void UpdateView(void)
    {
        if (!myView().IsNull())
        {
            myView()->MustBeResized();
        }
    }

    /// <summary>
    ///Set computed mode in false
    /// </summary>
    void SetDegenerateModeOn()
    {
        if (!myView().IsNull())
        {
            myView()->SetComputedMode(Standard_False);
        }
    }

    /// <summary>
    ///Set computed mode in true
    /// </summary>
    void SetDegenerateModeOff()
    {
        if (!myView().IsNull())
        {
            myView()->SetComputedMode(Standard_True);
        }
    }

    /// <summary>
    ///Fit all
    /// </summary>
    void WindowFitAll(int theXmin, int theYmin,
        int theXmax, int theYmax)
    {
        if (!myView().IsNull())
        {
            myView()->WindowFitAll(theXmin, theYmin, theXmax, theYmax);
        }
    }

    /// <summary>
    ///Current place of window
    /// </summary>
    /// <param name="theZoomFactor">Current zoom</param>
    void Place(int theX, int theY, float theZoomFactor)
    {
        Quantity_Factor aZoomFactor = theZoomFactor;
        if (!myView().IsNull())
        {
            myView()->Place(theX, theY, aZoomFactor);
        }
    }

    /// <summary>
    ///Set Zoom
    /// </summary>
    void Zoom(int theX1, int theY1, int theX2, int theY2)
    {
        if (!myView().IsNull())
        {
            myView()->Zoom(theX1, theY1, theX2, theY2);
        }
    }

    double getZoomScale()
    {
        if (!myView().IsNull())
            return myView()->Scale();
        return 1;
    }

    void setZoomScale(double scale)
    {
        if (!myView().IsNull())
            myView()->SetScale(scale);
    }

    void getViewPoint(double& x, double& y, double& z) {
        if (myView().IsNull())
            return;
        myView()->At(x, y, z);
    }

    void setViewPoint(double x, double y, double z) {
        if (myView().IsNull())
            return;
        myView()->SetAt(x, y, z);
    }

    /// <summary>
    ///Set Pan
    /// </summary>
    void Pan(int theX, int theY)
    {
        if (myView().IsNull())
        {
            return;
        }
        myView()->Pan(theX, theY);
    }

    /// <summary>
    ///Rotation
    /// </summary>
    void Rotation(int theX, int theY, double* x, double* y, double* z)
    {
        if (myView().IsNull())
        {
            return;
        }
        myView()->Rotation(theX, theY);
        myView()->Proj(*x, *y, *z);
    }

    void setProjection(double x, double y, double z) {
        if (myView().IsNull())
        {
            return;
        }
        myView()->SetProj(x, y, z);
    }

    /// <summary>
    ///Start rotation
    /// </summary>
    void StartRotation(int theX, int theY)
    {
        if (!myView().IsNull())
        {
            myView()->StartRotation(theX, theY);
        }
    }

    /// <summary>
    ///Select by rectangle
    /// </summary>
    void Select(int theX1, int theY1, int theX2, int theY2)
    {
        if (!myAISContext().IsNull())
        {
            myAISContext()->Select(theX1, theY1, theX2, theY2, myView());
        }
    }

    IntPtr Select(int theX, int theY)
    {
        if (!myAISContext().IsNull() && !myView().IsNull()) {
            myAISContext()->MoveTo(theX, theY, myView());
            myAISContext()->Select();
            myAISContext()->InitCurrent();
            if (!myAISContext()->MoreCurrent())
            {
                return IntPtr::Zero;
            }
            Handle(AIS_InteractiveObject) aCurrent = myAISContext()->Current();
            Handle(AIS_Shape) hashape = Handle(AIS_Shape)::DownCast(aCurrent);
            TopoDS_Shape aShape = hashape->Shape();
            return IntPtr(new ShapeContainer(aShape));
        }
        return IntPtr::Zero;
    }

    bool IsEqual(IntPtr pt1, IntPtr pt2){
        ShapeContainer* shape1 = (ShapeContainer*)pt1.ToPointer();
        ShapeContainer* shape2 = (ShapeContainer*)pt2.ToPointer();
        return (shape1->getShape().IsEqual(shape2->getShape()));
    }

    /// <summary>
    ///Select by click
    /// </summary>
    void Select()
    {
        if (!myAISContext().IsNull())
        {
            myAISContext()->Select();
        }
    }

    /// <summary>
    ///Move view
    /// </summary>
    void MoveTo(int theX, int theY)
    {
        if (!myAISContext().IsNull() && !myView().IsNull())
        {
            myAISContext()->MoveTo(theX, theY, myView());
        }
    }

    /// <summary>
    ///Select by rectangle with pressed "Shift" key
    /// </summary>
    void ShiftSelect(int theX1, int theY1, int theX2, int theY2)
    {
        if (!myAISContext().IsNull() && !myView().IsNull())
        {
            myAISContext()->ShiftSelect(theX1, theY1, theX2, theY2, myView());
        }
    }

    /// <summary>
    ///Select by "Shift" key
    /// </summary>
    void ShiftSelect()
    {
        if (!myAISContext().IsNull())
        {
            myAISContext()->ShiftSelect();
        }
    }

    /// <summary>
    ///Set background color
    /// </summary>
    void BackgroundColor(int& theRed, int& theGreen, int& theBlue)
    {
        if (!myView().IsNull())
        {
            Quantity_Color aColor = myView()->BackgroundColor();
            theRed = (int)aColor.Red() * 255;
            theGreen = (int)aColor.Green() * 255;
            theBlue = (int)aColor.Blue() * 255;
        }
    }

    /// <summary>
    ///Get background color Red
    /// </summary>
    int GetBGColR()
    {
        int anRgb[3];
        BackgroundColor(anRgb[0], anRgb[1], anRgb[2]);
        return anRgb[0];
    }

    /// <summary>
    ///Get background color Green
    /// </summary>
    int GetBGColG()
    {
        int anRgb[3];
        BackgroundColor(anRgb[0], anRgb[1], anRgb[2]);
        return anRgb[1];
    }

    /// <summary>
    ///Get background color Blue
    /// </summary>
    int GetBGColB()
    {
        int anRgb[3];
        BackgroundColor(anRgb[0], anRgb[1], anRgb[2]);
        return anRgb[2];
    }

    /// <summary>
    ///Update current viewer
    /// </summary>
    void UpdateCurrentViewer()
    {
        if (!myAISContext().IsNull())
        {
            myAISContext()->UpdateCurrentViewer();
        }
    }

    /// <summary>
    ///Front side
    /// </summary>
    void FrontView()
    {
        if (!myView().IsNull())
        {
            myView()->SetProj(V3d_Xpos);
        }
    }

    /// <summary>
    ///Top side
    /// </summary>
    void TopView()
    {
        if (!myView().IsNull())
        {
            myView()->SetProj(V3d_Zpos);
        }
    }

    /// <summary>
    ///Left side
    /// </summary>
    void LeftView()
    {
        if (!myView().IsNull())
        {
            myView()->SetProj(V3d_Ypos);
        }
    }

    /// <summary>
    ///Back side
    /// </summary>
    void BackView()
    {
        if (!myView().IsNull())
        {
            myView()->SetProj(V3d_Xneg);
        }
    }

    /// <summary>
    ///Right side
    /// </summary>
    void RightView()
    {
        if (!myView().IsNull())
        {
            myView()->SetProj(V3d_Yneg);
        }
    }

    /// <summary>
    ///Bottom side
    /// </summary>
    void BottomView()
    {
        if (!myView().IsNull())
        {
            myView()->SetProj(V3d_Zneg);
        }
    }

    /// <summary>
    ///Axo side
    /// </summary>
    void AxoView()
    {
        if (!myView().IsNull())
        {
            myView()->SetProj(V3d_XposYnegZpos);
        }
    }

    /// <summary>
    ///Scale
    /// </summary>
    float Scale()
    {
        return myView().IsNull()
            ? -1.0f
            : float(myView()->Scale());
    }

    /// <summary>
    ///Zoom in all view
    /// </summary>
    void ZoomAllView()
    {
        if (!myView().IsNull())
        {
            myView()->FitAll();
            myView()->ZFitAll();
        }
    }

    /// <summary>
    ///Reset view
    /// </summary>
    void Reset()
    {
        if (!myView().IsNull())
        {
            myView()->Reset();
        }
    }

    /// <summary>
    ///Set display mode of objects
    /// </summary>
    /// <param name="theMode">Set current mode</param>
    void SetDisplayMode(int theMode)
    {
        if (myAISContext().IsNull())
        {
            return;
        }

        AIS_DisplayMode aCurrentMode = theMode == 0 ? AIS_WireFrame : AIS_Shaded;
        if (myAISContext()->NbCurrents() == 0 || myAISContext()->NbSelected() == 0)
        {
            myAISContext()->SetDisplayMode(aCurrentMode);
        }
        else
        {
            for (myAISContext()->InitCurrent(); myAISContext()->MoreCurrent(); myAISContext()->NextCurrent())
            {
                myAISContext()->SetDisplayMode(myAISContext()->Current(), theMode, Standard_False);
            }
        }
        myAISContext()->UpdateCurrentViewer();
    }

    /// <summary>
    ///Set color
    /// </summary>
    void SetColor(int theR, int theG, int theB)
    {
        if (myAISContext().IsNull())
        {
            return;
        }

        Quantity_Color aCol(theR / 255.0, theG / 255.0, theB / 255.0, Quantity_TOC_RGB);
        for (; myAISContext()->MoreCurrent(); myAISContext()->NextCurrent())
        {
            myAISContext()->SetColor(myAISContext()->Current(), aCol.Name());
        }
    }

    /// <summary>
    ///Get object color red
    /// </summary>
    int GetObjColR()
    {
        int anRgb[3];
        ObjectColor(anRgb[0], anRgb[1], anRgb[2]);
        return anRgb[0];
    }

    /// <summary>
    ///Get object color green
    /// </summary>
    int GetObjColG()
    {
        int anRgb[3];
        ObjectColor(anRgb[0], anRgb[1], anRgb[2]);
        return anRgb[1];
    }

    /// <summary>
    ///Get object color blue
    /// </summary>
    int GetObjColB()
    {
        int anRgb[3];
        ObjectColor(anRgb[0], anRgb[1], anRgb[2]);
        return anRgb[2];
    }

    /// <summary>
    ///Get object color R/G/B
    /// </summary>
    void ObjectColor(int& theRed, int& theGreen, int& theBlue)
    {
        if (myAISContext().IsNull())
        {
            return;
        }

        theRed = 255;
        theGreen = 255;
        theBlue = 255;
        myAISContext()->InitCurrent();
        if (!myAISContext()->MoreCurrent())
        {
            return;
        }

        Handle(AIS_InteractiveObject) aCurrent = myAISContext()->Current();
        if (aCurrent->HasColor())
        {
            Quantity_Color anObjCol = myAISContext()->Color(myAISContext()->Current());
            theRed = int(anObjCol.Red()   * 255.0);
            theGreen = int(anObjCol.Green() * 255.0);
            theBlue = int(anObjCol.Blue()  * 255.0);
        }
    }

    /// <summary>
    ///Set background color R/G/B
    /// </summary>
    void SetBackgroundColor(int theRed, int theGreen, int theBlue)
    {
        if (!myView().IsNull())
        {
            myView()->SetBackgroundColor(Quantity_TOC_RGB, theRed / 255.0, theGreen / 255.0, theBlue / 255.0);
        }
    }

    /// <summary>
    ///Erase objects
    /// </summary>
    void EraseObjects()
    {
        if (myAISContext().IsNull())
        {
            return;
        }
        myAISContext()->EraseAll();
        /*
        for (myAISContext()->InitCurrent(); myAISContext()->MoreCurrent(); myAISContext()->NextCurrent())
        {
            myAISContext()->Erase(myAISContext()->Current(), Standard_True);
        }
        myAISContext()->ClearCurrents();
        */
    }

    /// <summary>
    ///Get version
    /// </summary>
    float GetOCCVersion()
    {
        return (float)OCC_VERSION;
    }

    /// <summary>
    ///set material
    /// </summary>
    void SetMaterial(int theMaterial)
    {
        if (myAISContext().IsNull())
        {
            return;
        }
        for (myAISContext()->InitCurrent(); myAISContext()->MoreCurrent(); myAISContext()->NextCurrent())
        {
            myAISContext()->SetMaterial(myAISContext()->Current(), (Graphic3d_NameOfMaterial)theMaterial);
        }
        myAISContext()->UpdateCurrentViewer();
    }

    /// <summary>
    ///Return true if object is selected
    /// </summary>
    bool IsObjectSelected()
    {
        if (myAISContext().IsNull())
        {
            return false;
        }
        myAISContext()->InitCurrent();
        return myAISContext()->MoreCurrent() != Standard_False;
    }

    /// <summary>
    ///Return display mode
    /// </summary>
    int DisplayMode()
    {
        if (myAISContext().IsNull())
        {
            return -1;
        }

        bool isOneOrMoreInShading = false;
        bool isOneOrMoreInWireframe = false;
        for (myAISContext()->InitCurrent(); myAISContext()->MoreCurrent(); myAISContext()->NextCurrent())
        {
            if (myAISContext()->IsDisplayed(myAISContext()->Current(), AIS_Shaded))
            {
                isOneOrMoreInShading = true;
            }
            if (myAISContext()->IsDisplayed(myAISContext()->Current(), AIS_WireFrame))
            {
                isOneOrMoreInWireframe = true;
            }
        }
        if (isOneOrMoreInShading
            && isOneOrMoreInWireframe)
        {
            return 10;
        }
        else if (isOneOrMoreInShading)
        {
            return 1;
        }
        else if (isOneOrMoreInWireframe)
        {
            return 0;
        }
        return -1;
    }

    /// <summary>
    ///Create new view
    /// </summary>
    /// <param name="theWnd">System.IntPtr that contains the window handle (HWND) of the control</param>
    void CreateNewView(System::IntPtr theWnd)
    {
        if (myAISContext().IsNull())
        {
            return;
        }

        myView() = myAISContext()->CurrentViewer()->CreateView();
        if (myGraphicDriver().IsNull())
        {
            myGraphicDriver() = new OpenGl_GraphicDriver(Handle(Aspect_DisplayConnection)());
            //myGraphicDriver()->ChangeOptions().contextDebug = true;
        }
        Handle(WNT_Window) aWNTWindow = new WNT_Window(reinterpret_cast<HWND> (theWnd.ToPointer()));
        myView()->SetWindow(aWNTWindow);
        Standard_Integer aWidth = 100, aHeight = 100;
        aWNTWindow->Size(aWidth, aHeight);
        if (!aWNTWindow->IsMapped())
        {
            aWNTWindow->Map();
        }
    }

    /// <summary>
    ///Set AISContext
    /// </summary>
    bool SetAISContext(OCCTProxyD3D^ theViewer)
    {
        this->myAISContext() = theViewer->GetContext();
        if (myAISContext().IsNull())
        {
            return false;
        }
        return true;
    }

    /// <summary>
    ///Get AISContext
    /// </summary>
    Handle_AIS_InteractiveContext GetContext()
    {
        return myAISContext();
    }

public:

    bool displayShape(System::IntPtr pt, double transparency, double r, double g, double b)
    {
        //ShapeContainer* shape = ShapeContainer::getContainer((void**)pt.ToPointer(), index);
        //Handle(AIS_Shape) aisShape = new AIS_Shape(shape->getShape(1));
        ShapeContainer* shape = (ShapeContainer*)(pt.ToPointer());
        Handle(AIS_Shape) aisShape = new AIS_Shape(shape->getShape());
        if (transparency < 0 || transparency >= 1)
            aisShape->SetTransparency();
        else
            aisShape->SetTransparency(transparency);
        aisShape->SetColor(Quantity_Color(r / 255.0, g / 255.0, b / 255.0, Quantity_TOC_RGB));
        myAISContext()->Display(aisShape, Standard_True);
        return true;
    }

    void cleanScene() {
        myAISContext()->RemoveAll();
    }

    bool displaySlice(System::IntPtr pt, double r, double g, double b)
    {
        if (pt == IntPtr::Zero)
            return false;
        ShapeContainer* shape = (ShapeContainer*)pt.ToPointer();
        Handle(AIS_Shape) aisShape = new AIS_Shape(shape->getShape());
		aisShape->SetColor(Quantity_Color(r / 255.0, g / 255.0, b / 255.0, Quantity_TOC_RGB));
        myAISContext()->Display(aisShape, Standard_True);
        myAISContext()->SetSelected(aisShape);
        return true;
    }

    bool displaySliceCut(System::IntPtr pt, double height, double r, double g, double b) {
        if (pt == IntPtr::Zero)
            return false;
        myAISContext()->RemoveAll();
        ShapeContainer* container = (ShapeContainer*) pt.ToPointer();
        TopoDS_Shape shape = container->getShape();
        double Xmin, Ymin, Zmin, Xmax, Ymax, Zmax;
        Bnd_Box C;
        BRepBndLib::Add(shape, C);
        C.Get(Xmin, Ymin, Zmin, Xmax, Ymax, Zmax);
        gp_Pnt pt1 = gp_Pnt(Xmin, Ymin, Zmin);
        gp_Pnt pt2 = gp_Pnt(Xmax, Ymax, height);
        TopoDS_Shape box = BRepPrimAPI_MakeBox(pt1, pt2).Shape();
        TopoDS_Shape cutResult = BRepAlgoAPI_Cut(shape, box);
        Handle(AIS_Shape) aisShape = new AIS_Shape(cutResult);
		aisShape->SetColor(Quantity_Color(r / 255.0, g / 255.0, b / 255.0, Quantity_TOC_RGB));
        myAISContext()->Display(aisShape);
        return true;
    }

    bool selectSlice(System::IntPtr pt) {
        ShapeContainer* shape = (ShapeContainer*)pt.ToPointer();
        Handle(AIS_Shape) aisShape = new AIS_Shape(shape->getShape());
        if (myAISContext()->IsSelected(aisShape))
            return false;
        myAISContext()->SetSelected(aisShape);
        return true;
    }

    bool strechSlice(double thickness, System::IntPtr ptShape, double r, double g, double b) {
        ShapeContainer* sc = (ShapeContainer*)ptShape.ToPointer();
        if (sc == NULL)
            return false;
        gp_Vec aPrismVec(0, 0, thickness);
        TopoDS_Shape myBody = BRepPrimAPI_MakePrism(sc->getShape(), aPrismVec, Standard_True);
        Handle(AIS_Shape) aisShape = new AIS_Shape(myBody);
		aisShape->SetColor(Quantity_Color(r / 255.0, g / 255.0, b / 255.0, Quantity_TOC_RGB));
        myAISContext()->SetDisplayMode(AIS_DisplayMode::AIS_Shaded);
        myAISContext()->Display(aisShape);
        return true;
    }

    /// <summary>
    ///Import Iges file
    /// </summary>
    /// <param name="theFileName">Name of import file</param>
    bool ImportIges(char* theFileName)
    {
        IGESControl_Reader aReader;
        if (aReader.ReadFile(theFileName) != IFSelect_RetDone)
        {
            return false;
        }

        aReader.TransferRoots();
        TopoDS_Shape aShape = aReader.OneShape();
        myAISContext()->Display(new AIS_Shape(aShape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
        return true;
    }

    /// <summary>
    ///Export Step file
    /// </summary>
    /// <param name="theFileName">Name of export file</param>
    bool ExportStep(char* theFileName)
    {
        STEPControl_StepModelType aType = STEPControl_AsIs;
        STEPControl_Writer        aWriter;
        AIS_ListOfInteractive ar;
        myAISContext()->ObjectsInside(ar);
        AIS_ListIteratorOfListOfInteractive it(ar);
        for (; it.More(); it.Next()) {
            Handle(AIS_InteractiveObject) aisobj = it.Value();
            Handle(AIS_Shape) hashape = Handle(AIS_Shape)::DownCast(aisobj);
            TopoDS_Shape aShape = hashape->Shape();
            if (aWriter.Transfer(aShape, aType) != IFSelect_RetDone)
            {
                return false;
            }
        }
        return aWriter.Write(theFileName) == IFSelect_RetDone;
    }

    /// <summary>
    ///Export Iges file
    /// </summary>
    /// <param name="theFileName">Name of export file</param>
    bool ExportIges(char* theFileName)
    {
        IGESControl_Controller::Init();
        IGESControl_Writer aWriter(Interface_Static::CVal("XSTEP.iges.unit"),
            Interface_Static::IVal("XSTEP.iges.writebrep.mode"));
        for (myAISContext()->InitCurrent(); myAISContext()->MoreCurrent(); myAISContext()->NextCurrent())
        {
            Handle(AIS_Shape) anIS = Handle(AIS_Shape)::DownCast(myAISContext()->Current());
            if (anIS.IsNull())
            {
                return false;
            }

            aWriter.AddShape(anIS->Shape());
        }

        aWriter.ComputeModel();
        return aWriter.Write(theFileName) != Standard_False;
    }

    /// <summary>
    ///Export Stl file
    /// </summary>
    /// <param name="theFileName">Name of export file</param>
    bool ExportStl(char* theFileName)
    {
        TopoDS_Compound aComp;
        BRep_Builder    aBuilder;
        aBuilder.MakeCompound(aComp);
        for (myAISContext()->InitCurrent(); myAISContext()->MoreCurrent(); myAISContext()->NextCurrent())
        {
            Handle(AIS_Shape) anIS = Handle(AIS_Shape)::DownCast(myAISContext()->Current());
            if (anIS.IsNull())
            {
                return false;
            }
            aBuilder.Add(aComp, anIS->Shape());
        }

        StlAPI_Writer aWriter;
        aWriter.Write(aComp, theFileName);
        return true;
    }

    /// <summary>
    ///Initialize OCCTProxyD3D
    /// </summary>
    void InitOCCTProxy()
    {
        myGraphicDriver().Nullify();
        myViewer().Nullify();
        myView().Nullify();
        myAISContext().Nullify();
    }

private:

    NCollection_Haft<Handle_V3d_Viewer>             myViewer;
    NCollection_Haft<Handle_V3d_View>               myView;
    NCollection_Haft<Handle_AIS_InteractiveContext> myAISContext;
    NCollection_Haft<Handle_OpenGl_GraphicDriver>   myGraphicDriver;
    BridgeFBO*                                      myBridgeFBO;     //!< Provides output to Direct3D buffers

};
