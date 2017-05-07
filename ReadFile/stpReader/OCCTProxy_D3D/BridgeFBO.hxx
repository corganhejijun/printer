#ifndef BRIDGE_FBO_HEADER
#define BRIDGE_FBO_HEADER

#include <OpenGl_GlCore20.hxx>
#include <OpenGl_FrameBuffer.hxx>

#include <TopoDS_Shape.hxx>
#include <TopTools_HSequenceOfShape.hxx>

//! Implements bridge FBO for direct rendering to Direct3D surfaces.
class BridgeFBO : public OpenGl_FrameBuffer
{
public:

  //! Creates new bridge FBO.
  BridgeFBO() : OpenGl_FrameBuffer()
  {
    //
  }

  //! Releases resources of bridge FBO.
  ~BridgeFBO()
  {
    Release (NULL);
  }

  //! Releases resources of bridge FBO.
  virtual void Release (OpenGl_Context* theGlContext);

  //! Initializes OpenGL FBO for Direct3D interoperability.
  Standard_Boolean Init (const Handle(OpenGl_Context)& theGlContext,
                         void*                         theD3DDevice);

  //! Binds Direcr3D color buffer to OpenGL texture.
  Standard_Boolean RegisterD3DColorBuffer (const Handle(OpenGl_Context)& theGlContext,
                                           void*                         theD3DBuffer,
                                           void*                         theBufferShare);

  //! Locks Direct3D objects for OpenGL drawing.
  virtual void BindBuffer (const Handle(OpenGl_Context)& theGlContext);

  //! Unlocks Direct3D objects after OpenGL drawing.
  virtual void UnbindBuffer (const Handle(OpenGl_Context)& theGlContext);

  //! Resizes  buffer according to Direct3D surfaces.
  void Resize (const Handle(OpenGl_Context)& theGlContext,
               int                           theSizeX,
               int                           theSizeY);

private:

  HANDLE myGlD3DHandle;
  HANDLE myGlD3DSharedColorHandle;
};

public class ShapeContainer
{
public:
    enum ShapeType
    {
        Entity, Slice
    };
    ShapeType type;
    double height;
    ShapeContainer(const Handle(TopTools_HSequenceOfShape) shape, double z = 0){
        type = ShapeType::Slice;
        shapeSequence = shape;
        height = z;
    }
    ShapeContainer(const TopoDS_Shape& shape) : Shape(shape){
        type = ShapeType::Entity;
    }
    TopoDS_Shape Shape;
    Handle(TopTools_HSequenceOfShape) shapeSequence;

    static ShapeContainer* getContainer(void** pt, int index)
    {
        ShapeContainer** shapes = (ShapeContainer**)pt;
        ShapeContainer* shape = *(shapes + index*sizeof(void*));
        return shape;
    }
};

#endif