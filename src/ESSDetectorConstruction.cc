//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id$
//
/// \file EDDetectorConstruction.cc
/// \brief Implementation of the EDDetectorConstruction class

#include "ESSDetectorConstruction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4Polycone.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSDetectorConstruction::ESSDetectorConstruction()
    : G4VUserDetectorConstruction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSDetectorConstruction::~ESSDetectorConstruction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *ESSDetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager *nistManager = G4NistManager::Instance();

  // Build materials
  G4Material *air = nistManager->FindOrBuildMaterial("G4_AIR");
  auto steelPipe = nistManager->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  // There is no need to test if materials were built/found
  // as G4NistManager would issue an error otherwise
  // Try the code with "XYZ".

  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double hx = 3 * m;
  G4double hy = 3. * m;
  G4double hz = 3 * m;

  // world volume
  G4Box *worldS = new G4Box("World", hx, hy, hz);

  G4LogicalVolume *worldLV = new G4LogicalVolume(worldS, air, "World");

  G4VPhysicalVolume *worldPV =
      new G4PVPlacement(0,               // no rotation
                        G4ThreeVector(), // at (0,0,0)
                        worldLV,         // its logical volume
                        "World",         // its name
                        0,               // its mother  volume
                        false,           // no boolean operation
                        0,               // copy number
                        checkOverlaps);  // overlaps checking

  G4double phiS = 0. * degree;
  G4double phiT = 360. * degree;
  G4double zplanes = 6;
  G4double rInner[]{0 * cm, 0 * cm, 0 * cm, 0 * cm, 0 * cm, 0. * cm};
  G4double rOuter[]{62.5 * mm, 62.5 * mm, 125 * mm,
                    125 * mm, 62.5 * mm, 62.5 * mm};
  G4double zplane[]{-20. * cm, 0. * cm, 0. * cm, 455 * mm, 455 * mm, 665 * mm};
  auto pipeBeamPoly = new G4Polycone("PolyChamber", phiS, phiT, zplanes, zplane,
                                     rInner, rOuter);

  auto wireScannerBox = new G4Box("boxS", 10. * cm, 5. * cm, 2.5 * cm);
  auto rot2 = new G4RotationMatrix();
  rot2->rotateY(0. * deg);
  rot2->rotateX(0. * deg);
  G4ThreeVector tr2(10. * cm, 0., 6.1 * cm);

  auto pipeBeamSolid =
      new G4UnionSolid("unionS", pipeBeamPoly, wireScannerBox, rot2, tr2);

  auto pipeBeamL = new G4LogicalVolume(pipeBeamSolid, steelPipe, "PolyChamberL");

  new G4PVPlacement(0, G4ThreeVector(0, 0, 0 * mm), pipeBeamL, "PipeVacuum",
                    worldLV, false, 0, true);

  auto diskPoly =
      new G4Tubs("PolyDisk", 50.0 * mm, 123.0 * mm, 0.5 * mm, phiS, phiT);
  auto diskL = new G4LogicalVolume(diskPoly, steelPipe, "DiskL");

  new G4PVPlacement(0, G4ThreeVector(.0, .0, 122 * mm), diskL, "Disk1",
                    pipeBeamL, false, 0, true);
  new G4PVPlacement(0, G4ThreeVector(.0, .0, 289 * mm), diskL, "Disk2",
                    pipeBeamL, false, 0, true);

  // always return the physical World
  //
  return worldPV;
}

void ESSDetectorConstruction::ConstructSDandField() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
