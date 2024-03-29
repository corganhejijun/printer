#pragma once
/*
*    Copyright (c) 2013 eryar All Rights Reserved.
*
*           File : DxfReader.h
*         Author : eryar@163.com
*           Date : 2013-12-22 14:40
*        Version : 1.0v
*
*    Description : Use dxflib to import entities from DXF to OpenCascade.
*
*      Key Words : OpenCascade, dxflib, DXF, DataExchange
*
*/

#pragma once

//#define WNT
#include <BRep_Builder.hxx>
#include <TopTools_HSequenceOfShape.hxx>

#include <memory>

#include "dl_dxf.h"
#include "dl_creationadapter.h"

/**
* @breif Facade dxflib for OpenCascade DataExchange with DXF.
*/
class DxfReader : public DL_CreationAdapter
{
public:
    /**
    * @brief constructor.
    * @param fileName [in] dxf file name with path.
    */
    DxfReader(const wchar_t* fileName);
    ~DxfReader(void);

    /**
    * @brief Get the shape of the dxf.
    * @return OpenCascade topology shape.
    */
    const TopTools_HSequenceOfShape GetShape(void) const;

public:
    virtual void addPoint(const DL_PointData&);
    virtual void addLine(const DL_LineData& line);
    virtual void addArc(const DL_ArcData& arc);
    virtual void addCircle(const DL_CircleData& circle);
    virtual void addEllipse(const DL_EllipseData&);

    virtual void addPolyline(const DL_PolylineData& polyline);
    virtual void addVertex(const DL_VertexData&);

    virtual void addSpline(const DL_SplineData&);
    virtual void addKnot(const DL_KnotData&);
    virtual void addControlPoint(const DL_ControlPointData&);

    virtual void add3dFace(const DL_3dFaceData&);
    virtual void addSolid(const DL_SolidData& solid);

private:
    std::auto_ptr<DL_Dxf> mDxf;

    TopTools_HSequenceOfShape mShapeList;
    BRep_Builder mBuilder;
};
