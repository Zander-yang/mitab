/**********************************************************************
 * $Id: mitab_imapinfofile.cpp,v 1.1 1999-11-08 04:17:27 stephane Exp $
 *
 * Name:     mitab_imapinfo
 * Project:  MapInfo mid/mif Read/Write library
 * Language: C++
 * Purpose:  Implementation of the IMapInfoFile class, super class of
 *           of MIFFile and TABFile
 * Author:   Daniel Morissette, danmo@videotron.ca
 *
 **********************************************************************
 * Copyright (c) 1999, Daniel Morissette
 *
 * All rights reserved.  This software may be copied or reproduced, in
 * all or in part, without the prior written consent of its author,
 * Daniel Morissette (danmo@videotron.ca).  However, any material copied
 * or reproduced must bear the original copyright notice (above), this 
 * original paragraph, and the original disclaimer (below).
 * 
 * The entire risk as to the results and performance of the software,
 * supporting text and other information contained in this file
 * (collectively called the "Software") is with the user.  Although 
 * considerable efforts have been used in preparing the Software, the 
 * author does not warrant the accuracy or completeness of the Software.
 * In no event will the author be liable for damages, including loss of
 * profits or consequential damages, arising out of the use of the 
 * Software.
 * 
 **********************************************************************
 *
 * $Log: mitab_imapinfofile.cpp,v $
 * Revision 1.1  1999-11-08 04:17:27  stephane
 * First Revision
 *
 *
 **********************************************************************/

#include "mitab.h"

IMapInfoFile::IMapInfoFile()
{
    m_poFilterGeom = NULL;
    
}

IMapInfoFile::~IMapInfoFile()
{
    if( m_poFilterGeom != NULL )
    {
	delete m_poFilterGeom;
	m_poFilterGeom = NULL;
    }
}

OGRFeature *IMapInfoFile::GetNextFeature()
{
    OGRFeature *poFeature, *poFeatureRef;
      
    poFeatureRef = GetFeatureRef(m_nCurFeatureId+1);
    if (poFeatureRef)
    {
	poFeature = poFeatureRef->Clone();
	poFeature->SetFID(poFeatureRef->GetFID());
	return poFeature;
    }
    else
      return NULL;
}

OGRErr     IMapInfoFile::CreateFeature(OGRFeature *poFeature)
{
    TABFeature *poTABFeature;
    OGRGeometry   *poGeom;

    poGeom = poFeature->GetGeometryRef();

    switch (poGeom->getGeometryType())
    {
      case wkbPoint:
	poTABFeature = new TABPoint(poFeature->GetDefnRef());
	break;
      case wkbPolygon:
	poTABFeature = new TABRegion(poFeature->GetDefnRef());
	break;
      case wkbLineString:
      case wkbMultiPoint:
      case wkbMultiLineString:
      case wkbMultiPolygon:
	poTABFeature = new TABPolyline(poFeature->GetDefnRef());
	break;
      case wkbGeometryCollection:
      case wkbUnknown:
      default:
         poTABFeature = new TABFeature(poFeature->GetDefnRef()); 
        break;
    }

    poTABFeature->SetGeometryDirectly(poGeom->clone());
    
    for (int i=0; i< poFeature->GetDefnRef()->GetFieldCount();i++)
    {
	poTABFeature->SetField(i,poFeature->GetRawFieldRef( i ));
    }
    

    if (SetFeature(poTABFeature) == 0)
      return OGRERR_NONE;
    else
      return OGRERR_FAILURE;
}

OGRFeature *IMapInfoFile::GetFeature(long nFeatureId)
{
    OGRFeature *poFeature, *poFeatureRef;

    
    poFeatureRef = GetFeatureRef(nFeatureId);
    if (poFeatureRef)
    {
	poFeature = poFeatureRef->Clone();
	poFeature->SetFID(poFeatureRef->GetFID());
	
	return poFeature;
    }
    else
      return NULL;
}

OGRGeometry *IMapInfoFile::GetSpatialFilter()
{
    return m_poFilterGeom;
}

void IMapInfoFile::SetSpatialFilter (OGRGeometry * poGeomIn )

{
    if( m_poFilterGeom != NULL )
    {
        delete m_poFilterGeom;
        m_poFilterGeom = NULL;
    }

    if( poGeomIn != NULL )
        m_poFilterGeom = poGeomIn->clone();
}


