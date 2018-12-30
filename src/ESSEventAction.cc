#include "ESSEventAction.hh"

#include <G4Event.hh>
#include <G4ios.hh>

ESSEventAction::ESSEventAction(bool verbose)
    : G4UserEventAction(), fVerbose(verbose) {}

ESSEventAction::~ESSEventAction() {}

void ESSEventAction::BeginOfEventAction(const G4Event *event) {
  if (fVerbose)
    G4cout << ">>> Start event: " << event->GetEventID() << G4endl;
}

void ESSEventAction::EndOfEventAction(const G4Event *event) {
  if (fVerbose)
    G4cout << ">>> End event: " << event->GetEventID() << G4endl;
}