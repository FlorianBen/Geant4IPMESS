#include "ESSPrimaryGeneratorAction.hh"

#include <G4Event.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleTable.hh>
#include <G4PrimaryParticle.hh>
#include <G4PrimaryVertex.hh>
#include <G4SystemOfUnits.hh>
#include <G4ios.hh>

ESSPrimaryGeneratorAction::ESSPrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction() {}

ESSPrimaryGeneratorAction::~ESSPrimaryGeneratorAction() {}

void ESSPrimaryGeneratorAction::GeneratePrimaries(G4Event *event) {
  // this function is called at the begining of ecah event

  // Define particle properties
  G4String particleName = "proton";
  G4ThreeVector position(0, 0, 0.372 * m);
  G4ThreeVector momentum(0. * GeV, 2. * GeV, 0 * GeV);
  G4double time = 0;

  // Get particle definition from G4ParticleTable
  G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition *particleDefinition =
      particleTable->FindParticle(particleName);
  if (!particleDefinition) {
    G4cerr << "Error: " << particleName << " not found in G4ParticleTable"
           << G4endl;
    exit(1);
  }

  // Create primary particle
  G4PrimaryParticle *primaryParticle =
      new G4PrimaryParticle(particleDefinition);
  primaryParticle->SetMomentum(momentum.x(), momentum.y(), momentum.z());
  primaryParticle->SetMass(particleDefinition->GetPDGMass());
  primaryParticle->SetCharge(particleDefinition->GetPDGCharge());

  // Create vertex
  G4PrimaryVertex *vertex = new G4PrimaryVertex(position, time);
  vertex->SetPrimary(primaryParticle);
  event->AddPrimaryVertex(vertex);
}