#include "ESSAnalysis.hh"
#include "ESSRunAction.hh"


#include <G4Run.hh>
#include <G4SystemOfUnits.hh>

ESSRunAction::ESSRunAction() : G4UserRunAction() {
  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in B4Analysis.hh
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  G4cout << "Using " << analysisManager->GetType() << " analysis manager"
         << G4endl;

  // Creating histograms
  for (G4int i = 0; i < 8; ++i) {
    std::ostringstream os;
    os << i;
    G4String hname = "Camera";
    hname += os.str();
    G4String htitle = "Edep [MeV] in Camera";
    htitle += os.str();
    analysisManager->CreateH1(hname, htitle, 200, 0., 3);
  }
}

ESSRunAction::~ESSRunAction() { delete G4AnalysisManager::Instance(); }

void ESSRunAction::BeginOfRunAction(const G4Run * /*run*/) {
  // Open an output file
  G4String fileName = "camera";
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile(fileName);
}

void ESSRunAction::EndOfRunAction(const G4Run * /*run*/) {
  // save histograms
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}