#include <ESSActionInitialization.hh>
#include <ESSEventAction.hh>
#include <ESSPrimaryGeneratorAction.hh>

ESSActionInitialization::ESSActionInitialization()
    : G4VUserActionInitialization() {}

ESSActionInitialization::~ESSActionInitialization() {}

void ESSActionInitialization::Build() const {
  SetUserAction(new ESSPrimaryGeneratorAction);
  SetUserAction(new ESSEventAction(false));
}

void ESSActionInitialization::BuildForMaster() const {
  //SetUserAction(new EDRunAction(false));
}