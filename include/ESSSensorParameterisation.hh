#ifndef ESSSensorParameterisation_H
#define ESSSensorParameterisation_H 1

#include "ESSConstants.hh"

#include <G4VPVParameterisation.hh>
#include <globals.hh>

#include <array>

class G4VPhysicalVolume;

class ESSSensorParameterisation : public G4VPVParameterisation {
public:
  ESSSensorParameterisation();
  virtual ~ESSSensorParameterisation();

  virtual void ComputeTransformation(const G4int copyNo,
                                     G4VPhysicalVolume *physVol) const;

private:
  std::array<G4double, kNofPixels> fXPixels;
  std::array<G4double, kNofPixels> fYPixels;
};

#endif