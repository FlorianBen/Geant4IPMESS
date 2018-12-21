#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4GDMLParser.hh"
#include "G4GenericBiasingPhysics.hh"
#include "G4PhysListFactory.hh"
#include "G4UImanager.hh"
#include <FTFP_BERT.hh>

#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

#include "ESSActionInitialization.hh"
#include "ESSDetectorConstruction.hh"

int main(int argc, char *argv[]) {
  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive *ui = 0;
  if (argc == 1) {
    ui = new G4UIExecutive(argc, argv);
  }

  // Construct the run manager
  //
#ifdef G4MULTITHREADED
  G4MTRunManager *runManager = new G4MTRunManager;
  runManager->SetNumberOfThreads(G4Threading::G4GetNumberOfCores());
#else
  G4RunManager *runManager = new G4RunManager;
#endif

  runManager->SetUserInitialization(new ESSDetectorConstruction());

  // Physics list
  G4VModularPhysicsList *physicsList = new FTFP_BERT;
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);

  // User action initialization
  runManager->SetUserInitialization(new ESSActionInitialization());

  // Initialize visualization
  G4VisManager *visManager = new G4VisExecutive;
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager *UImanager = G4UImanager::GetUIpointer();

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
  return 0;

  return 0;
}
