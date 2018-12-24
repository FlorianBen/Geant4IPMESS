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

#include "CADMesh.hh"

ESSDetectorConstruction::ESSDetectorConstruction()
    : G4VUserDetectorConstruction() {}

ESSDetectorConstruction::~ESSDetectorConstruction() {}

G4VPhysicalVolume *ESSDetectorConstruction::Construct() {
  // Get nist material manager
  auto nistManager = G4NistManager::Instance();

  // Build materials from NIST DB
  auto air = nistManager->FindOrBuildMaterial("G4_AIR");
  auto steelPipe = nistManager->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  // Build materials for vacuum.
  // Vacuum condition
  G4double density = (1e0 / 1013.) * 27. * mg / cm3;
  G4double tempVacuum = 293. * kelvin;
  G4double presVacuum = 1.0e-12 * bar;

  G4double a, z;
  G4String symbol;
  G4int nAtoms;
  a = 1.01 * g / mole;
  auto elH = new G4Element("Hydrogen", symbol = "H", z = 1., a);
  a = 16.00 * g / mole;
  auto elO = new G4Element("Oxygen", symbol = "O", z = 8., a);
  a = 12.00 * g / mole;
  auto elC = new G4Element("Carbon", symbol = "C", z = 6., a);
  a = 14.00 * g / mole;
  auto elN = new G4Element("Nitrogen", symbol = "N", z = 7., a);

  auto dihydrogenGas = new G4Material("DiHydrogenGas", density, 1, kStateGas,
                                      tempVacuum, presVacuum);
  dihydrogenGas->AddElement(elH, nAtoms = 2);

  auto dicarbonicGas = new G4Material("DiCarbonicGas", density, 2, kStateGas,
                                      tempVacuum, presVacuum);
  dicarbonicGas->AddElement(elC, nAtoms = 1);
  dicarbonicGas->AddElement(elO, nAtoms = 2);

  auto carbonicGas = new G4Material("CarbonicGas", density, 2, kStateGas,
                                    tempVacuum, presVacuum);
  carbonicGas->AddElement(elC, nAtoms = 1);
  carbonicGas->AddElement(elO, nAtoms = 1);

  auto dinitrogenGas = new G4Material("DiNitrogenGas", density, 1, kStateGas,
                                      tempVacuum, presVacuum);
  dinitrogenGas->AddElement(elN, nAtoms = 2);

  auto vacuumGas = new G4Material("VacuumGas", density, 4, kStateGas,
                                  tempVacuum, presVacuum);
  vacuumGas->AddMaterial(dihydrogenGas, 79. * perCent);
  vacuumGas->AddMaterial(carbonicGas, 10. * perCent);
  vacuumGas->AddMaterial(carbonicGas, 10. * perCent);
  vacuumGas->AddMaterial(dicarbonicGas, 1. * perCent);

  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;

  // World Creation
  // World parameters
  G4double hx = 3 * m;
  G4double hy = 3. * m;
  G4double hz = 3 * m;
  // World volume
  auto worldS = new G4Box("World", hx, hy, hz);
  auto worldLV = new G4LogicalVolume(worldS, air, "World");
  G4VPhysicalVolume *worldPV =
      new G4PVPlacement(0,               // no rotation
                        G4ThreeVector(), // at (0,0,0)
                        worldLV,         // its logical volume
                        "World",         // its name
                        0,               // its mother  volume
                        false,           // no boolean operation
                        0,               // copy number
                        checkOverlaps);  // overlaps checking

  // LWU Creation
  G4double phiS = 0. * degree;
  G4double phiT = 360. * degree;
  G4double zplanes = 6;
  // Outer
  G4double rIOuter[]{0 * cm, 0 * cm, 0 * cm, 0 * cm, 0 * cm, 0. * cm};
  G4double rOuter[]{55. * mm,  55. * mm, 130. * mm,
                    130. * mm, 55. * mm, 55. * mm};
  G4double zplaneOuter[]{-1000. * mm, -2 * mm,  0. * cm,
                         457 * mm,    457 * mm, 1457 * mm};
  auto pipeOuterBeamPoly = new G4Polycone("PolyChamber", phiS, phiT, zplanes,
                                          zplaneOuter, rIOuter, rOuter);
  auto wireScannerOuterBox =
      new G4Box("boxWSOuter", 155. * mm, 80. * mm, 29. * mm);
  auto cf200OuterTub =
      new G4Tubs("cf200Outer", .0, 105 * mm, 10 * cm, phiS, phiT);
  auto rotWS = new G4RotationMatrix();
  rotWS->rotateZ(90. * deg);
  auto pipeBeamSolid1 = new G4UnionSolid(
      "unionWS1Outer", pipeOuterBeamPoly, wireScannerOuterBox,
      new G4RotationMatrix(), G4ThreeVector(-100. * mm, 0., 65 * mm));
  pipeBeamSolid1 =
      new G4UnionSolid("unionWS2Outer", pipeBeamSolid1, wireScannerOuterBox,
                       rotWS, G4ThreeVector(0. * mm, 100. * mm, 65 * mm));
  auto rot1 = new G4RotationMatrix();
  rot1->rotateY(90. * deg);
  pipeBeamSolid1 =
      new G4UnionSolid("unionF1Outer", pipeBeamSolid1, cf200OuterTub, rot1,
                       G4ThreeVector(100. * mm, 0. * mm, 206 * mm));
  rot1->rotateX(90. * deg);
  pipeBeamSolid1 =
      new G4UnionSolid("unionF2Outer", pipeBeamSolid1, cf200OuterTub, rot1,
                       G4ThreeVector(0. * mm, 100. * mm, 337 * mm));
  auto pipeBeamL =
      new G4LogicalVolume(pipeBeamSolid1, steelPipe, "PolyChamberOuterL");

  new G4PVPlacement(0, G4ThreeVector(0, 0, 0 * mm), pipeBeamL, "PipeOuter",
                    worldLV, false, 0, checkOverlaps);
  rot1->rotateX(-90. * deg);

  // Inner
  G4double rIInner[]{0 * cm, 0 * cm, 0 * cm, 0 * cm, 0 * cm, 0. * cm};
  G4double rInner[]{50 * mm, 50 * mm, 125 * mm, 125 * mm, 50 * mm, 50 * mm};
  G4double zplaneInner[]{-950. * mm, 0 * mm,   0. * cm,
                         455 * mm,   455 * mm, 1455 * mm};
  auto pipeInnerBeamPoly = new G4Polycone("PolyVacuum", phiS, phiT, zplanes,
                                          zplaneInner, rIInner, rInner);
  auto wireScannerInnerBox =
      new G4Box("boxSInner", 150. * mm, 75. * mm, 24. * mm);
  auto cf200InnerTub =
      new G4Tubs("cf200Inner", .0, 100 * mm, 10 * cm, phiS, phiT);

  auto pipeBeamSolidInner = new G4UnionSolid(
      "unionWS1Inner", pipeInnerBeamPoly, wireScannerInnerBox,
      new G4RotationMatrix(), G4ThreeVector(-100. * mm, 0., 65 * mm));
  pipeBeamSolidInner =
      new G4UnionSolid("unionWS2Inner", pipeBeamSolidInner, wireScannerInnerBox,
                       rotWS, G4ThreeVector(0. * mm, 100. * mm, 65 * mm));
  pipeBeamSolidInner =
      new G4UnionSolid("unionF1Inner", pipeBeamSolidInner, cf200InnerTub, rot1,
                       G4ThreeVector(100. * mm, 0. * mm, 206 * mm));
  rot1->rotateX(90. * deg);
  pipeBeamSolidInner =
      new G4UnionSolid("unionF2Inner", pipeBeamSolidInner, cf200InnerTub, rot1,
                       G4ThreeVector(0. * mm, 100. * mm, 337 * mm));
  auto pipeVacuumL =
      new G4LogicalVolume(pipeBeamSolidInner, vacuumGas, "PolyChamberInnerL");

  new G4PVPlacement(0, G4ThreeVector(0, 0, 0 * mm), pipeVacuumL, "PipeInner",
                    pipeBeamL, false, 0, checkOverlaps);

  // Disks
  auto diskPoly =
      new G4Tubs("PolyDisk", 50.0 * mm, 123.0 * mm, 0.5 * mm, phiS, phiT);
  auto diskL = new G4LogicalVolume(diskPoly, steelPipe, "DiskL");

  new G4PVPlacement(0, G4ThreeVector(.0, .0, 122 * mm), diskL, "Disk1",
                    pipeVacuumL, false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(.0, .0, 289 * mm), diskL, "Disk2",
                    pipeVacuumL, false, 1, checkOverlaps);


  // IPM1
  auto offset = G4ThreeVector(-1.12595 * mm, -958.165 * mm, -261.819 * mm);
  CADMesh *meshIPM = new CADMesh("IPM/out_cadreV2.stl", mm, offset, false);
  auto IPM1Cadre1 = meshIPM->TessellatedMesh();
  // TODO: Change to MACOR
  auto IPML = new G4LogicalVolume(IPM1Cadre1, steelPipe, "IPML");
  auto rotIPM1 = new G4RotationMatrix();
  rotIPM1->rotateY(-90. * deg);

  new G4PVPlacement(rotIPM1, G4ThreeVector(.0 * mm, 60.0 * mm, 206 * mm), IPML, "IPM1",
                    pipeVacuumL, false, 0, checkOverlaps);

  rotIPM1 = new G4RotationMatrix();
  rotIPM1->rotateY(-90. * deg);
  rotIPM1->rotateZ(180. * deg);
  new G4PVPlacement(rotIPM1, G4ThreeVector(.0 * mm, -60.0 * mm, 206 * mm), IPML, "IPM2",
                    pipeVacuumL, false, 1, checkOverlaps);

  // IPM2
  auto rotIPM2 = new G4RotationMatrix();
  rotIPM2->rotateY(-90. * deg);

  new G4PVPlacement(rotIPM2, G4ThreeVector(.0 * mm, 60.0 * mm, 372 * mm), IPML, "IPM1",
                    pipeVacuumL, false, 0, checkOverlaps);

  rotIPM2 = new G4RotationMatrix();
  rotIPM2->rotateY(-90. * deg);
  rotIPM2->rotateZ(180. * deg);
  new G4PVPlacement(rotIPM2, G4ThreeVector(.0 * mm, -60.0 * mm, 372 * mm), IPML, "IPM2",
                    pipeVacuumL, false, 1, checkOverlaps);

  // MCP

  // Camera
  auto offsetcam = G4ThreeVector(-0 * cm, 0, 0);
  // TODO: Add lens
  CADMesh *meshcam =
      new CADMesh("Camera/out_FL2-016-R0.stl", mm, offsetcam, false);
  G4VSolid *cad_solid = meshcam->TessellatedMesh();
  auto rotCam1 = new G4RotationMatrix();
  rotCam1->rotateY(90. * deg);
  auto camL = new G4LogicalVolume(cad_solid, vacuumGas, "CameraL");
  new G4PVPlacement(rotCam1, G4ThreeVector(290. * mm, .0 * mm, 206 * mm), camL,
                    "Cam1", pipeVacuumL, false, 0, checkOverlaps);
  auto rotCam2 = new G4RotationMatrix();
  rotCam2->rotateX(-90. * deg);
  new G4PVPlacement(rotCam2, G4ThreeVector(.0 * mm, 290. * mm, 372 * mm), camL,
                    "Cam2", pipeVacuumL, false, 1, checkOverlaps);

  // WS

  // Dipole

  // LWU Support

  // always return the physical World
  return worldPV;
}

void ESSDetectorConstruction::ConstructSDandField() {}

void ESSDetectorConstruction::ConstructMaterials() {}
