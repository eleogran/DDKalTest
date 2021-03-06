#ifndef DDPLANARSTRIPHIT_H
#define DDPLANARSTRIPHIT_H

/** DDPlanarStripHit: User defined KalTest hit class using u coordinate, which provides coordinate vector as defined by the MeasLayer 
 *  
 * @author S.Aplin DESY
 */

#include "KalTrackDim.h"

#include "DDVTrackHit.h"


#define DDPlanarStripHit_DIM 1 // set to 2 if one want to debug strip hits by using the 2nd dimention

class DDPlanarStripHit : public DDVTrackHit {
  
public:
  
  /** Constructor Taking a single coordinate and associated measurement layer, with bfield */
  DDPlanarStripHit(const TVMeasLayer &ms,
               Double_t       *x,
               Double_t       *dx,
               Double_t        bfield,
               EVENT::TrackerHit* trkhit) 
  : DDVTrackHit(ms, x, dx, bfield, DDPlanarStripHit_DIM,trkhit)
  { /* no op */ } 
  
  // TVTrackHit's pure virtuals that must be implemented
  
  /** Global to Local coordinates */
  virtual TKalMatrix XvToMv (const TVector3 &xv, Double_t t0) const;
  
  /** Print Debug information */
  virtual void       DebugPrint(Option_t *opt = "")           const;
  
  
private:
  
  
};
#endif
