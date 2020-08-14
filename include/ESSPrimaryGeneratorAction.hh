#ifndef ESSPrimaryGeneratorAction_h
#define ESSPrimaryGeneratorAction_h 1

#include <G4GenericMessenger.hh>
#include <G4ParticleGun.hh>
#include <G4VUserPrimaryGeneratorAction.hh>

class G4Event;

class ESSPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
 public:
  ESSPrimaryGeneratorAction();
  virtual ~ESSPrimaryGeneratorAction();

  // method from the base class
  virtual void GeneratePrimaries(G4Event*);

 private:
  G4GenericMessenger* fMessenger;
  G4ParticleGun* fParticleGun;
  G4bool fRandom;
  G4double frandEnegy;
  G4double fXEnegy;
  G4double fYEnegy;
  G4double fZEnegy;
};

#endif
