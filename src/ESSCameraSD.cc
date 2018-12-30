#include "ESSCameraSD.hh"

#include <G4HCofThisEvent.hh>
#include <G4SDManager.hh>
#include <G4VTouchable.hh>
#include <G4Step.hh>
#include <G4ios.hh>

ESSCameraSD::ESSCameraSD(const G4String &name,
                         const G4String &hitsCollectionName)
    : G4VSensitiveDetector(name),
      fHitsCollection(0)
{
  collectionName.insert(hitsCollectionName);
}

ESSCameraSD::~ESSCameraSD()
{
}

void ESSCameraSD::Initialize(G4HCofThisEvent *hce)
{
  fHitsCollection = new ESSCameraHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in hce
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool ESSCameraSD::ProcessHits(G4Step *step,
                                G4TouchableHistory *history)
{
  // energy deposit
  G4double edep = step->GetTotalEnergyDeposit();
  if (edep == 0.)
    return false;
  auto hit = new ESSCameraHit();

  const G4VTouchable *touchable = step->GetPreStepPoint()->GetTouchable();
  G4VPhysicalVolume *layerPV = touchable->GetVolume();
  G4int layerNumber = layerPV->GetCopyNo();

  // Add values
  hit->AddEdep(edep);
  hit->SetCamNumber(layerNumber);

  fHitsCollection->insert(hit);

  return true;
}

void ESSCameraSD::EndOfEvent(G4HCofThisEvent * /*hce*/)
{
  if (fHitsCollection->entries() > 0)
  {
    G4cout << "\n-------->" << fHitsCollection->GetName()
           << ": in this event: " << G4endl;

    G4int nofHits = fHitsCollection->entries();
    for (G4int i = 0; i < nofHits; i++)
      (*fHitsCollection)[i]->Print();
  }
}
