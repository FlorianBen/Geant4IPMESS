#include "ESSConstants.hh"
#include "ESSBeamStopSD.hh"
#include "ESSAnalysis.hh"

#include <G4HCofThisEvent.hh>
#include <G4SDManager.hh>
#include <G4Step.hh>
#include <G4SystemOfUnits.hh>
#include <G4VTouchable.hh>
#include <G4ios.hh>

ESSBeamStopSD::ESSBeamStopSD(const G4String &name,
                         const G4String &hitsCollectionName, const G4int idTuple)
    : G4VSensitiveDetector(name), fHitsCollection(0), fidTuple(idTuple) {
  collectionName.insert(hitsCollectionName);
}

ESSBeamStopSD::~ESSBeamStopSD() {}

void ESSBeamStopSD::Initialize(G4HCofThisEvent *hce) {
  fHitsCollection =
      new ESSBeamStopHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in hce
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection);
  for (auto i = 0; i < 1; i++) {
    auto hit = new ESSBeamStopHit();
    fHitsCollection->insert(hit);
  }
}

G4bool ESSBeamStopSD::ProcessHits(G4Step *step, G4TouchableHistory *) {
  auto trackID = step->GetTrack()->GetTrackID();
  if(trackID == 1) {
    step->GetTrack()->SetTrackStatus(fStopAndKill);

    return true;
  }
  return false;
}
