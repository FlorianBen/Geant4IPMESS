#include "ESSActionInitialization.hh"
#include "ESSEventAction.hh"
#include "ESSPrimaryGeneratorAction.hh"
#include "ESSRunAction.hh"

ESSActionInitialization::ESSActionInitialization()
    : G4VUserActionInitialization() {}

ESSActionInitialization::~ESSActionInitialization() {}

void ESSActionInitialization::Build() const {
  SetUserAction(new ESSPrimaryGeneratorAction);
  SetUserAction(new ESSEventAction(false));
  SetUserAction(new ESSRunAction);
}

void ESSActionInitialization::BuildForMaster() const {
  SetUserAction(new ESSRunAction);
}