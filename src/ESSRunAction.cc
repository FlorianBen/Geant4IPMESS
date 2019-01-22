#include "ESSRunAction.hh"
#include "ESSAnalysis.hh"
#include "ESSConstants.hh"

#ifdef G4MPI
#include <G4MPImanager.hh>
#endif

#include <G4Run.hh>
#include <G4SystemOfUnits.hh>

ESSRunAction::ESSRunAction() : G4UserRunAction() {
  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in B4Analysis.hh
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetNtupleMerging(true);
  analysisManager->SetVerboseLevel(1);
  G4cout << "Using " << analysisManager->GetType() << " analysis manager"
         << G4endl;

  // Creating histograms
  for (G4int i = 0; i < 2; ++i) {
    std::ostringstream os;
    os << i;
    G4String hname = "Camera";
    hname += os.str();
    G4String htitle = "Edep [MeV] in Camera";
    htitle += os.str();
    analysisManager->CreateH2(hname, htitle, kNofSensorColumns, 0,
                              kNofSensorColumns - 1, kNofSensorRows, 0,
                              kNofSensorRows -
                                  1); //, "x (pixels)", "y (pixels)");
  }

  analysisManager->CreateNtuple("MCP", "MCP hits");
  analysisManager->CreateNtupleIColumn("ID");      // column id = 0
  analysisManager->CreateNtupleIColumn("PDG");     // column id = 1
  analysisManager->CreateNtupleDColumn("Ekin");    // column id = 2
  analysisManager->CreateNtupleDColumn("Xpos");    // column id = 3 
  analysisManager->CreateNtupleDColumn("Ypos");    // column id = 4
  analysisManager->CreateNtupleDColumn("Zpos");    // column id = 5
  analysisManager->CreateNtupleDColumn("Xmom");    // column id = 6
  analysisManager->CreateNtupleDColumn("Ymom");    // column id = 7
  analysisManager->CreateNtupleDColumn("Zmom");    // column id = 8
  analysisManager->CreateNtupleDColumn("time");    // column id = 9
  analysisManager->FinishNtuple();
}

ESSRunAction::~ESSRunAction() { delete G4AnalysisManager::Instance(); }

void ESSRunAction::BeginOfRunAction(const G4Run * /*run*/) {
  // Open an output file
  G4String fileName = "camera";
#ifdef G4MPI
  fileName = fileName + "_rank" +
             std::to_string(G4MPImanager::GetManager()->GetRank());
#endif
  fileName = fileName + "_hist";
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile(fileName);
}

void ESSRunAction::EndOfRunAction(const G4Run * /*run*/) {
  // save histograms
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}