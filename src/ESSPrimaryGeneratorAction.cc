#include "ESSPrimaryGeneratorAction.hh"

#include <G4Event.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleTable.hh>
#include <G4PrimaryParticle.hh>
#include <G4PrimaryVertex.hh>
#include <G4RandomTools.hh>
#include <G4SystemOfUnits.hh>
#include <G4ios.hh>

#include <Randomize.hh>

ESSPrimaryGeneratorAction::ESSPrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(),
      fMessenger(0),
      fParticleGun(0),
      fRandom(false),
      frandEnegy(10.0 * MeV),
      fZEnegy(1.0),
      fXEnegy(0.0),
      fYEnegy(0.0) {
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

  // Define particle properties
  G4String particleName = "proton";
  G4ThreeVector position(0, 0, -0.300 * m);
  G4ThreeVector momentum(.0, .0, 1.);
  G4double time = 0;

  // Get particle definition from G4ParticleTable
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particleDefinition =
      particleTable->FindParticle(particleName);
  if (!particleDefinition) {
    G4cerr << "Error: " << particleName << " not found in G4ParticleTable"
           << G4endl;
    exit(1);
  }

  fParticleGun->SetParticleTime(time);
  fParticleGun->SetParticleDefinition(particleDefinition);
  fParticleGun->SetParticleEnergy(2 * GeV);
  fParticleGun->SetParticleMomentumDirection(momentum);
  fParticleGun->SetParticlePosition(position);

  // Generic messenger
  fMessenger = new G4GenericMessenger(this, "/ESS/primary/",
                                      "Primary generator control");

  fMessenger->DeclareProperty("setRandom", fRandom,
                              "Activate/Inactivate random option");
  fMessenger->DeclarePropertyWithUnit("setRandEnergy", "MeV", frandEnegy,
                                      "Mean energy for random deviation");
  fMessenger->DeclareProperty("setXDir", fXEnegy, "Particle energy X");
  fMessenger->DeclareProperty("setYDir", fYEnegy, "Particle energy Y");
  fMessenger->DeclareProperty("setZDir", fZEnegy, "Particle energy Z");
}

ESSPrimaryGeneratorAction::~ESSPrimaryGeneratorAction() {}

void ESSPrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
  if (fRandom) {
    auto base = fParticleGun->GetParticleMomentumDirection();
    base.setZ(1);
    base.setX(G4RandGauss::shoot(0, 0.5));
    base.setY(G4RandGauss::shoot(0, 0.5));
    fParticleGun->SetParticleMomentumDirection(base);
  } else {
    auto base = fParticleGun->GetParticleMomentumDirection();
    base.setZ(fZEnegy);
    base.setX(fXEnegy);
    base.setY(fYEnegy);
    fParticleGun->SetParticleMomentumDirection(base);
  }

  fParticleGun->GeneratePrimaryVertex(event);
}