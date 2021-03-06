/* #ifdef G4MULTITHREADED
#include <G4MTRunManager.hh>
#else
#include <G4RunManager.hh>
#endif */

#include "G4MTRunManager.hh"

#ifdef G4MPI
#include <G4MPImanager.hh>
#include <G4MPIsession.hh>
#endif

#include <FTFP_BERT.hh>
#include <G4GDMLParser.hh>
#include <G4GenericBiasingPhysics.hh>
#include <G4PhysListFactory.hh>
#include <G4UImanager.hh>

#include <G4UIExecutive.hh>
#include <G4VisExecutive.hh>

#include <G4GDMLParser.hh>
#include <G4LogicalVolume.hh>
#include <G4TransportationManager.hh>

#include "ESSActionInitialization.hh"
#include "ESSDetectorConstruction.hh"

int main(int argc, char* argv[]) {
#ifdef G4MPI
  G4MPImanager* g4MPI = new G4MPImanager(argc, argv);
  G4MPIsession* session = g4MPI->GetMPIsession();

  // LAM/MPI users can use G4tcsh.
  G4String prompt = "[";
  prompt += "\033[31mG4MPI\033[0m";
  prompt += "](\e[104m%s\e[49m)[%/]:";
  session->SetPrompt(prompt);
#else
  // Detect interactive mode (if no arguments) and define UI session
  G4UIExecutive* ui = 0;
  if (argc == 1) {
    ui = new G4UIExecutive(argc, argv);
  }
#endif

  // Construct the run manager
  G4MTRunManager* runManager = new G4MTRunManager;
  runManager->SetNumberOfThreads(G4Threading::G4GetNumberOfCores());


  runManager->SetUserInitialization(new ESSDetectorConstruction());

  // Physics list
  auto physicListFactory = new G4PhysListFactory();
  auto physicsList =
      physicListFactory->GetReferencePhysList("QGSP_BERT_LIV");
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);

  // User action initialization
  runManager->SetUserInitialization(new ESSActionInitialization());

  runManager->Initialize();

  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

#ifdef G4MPI
  session->SessionStart();
  delete visManager;
  delete g4MPI;
  delete runManager;
#else
  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  if (!ui) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
  } else {
    // interactive mode : define UI session
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    if (ui->IsGUI()) {
      UImanager->ApplyCommand("/control/execute icons.mac");
    }
    ui->SessionStart();
    delete ui;
  }
  delete visManager;
  delete runManager;
#endif

  return EXIT_SUCCESS;
}
