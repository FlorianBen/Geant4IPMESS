#include "ESSConstants.hh"
#include "ESSMCPSD.hh"
#include "ESSAnalysis.hh"

#include <G4HCofThisEvent.hh>
#include <G4SDManager.hh>
#include <G4Step.hh>
#include <G4SystemOfUnits.hh>
#include <G4VTouchable.hh>
#include <G4ios.hh>

ESSMCPSD::ESSMCPSD(const G4String &name,
                         const G4String &hitsCollectionName)
    : G4VSensitiveDetector(name), fHitsCollection(0) {
  collectionName.insert(hitsCollectionName);
}

ESSMCPSD::~ESSMCPSD() {}

void ESSMCPSD::Initialize(G4HCofThisEvent *hce) {
  fHitsCollection =
      new ESSMCPHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in hce
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection);
  for (auto i = 0; i < kNofPixels; i++) {
    auto hit = new ESSMCPHit();
    hit->SetCamNumber(i);
    fHitsCollection->insert(hit);
  }
}

G4bool ESSMCPSD::ProcessHits(G4Step *step, G4TouchableHistory *) {
  // energy deposit
  G4double edep = step->GetTotalEnergyDeposit();
  if (edep == 0.)
    return false;

  const G4VTouchable *touchable = step->GetPreStepPoint()->GetTouchable();
  G4VPhysicalVolume *layerPV = touchable->GetVolume();

  G4int cameraNumber = touchable->GetCopyNumber(2);
  G4int columnNumber = touchable->GetCopyNumber(1);
  G4int pixelNumber = layerPV->GetCopyNo();

  auto hit = (*fHitsCollection)[pixelNumber];

  // Add values
  hit->AddEdep(edep);
  hit->SetCamNumber(pixelNumber);

  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  //analysisManager->FillH1(hit->GetCamNumber(), edep / MeV);
  analysisManager->FillH2(cameraNumber,columnNumber,pixelNumber,edep / MeV);
  return true;
}
