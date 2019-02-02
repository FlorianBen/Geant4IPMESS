#include "ESSBeamStopHit.hh"

#include <G4UnitsTable.hh>

#include <iomanip>

G4ThreadLocal G4Allocator<ESSBeamStopHit> *ESSBeamStopHitAllocator = 0;

ESSBeamStopHit::ESSBeamStopHit()
    : G4VHit(), fEdep(0.) {}

ESSBeamStopHit::~ESSBeamStopHit() {}

ESSBeamStopHit::ESSBeamStopHit(const ESSBeamStopHit & /*right*/) : G4VHit() {}

const ESSBeamStopHit &ESSBeamStopHit::operator=(const ESSBeamStopHit & /*right*/) {
  return *this;
}

int ESSBeamStopHit::operator==(const ESSBeamStopHit & /*right*/) const { return 0; }

void ESSBeamStopHit::Print() {
  if (fEdep > 0.);
}