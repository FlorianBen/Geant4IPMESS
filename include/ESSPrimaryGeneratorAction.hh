#ifndef ESSPrimaryGeneratorAction_h
#define ESSPrimaryGeneratorAction_h 1

#include <G4VUserPrimaryGeneratorAction.hh>

class G4Event;

class ESSPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  ESSPrimaryGeneratorAction();
  virtual ~ESSPrimaryGeneratorAction();

  // method from the base class
  virtual void GeneratePrimaries(G4Event *);
};

#endif
