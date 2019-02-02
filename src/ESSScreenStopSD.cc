#include "ESSConstants.hh"
#include "ESSScreenStopSD.hh"
#include "ESSAnalysis.hh"

#include <G4HCofThisEvent.hh>
#include <G4SDManager.hh>
#include <G4Step.hh>
#include <G4SystemOfUnits.hh>
#include <G4VTouchable.hh>
#include <G4ios.hh>

ESSScreenStopSD::ESSScreenStopSD(const G4String &name,
                         const G4String &hitsCollectionName, const G4int idTuple)
    : G4VSensitiveDetector(name), fHitsCollection(0), fidTuple(idTuple) {
  collectionName.insert(hitsCollectionName);
}

ESSScreenStopSD::~ESSScreenStopSD() {}

void ESSScreenStopSD::Initialize(G4HCofThisEvent *hce) {
  fHitsCollection =
      new ESSMCPHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in hce
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection);
  for (auto i = 0; i < 2; i++) {
    auto hit = new ESSMCPHit();
    hit->SetMCPNumber(i);
    fHitsCollection->insert(hit);
  }
}

G4bool ESSScreenStopSD::ProcessHits(G4Step *step, G4TouchableHistory *) {
  // TODO: Implementer l'enregistrement des données pour les ecrans
  
  return true;
}
