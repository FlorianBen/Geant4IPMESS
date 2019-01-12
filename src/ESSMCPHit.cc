#include "ESSMCPHit.hh"

#include <G4UnitsTable.hh>

#include <iomanip>

G4ThreadLocal G4Allocator<ESSMCPHit>* ESSMCPHitAllocator = 0;

ESSMCPHit::ESSMCPHit()
 : G4VHit(),
   fCamNumber(-1),
   fEdep(0.)
{}

ESSMCPHit::~ESSMCPHit()
{}

ESSMCPHit::ESSMCPHit(const ESSMCPHit& /*right*/)
 : G4VHit() 
{}

const ESSMCPHit& 
ESSMCPHit::operator=(const ESSMCPHit& /*right*/)
{
  return *this;
}

int ESSMCPHit::operator==(const ESSMCPHit& /*right*/) const
{
  return 0;
}

void ESSMCPHit::Print()
{
  if ( fEdep > 0. ) {
    G4cout << "Camera hit in layer: " << fCamNumber 
           << "  Edep: " << std::setw(7) << G4BestUnit(fEdep,"Energy") << G4endl;
  }          
}