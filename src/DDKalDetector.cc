
#include "DDKalTest/DDKalDetector.h"
#include "DDKalTest/DDCylinderMeasLayer.h"
#include "DDKalTest/DDConeMeasLayer.h"
#include "DDKalTest/DDParallelPlanarMeasLayer.h"
#include "DDKalTest/DDDiscMeasLayer.h"
#include <UTIL/LCTrackerConf.h>

//#include "DDKalTest/DDParallelPlanarStripMeasLayer.h"

#include "DD4hep/LCDD.h"
#include "DD4hep/DD4hepUnits.h"
#include "DDRec/SurfaceManager.h"
#include "DDRec/SurfaceHelper.h"

#include <lcio.h>
#include "Exceptions.h"

#include "streamlog/streamlog.h"


DDKalDetector::DDKalDetector( DD4hep::Geometry::DetElement det ){
  
  
  streamlog_out( DEBUG4 ) << " DDKalDetector::DDKalDetector() initialize surfaces for detector " 
			  << det.name() << " type: " << det.type() << std::endl ;
  
  // --- get B field :
  double origin[3] = { 0., 0., 0. } , bfield[3] ;

  DD4hep::Geometry::LCDD& lcdd = DD4hep::Geometry::LCDD::getInstance();

 DD4hep::Geometry::OverlayedField ovField = lcdd.field() ;

  ovField.magneticField( origin , bfield ) ;

  double Bz = bfield[2] / dd4hep::tesla ;

  streamlog_out( DEBUG4 ) << " - use Bz = " << Bz << " Tesla " << std::endl ;
  //-------------


#define use_surface_helper 1

#if use_surface_helper

  DD4hep::DDRec::SurfaceHelper ds( det ) ;
  const DD4hep::DDRec::SurfaceList& detSL = ds.surfaceList() ;
  for( DD4hep::DDRec::SurfaceList::const_iterator it = detSL.begin() ; it != detSL.end() ; ++it ){
    
#else
  //===========  get the surface map from the SurfaceManager ================

  DD4hep::DDRec::SurfaceManager& surfMan = *lcdd.extension<DD4hep::DDRec::SurfaceManager>() ;

  typedef DD4hep::DDRec::SurfaceMap SMap ;

  const SMap* sMap = surfMan.map( det.name() ) ;

  if( ! sMap ) {   
    std::stringstream err  ; err << " DDKalDetector::DDKalDetector() - "
				 << " Could not find surface map for detector: " 
                                 <<   det.name() << " in SurfaceManager " ;
    throw lcio::Exception( err.str() ) ;
  }
  
  for( SMap::const_iterator it = sMap->begin() ; it != sMap->end() ; ++it){
    
    DDSurfaces::ISurface* surf =  it->second ;

#endif    

    DDSurfaces::ISurface* surf =  *it ;

    streamlog_out( DEBUG5 ) << "DDKalDetector:  install surface for : " << UTIL::LCTrackerCellID::valueString( surf->id() ) << std::endl ;


    streamlog_out( DEBUG ) << " ------------------------- "
			   << "  surface: "  << *surf         << std::endl
			   << " ------------------------- "  << std::endl ;

    if( surf->type().isCylinder() ) {

      Add( new DDCylinderMeasLayer( surf , Bz ) ) ;
    }

    else if( surf->type().isCone() ) {

      Add( new DDConeMeasLayer( surf , Bz ) ) ;
    }
    
    else if( surf->type().isPlane() ){ 

      if( surf->type().isParallelToZ() ) {
      
 	Add( new DDParallelPlanarMeasLayer( surf , Bz ) ) ;

      } else if(  surf->type().isOrthogonalToZ() ){
	
	Add( new DDDiscMeasLayer( surf , Bz ) ) ;

      } else{ 

 	Add( new DDPlanarMeasLayer( surf , Bz ) ) ;
      }

    }



  }


}
