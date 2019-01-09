#include "ESSSensorParameterisation.hh"
#include "ESSConstants.hh"

#include <G4SystemOfUnits.hh>
#include <G4ThreeVector.hh>
#include <G4VPhysicalVolume.hh>

// TODO: Implement

ESSSensorParameterisation::ESSSensorParameterisation()
    : G4VPVParameterisation() {
  for (auto copyNo = 0; copyNo < kNofPixels; copyNo++) {
    auto column = copyNo / kNofSensorRows;
    auto row = copyNo % kNofSensorRows;
    fXPixels[copyNo] = (column - 9) * 15. * cm - 7.5 * cm;
    fYPixels[copyNo] = (row - 1) * 15 * cm - 7.5 * cm;
  }
}

ESSSensorParameterisation::~ESSSensorParameterisation() {}

void ESSSensorParameterisation::ComputeTransformation(
    const G4int copyNo, G4VPhysicalVolume *physVol) const {
  physVol->SetTranslation(
      G4ThreeVector(fXPixels[copyNo], fYPixels[copyNo], 0.));
}