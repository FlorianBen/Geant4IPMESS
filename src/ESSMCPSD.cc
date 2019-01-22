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
                         const G4String &hitsCollectionName, const G4int idTuple)
    : G4VSensitiveDetector(name), fHitsCollection(0), fidTuple(idTuple) {
  collectionName.insert(hitsCollectionName);
}

ESSMCPSD::~ESSMCPSD() {}

void ESSMCPSD::Initialize(G4HCofThisEvent *hce) {
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

G4bool ESSMCPSD::ProcessHits(G4Step *step, G4TouchableHistory *) {
  // TODO: Implementer l'enregistrement des information pour le premier hit (qqsoit l'energie ?)
  // TODO: exporter la position, direction, le type, l'energie de la particule.
  G4double edep = step->GetTotalEnergyDeposit();
  if (edep == 0.)
    return false;

  //const G4VTouchable *touchable = step->GetPreStepPoint()->GetTouchable();
  //G4VPhysicalVolume *layerPV = touchable->GetVolume();
  // Create new hit
  ESSMCPHit* newHit = new ESSMCPHit();
  
  // Layer number
  // = copy number of mother volume
  G4StepPoint* preStepPoint = step->GetPreStepPoint();
  const G4Track* track = step->GetTrack();
  G4int pdgCode = track->GetDefinition()->GetPDGEncoding();

  const G4VTouchable* touchable = preStepPoint->GetTouchable();
  G4VPhysicalVolume* motherPhysical = touchable->GetVolume(1); // mother
  G4int copyNo = motherPhysical->GetCopyNo();
  newHit->SetMCPNumber(copyNo);
  
  // Time
  G4double time = preStepPoint->GetGlobalTime();
  newHit->SetTime(time);
 
  // Position & momentum
  auto position = preStepPoint->GetPosition();
  auto momentu = preStepPoint->GetMomentum();
  newHit->SetPosition(position);
 
  // Add hit in the collection
  fHitsCollection->insert(newHit);

  // Add hits properties in the ntuple
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleIColumn(fidTuple, 0, copyNo);
  analysisManager->FillNtupleIColumn(fidTuple, 1, pdgCode);
  analysisManager->FillNtupleDColumn(fidTuple, 2, edep);
  analysisManager->FillNtupleDColumn(fidTuple, 3, position.x());
  analysisManager->FillNtupleDColumn(fidTuple, 4, position.y());
  analysisManager->FillNtupleDColumn(fidTuple, 5, position.z());
  analysisManager->FillNtupleDColumn(fidTuple, 6, time);
  analysisManager->AddNtupleRow(fidTuple);  
  //auto hit = (*fHitsCollection)[pixelNumber];

  return true;
}
