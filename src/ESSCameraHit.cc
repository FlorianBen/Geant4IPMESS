#include "ESSCameraHit.hh"

#include <G4UnitsTable.hh>

#include <iomanip>

G4ThreadLocal G4Allocator<ESSCameraHit>* ESSCameraHitAllocator = 0;

ESSCameraHit::ESSCameraHit()
 : G4VHit(),
   fCamNumber(-1),
   fColumnNumber(-1),
   fRowNumber(-1),
   fEdep(0.)
{}

ESSCameraHit::~ESSCameraHit()
{}

ESSCameraHit::ESSCameraHit(const ESSCameraHit& /*right*/)
 : G4VHit() 
{}

const ESSCameraHit& 
ESSCameraHit::operator=(const ESSCameraHit& /*right*/)
{
  return *this;
}

int ESSCameraHit::operator==(const ESSCameraHit& /*right*/) const
{
  return 0;
}

void ESSCameraHit::Print()
{
  if ( fEdep > 0. ) {
    G4cout << "Camera hit in camera: " << fCamNumber 
           << "  Edep: " << std::setw(7) << G4BestUnit(fEdep,"Energy") << G4endl;
  }          
}