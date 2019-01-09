#include "ESSDetectorConstruction.hh"
#include "ESSCameraSD.hh"

#include <string>

#include <G4Box.hh>
#include <G4GDMLParser.hh>
#include <G4LogicalVolume.hh>
#include <G4MultiUnion.hh>
#include <G4NistManager.hh>
#include <G4PVPlacement.hh>
#include <G4PVReplica.hh>
#include <G4Polycone.hh>
#include <G4SDManager.hh>
#include <G4SubtractionSolid.hh>
#include <G4SystemOfUnits.hh>
#include <G4Transform3D.hh>
#include <G4Trd.hh>
#include <G4Tubs.hh>

#include <CADMesh.hh>

ESSDetectorConstruction::ESSDetectorConstruction()
    : G4VUserDetectorConstruction() {
  checkOverlaps = true;
  size_pixel = 120 * um;
  thickness_sensor = 100 * um;
  nx_pixels = 97;
  ny_pixels = 61;
}

ESSDetectorConstruction::~ESSDetectorConstruction() {
  delete mat_air;
  delete mat_copper;
  delete mat_gold;
  delete mat_macor;
  delete mat_steel;
  delete mat_vacuum;
}

G4VPhysicalVolume *ESSDetectorConstruction::Construct() {
  ConstructMaterials();
  // World Creation
  // World parameters
  G4double hx = 3 * m;
  G4double hy = 3. * m;
  G4double hz = 3 * m;
  // World volume
  auto worldS = new G4Box("World", hx, hy, hz);
  worldL = new G4LogicalVolume(worldS, mat_air, "World");
  G4VPhysicalVolume *worldPV =
      new G4PVPlacement(0,               // no rotation
                        G4ThreeVector(), // at (0,0,0)
                        worldL,          // its logical volume
                        "World",         // its name
                        0,               // its mother  volume
                        false,           // no boolean operation
                        0,               // copy number
                        checkOverlaps);  // overlaps checking

  // LWU
  auto solidOuter = ConstructSolidOuterLWU();
  auto solidInner = ConstructSolidInnerLWU();
  ConstructLWU(solidOuter, solidInner);

  // Disks
  auto solidDisk = ConstructSolidDisk();
  ConstructDisk(solidDisk);

  // IPM
  auto solidFrame = ConstructSolidFrame();
  auto solidPCB_BT = ConstructSolidPCB();
  auto solidPCB_HT = new G4Box("PCB", 1. * mm, 49. * mm, 49. * mm);
  ConstructIPM(solidFrame, solidPCB_BT, solidPCB_HT);

  // MCP
  auto solidMCP = ConstructSolidMCP();
  auto solidPhos = ConstructSolidPhos();
  auto solidFrameMCP = ConstructSolidFrameMCP();
  ConstructMCP(solidMCP, solidPhos, solidFrameMCP);

  // Vision System
  auto solidCam = ConstructSolidCamera();
  auto solidLens = ConstructSolidLens();
  auto solidSensor = ConstructSolidSensor();
  ConstructVisionS(solidCam, solidLens, solidSensor);

  // Quads
  auto solidQuad = ConstructSolidQuad();
  ConstructQuad(solidQuad);
  auto solidQuadSupport = ConstructSolidQuadSupport();
  ConstructQuadSupport(solidQuadSupport);

  // LWU Support
  auto solidSupportLWU = ConstructSolidLWUSupport();
  ConstructLWUSupport(solidSupportLWU);

  // G4GDMLParser parser;
  // parser.Write("out.gdml", worldPV);
  return worldPV;
}

void ESSDetectorConstruction::ConstructSDandField() {
  /*   auto sdManager = G4SDManager::GetSDMpointer();
    auto cameraSD = new ESSCameraSD("CamSD", "CameraCollection");
    sdManager->AddNewDetector(cameraSD);
    quadL->SetSensitiveDetector(cameraSD); */
}

void ESSDetectorConstruction::ConstructMaterials() {
  auto nistManager = G4NistManager::Instance();
  mat_air = nistManager->FindOrBuildMaterial("G4_AIR");
  mat_copper = nistManager->FindOrBuildMaterial("G4_Cu");
  mat_gold = nistManager->FindOrBuildMaterial("G4_Au");
  mat_macor = new G4Material("Macor", 2.52 * g / cm3, 6, kStateSolid);
  mat_mcp_glass = new G4Material("MCP_glass", 4. * g / cm3, 9, kStateSolid);
  mat_steel = nistManager->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  // Vacuum condition (ESS condition)
  // TODO: Check relation between density and pressure
  G4double density = (1e-12 / 1013.) * 27. * mg / cm3;
  G4double tempVacuum = 293. * kelvin;
  G4double presVacuum = 1.0e-12 * bar;

  G4int nAtoms;
  auto elH = nistManager->FindOrBuildElement(
      "H"); // new G4Element("Hydrogen", symbol = "H", z = 1., a);
  auto elO = nistManager->FindOrBuildElement(
      "O"); // new G4Element("Oxygen", symbol = "O", z = 8., a);
  auto elC = nistManager->FindOrBuildElement(
      "C"); // new G4Element("Carbon", symbol = "C", z = 6., a);
  auto elN = nistManager->FindOrBuildElement(
      "N"); // new G4Element("Nitrogen", symbol = "N", z = 7., a);

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

  mat_vacuum = new G4Material("VacuumGas", density, 4, kStateGas, tempVacuum,
                              presVacuum);
  mat_vacuum->AddMaterial(dihydrogenGas, 79. * perCent);
  mat_vacuum->AddMaterial(carbonicGas, 10. * perCent);
  mat_vacuum->AddMaterial(carbonicGas, 10. * perCent);
  mat_vacuum->AddMaterial(dicarbonicGas, 1. * perCent);

  // Macor (according to CORNING)
  auto siliconDioxyde = nistManager->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  auto magnesiumOxide = nistManager->FindOrBuildMaterial("G4_MAGNESIUM_OXIDE");
  auto aluminiumOxide = nistManager->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
  auto potassiumOxyde = nistManager->FindOrBuildMaterial("G4_POTASSIUM_OXIDE");
  auto boronTrioxyde = nistManager->FindOrBuildMaterial("G4_BORON_OXIDE");
  auto fluor = nistManager->FindOrBuildElement("F");

  mat_macor->AddMaterial(siliconDioxyde, 46 * perCent);
  mat_macor->AddMaterial(magnesiumOxide, 17 * perCent);
  mat_macor->AddMaterial(aluminiumOxide, 16 * perCent);
  mat_macor->AddMaterial(potassiumOxyde, 10 * perCent);
  mat_macor->AddMaterial(boronTrioxyde, 7 * perCent);
  mat_macor->AddElement(fluor, 4. * perCent);

  // Rogers PCB (RO4350 composition seems to be secret) -> kind of PTFE ?
  mat_pcb = nistManager->FindOrBuildMaterial("G4_TEFLON");

  // MCP glass (according to Photonis)
  auto elPb = nistManager->FindOrBuildElement("Pb");
  auto elSi = nistManager->FindOrBuildElement("Si");
  auto elK = nistManager->FindOrBuildElement("K");
  auto elRb = nistManager->FindOrBuildElement("Rb");
  auto elBa = nistManager->FindOrBuildElement("Ba");
  auto elAs = nistManager->FindOrBuildElement("As");
  auto elCs = nistManager->FindOrBuildElement("Cs");
  auto elNa = nistManager->FindOrBuildElement("Na");

  G4double fracMass;
  mat_mcp_glass->AddElement(elPb, 48 * perCent);
  mat_mcp_glass->AddElement(elO, 25.8 * perCent);
  mat_mcp_glass->AddElement(elSi, 18.2 * perCent);
  mat_mcp_glass->AddElement(elK, 4.2 * perCent);
  mat_mcp_glass->AddElement(elRb, 1.8 * perCent);
  mat_mcp_glass->AddElement(elBa, 1.3 * perCent);
  mat_mcp_glass->AddElement(elAs, 0.4 * perCent);
  mat_mcp_glass->AddElement(elCs, 0.2 * perCent);
  mat_mcp_glass->AddElement(elNa, fracMass = 0.1 * perCent);

  // Silicon
  mat_silicon = nistManager->FindOrBuildMaterial("G4_Si");
}

G4VSolid *ESSDetectorConstruction::ConstructSolidOuterLWU() {
  // LWU Creation
  G4double phiS = 0. * degree;
  G4double phiT = 360. * degree;
  G4double zplanes = 6;
  // Outer shapes
  G4double rIOuter[]{0 * cm, 0 * cm, 0 * cm, 0 * cm, 0 * cm, 0. * cm};
  G4double rOuter[]{55. * mm,  55. * mm, 130. * mm,
                    130. * mm, 55. * mm, 55. * mm};
  G4double zplaneOuter[]{-285. * mm, -2 * mm,  0. * cm,
                         457 * mm,   457 * mm, 1050 * mm};
  auto pipeOuterBeamPoly = new G4Polycone("PolyChamber", phiS, phiT, zplanes,
                                          zplaneOuter, rIOuter, rOuter);
  auto wireScannerOuterBox =
      new G4Box("boxWSOuter", 155. * mm, 80. * mm, 29. * mm);
  auto cf200OuterTub =
      new G4Tubs("cf200Outer", .0, 105 * mm, 8 * cm, phiS, phiT);

  // LWU => Position and rotations
  auto posLWU = G4ThreeVector(.0 * mm, .0 * mm, .0 * mm);
  auto rotLWU = G4RotationMatrix();
  auto trLWU = G4Transform3D(rotLWU, posLWU);
  // Wire Scanner
  auto posWS1 = G4ThreeVector(0. * mm, 100. * mm, 65 * mm);
  auto posWS2 = G4ThreeVector(-100. * mm, 0., 65 * mm);
  auto rotWS1 = G4RotationMatrix();
  rotWS1.rotateZ(90. * deg);
  auto rotWS2 = G4RotationMatrix();
  auto trWS1 = G4Transform3D(rotWS1, posWS1);
  auto trWS2 = G4Transform3D(rotWS2, posWS2);
  // CF200
  auto posCF200_1 = G4ThreeVector(100. * mm, 0. * mm, 206 * mm);
  auto posCF200_2 = G4ThreeVector(0. * mm, 100. * mm, 337 * mm);
  auto rotCF200_1 = G4RotationMatrix();
  rotCF200_1.rotateY(90. * deg);
  auto rotCF200_2 = G4RotationMatrix();
  rotCF200_2.rotateX(90. * deg);
  // rotCF200_2.rotateX(90. * deg);
  auto trCF200_1 = G4Transform3D(rotCF200_1, posCF200_1);
  auto trCF200_2 = G4Transform3D(rotCF200_2, posCF200_2);

  // Union for outer shell
  G4MultiUnion *unionOuter = new G4MultiUnion("OuterUnion");
  unionOuter->AddNode(*pipeOuterBeamPoly, trLWU);
  unionOuter->AddNode(*wireScannerOuterBox, trWS1);
  unionOuter->AddNode(*wireScannerOuterBox, trWS2);
  unionOuter->AddNode(*cf200OuterTub, trCF200_1);
  unionOuter->AddNode(*cf200OuterTub, trCF200_2);
  unionOuter->Voxelize();
  return unionOuter;
}

G4VSolid *ESSDetectorConstruction::ConstructSolidInnerLWU() {
  // LWU Creation
  G4double phiS = 0. * degree;
  G4double phiT = 360. * degree;
  G4double zplanes = 6;
  // Inner shapes
  G4double rIInner[]{0 * cm, 0 * cm, 0 * cm, 0 * cm, 0 * cm, 0. * cm};
  G4double rInner[]{50 * mm, 50 * mm, 125 * mm, 125 * mm, 50 * mm, 50 * mm};
  G4double zplaneInner[]{-280. * mm, 0 * mm,   0. * cm,
                         455 * mm,   455 * mm, 1045 * mm};
  auto pipeInnerBeamPoly = new G4Polycone("PolyVacuum", phiS, phiT, zplanes,
                                          zplaneInner, rIInner, rInner);
  auto wireScannerInnerBox =
      new G4Box("boxSInner", 150. * mm, 75. * mm, 24. * mm);
  auto cf200InnerTub =
      new G4Tubs("cf200Inner", .0, 100 * mm, 6 * cm, phiS, phiT);

  // LWU => Position and rotations
  auto posLWU = G4ThreeVector(.0 * mm, .0 * mm, .0 * mm);
  auto rotLWU = G4RotationMatrix();
  auto trLWU = G4Transform3D(rotLWU, posLWU);
  // Wire Scanner
  auto posWS1 = G4ThreeVector(0. * mm, 100. * mm, 65 * mm);
  auto posWS2 = G4ThreeVector(-100. * mm, 0., 65 * mm);
  auto rotWS1 = G4RotationMatrix();
  rotWS1.rotateZ(90. * deg);
  auto rotWS2 = G4RotationMatrix();
  auto trWS1 = G4Transform3D(rotWS1, posWS1);
  auto trWS2 = G4Transform3D(rotWS2, posWS2);
  // CF200
  auto posCF200_1 = G4ThreeVector(100. * mm, 0. * mm, 206 * mm);
  auto posCF200_2 = G4ThreeVector(0. * mm, 100. * mm, 337 * mm);
  auto rotCF200_1 = G4RotationMatrix();
  rotCF200_1.rotateY(90. * deg);
  auto rotCF200_2 = G4RotationMatrix();
  rotCF200_2.rotateX(90. * deg);
  // rotCF200_2.rotateX(90. * deg);
  auto trCF200_1 = G4Transform3D(rotCF200_1, posCF200_1);
  auto trCF200_2 = G4Transform3D(rotCF200_2, posCF200_2);
  // Union for inner shell
  G4MultiUnion *unionInner = new G4MultiUnion("InnerUnion");
  unionInner->AddNode(*pipeInnerBeamPoly, trLWU);
  unionInner->AddNode(*wireScannerInnerBox, trWS1);
  unionInner->AddNode(*wireScannerInnerBox, trWS2);
  unionInner->AddNode(*cf200InnerTub, trCF200_1);
  unionInner->AddNode(*cf200InnerTub, trCF200_2);
  unionInner->Voxelize();
  return unionInner;
}

G4VSolid *ESSDetectorConstruction::ConstructSolidFrame() {
  auto meshIPM = CADMesh::TessellatedMesh::FromSTL("IPM/Frame.stl");
  meshIPM->SetScale(m);
  meshIPM->SetReverse(false);
  meshIPM->SetOffset(
      G4ThreeVector(-1.12595 * mm, -958.165 * mm, -261.819 * mm));
  return meshIPM->GetSolid();
}

G4VSolid *ESSDetectorConstruction::ConstructSolidPCB() {
  auto meshPCBBT = CADMesh::TessellatedMesh::FromSTL("IPM/Plaque_BT.stl");
  meshPCBBT->SetScale(mm);
  meshPCBBT->SetReverse(false);
  meshPCBBT->SetOffset(
      G4ThreeVector(-1.12595 * mm, -916.165 * mm, -311.819 * mm));
  return meshPCBBT->GetSolid();
}

G4VSolid *ESSDetectorConstruction::ConstructSolidMCP() {

  G4double phiS = 0. * degree;
  G4double phiT = 360. * degree;
  return new G4Tubs("MCP", 0.0 * mm, 23.0 * mm, 0.5 * mm, phiS, phiT);
  /*   auto meshMCP = CADMesh::TessellatedMesh::FromSTL("MCP/MCP_glass.stl");
  meshMCP->SetScale(mm);
  meshMCP->SetReverse(false);
  return meshMCP->GetSolid(); */
}

G4VSolid *ESSDetectorConstruction::ConstructSolidPhos() {
  G4double phiS = 0. * degree;
  G4double phiT = 360. * degree;
  return new G4Tubs("Phos", 0.0 * mm, 23.0 * mm, 0.25 * mm, phiS, phiT);
}

G4VSolid *ESSDetectorConstruction::ConstructSolidFrameMCP() {
  G4double phiS = 0. * degree;
  G4double phiT = 360. * degree;
  return new G4Tubs("FrameMCP", 23.5 * mm, 35.0 * mm, 2.5 * mm, phiS, phiT);
}

G4VSolid *ESSDetectorConstruction::ConstructSolidDisk() {
  G4double phiS = 0. * degree;
  G4double phiT = 360. * degree;
  return new G4Tubs("PolyDisk", 50.0 * mm, 123.0 * mm, 0.5 * mm, phiS, phiT);
}

G4VSolid *ESSDetectorConstruction::ConstructSolidCamera() {
  auto solidCam = CADMesh::TessellatedMesh::FromSTL("Camera/Camera.stl");
  solidCam->SetScale(mm);
  solidCam->SetReverse(false);
  return solidCam->GetSolid();
}

G4VSolid *ESSDetectorConstruction::ConstructSolidSensor() {
  auto sensorBox = new G4Box("boxSensor", nx_pixels * size_pixel / 2,
                             ny_pixels * size_pixel / 2, thickness_sensor);
  return sensorBox;
}

G4VSolid *ESSDetectorConstruction::ConstructSolidLens() {
  auto solidLens = CADMesh::TessellatedMesh::FromSTL("Lens/Lens.stl");
  solidLens->SetScale(mm);
  solidLens->SetReverse(false);
  return solidLens->GetSolid();
}

G4VSolid *ESSDetectorConstruction::ConstructSolidQuad() {
  auto solidQuad = CADMesh::TessellatedMesh::FromSTL("LWU_E_type1/Quad_1.stl");
  solidQuad->SetScale(mm);
  solidQuad->SetReverse(false);
  solidQuad->SetOffset(G4ThreeVector(-540 * mm, .0 * mm, 0.0 * mm));
  return solidQuad->GetSolid();
}

G4VSolid *ESSDetectorConstruction::ConstructSolidQuadSupport() {
  auto solidQuadSupport =
      CADMesh::TessellatedMesh::FromSTL("LWU_E_type1/Quad_Support_1.stl");
  solidQuadSupport->SetScale(mm);
  solidQuadSupport->SetReverse(false);
  solidQuadSupport->SetOffset(G4ThreeVector(540 * mm, 0 * mm, 0 * mm));
  return solidQuadSupport->GetSolid();
}

G4VSolid *ESSDetectorConstruction::ConstructSolidLWUSupport() {
  auto supportL1 = new G4Box("Support", 110. * mm, 110. * mm, 770. * mm);
  auto supportL2 = new G4Box("Support", 45. * mm, 90. * mm, 780. * mm);
  auto solidL = new G4SubtractionSolid("SupportLWU_L", supportL1, supportL2);
  auto supportT1 = new G4Box("Support", 155. * mm, 110. * mm, 62. * mm);
  auto supportT2 = new G4Box("Support", 160. * mm, 90. * mm, 45. * mm);
  auto solidT = new G4SubtractionSolid("SupportLWU_T", supportT1, supportT2);

  auto transL1 = G4Transform3D(G4RotationMatrix(),
                               G4ThreeVector(.0 * mm, .0 * mm, .0 * mm));
  auto transL2 = G4Transform3D(G4RotationMatrix(),
                               G4ThreeVector(-430.0 * mm, .0 * mm, .0 * mm));
  auto transT1 = G4Transform3D(G4RotationMatrix(),
                               G4ThreeVector(-210 * mm, .0 * mm, 553.0 * mm));
  auto transT2 = G4Transform3D(G4RotationMatrix(),
                               G4ThreeVector(-210 * mm, .0 * mm, -553.0 * mm));

  G4MultiUnion *unionSolid = new G4MultiUnion("UnionSupportLWU");
  unionSolid->AddNode(*solidL, transL1);
  unionSolid->AddNode(*solidL, transL2);
  unionSolid->AddNode(*solidT, transT1);
  unionSolid->AddNode(*solidT, transT2);
  unionSolid->Voxelize();
  /*
  auto offsetsupport1 = G4ThreeVector(720 * mm, -215 * mm, 510 * mm);
  auto rotSupport = new G4RotationMatrix();
  CADMesh *meshSupport1 =
      new CADMesh("LWU_E_type1/outPart__Feature_289-10233-01_AF10000.stl", mm,
                  offsetsupport1, false);
  G4VSolid *cadsolidSupport = meshSupport1->TessellatedMesh();
  auto SupportL = new G4LogicalVolume(cadsolidSupport, steelPipe, "SupportL");
  rotSupport->rotateX(-90 * deg);
  rotSupport->rotateZ(-90 * deg);

  new G4PVPlacement(rotSupport,
                    G4ThreeVector(210. * mm, -513. * mm, -456. * mm), SupportL,
                    "Support1", worldLV, false, 0, checkOverlaps);
  new G4PVPlacement(rotSupport,
                    G4ThreeVector(-210. * mm, -513. * mm, -456. * mm), SupportL,
                    "Support1", worldLV, false, 1, checkOverlaps);

  // always return the physical World
  */
  return unionSolid;
}

void ESSDetectorConstruction::ConstructLWU(G4VSolid *solidOuter,
                                           G4VSolid *solidInner) {
  pipeL = new G4LogicalVolume(solidOuter, mat_steel, "PolyChamberOuterL");

  new G4PVPlacement(0, G4ThreeVector(0, 0, 0 * mm), pipeL, "PipeOuter", worldL,
                    false, 0, checkOverlaps);

  vacuumL = new G4LogicalVolume(solidInner, mat_vacuum, "PolyChamberInnerL");

  new G4PVPlacement(0, G4ThreeVector(0, 0, 0 * mm), vacuumL, "PipeInner", pipeL,
                    false, 0, checkOverlaps);
}

void ESSDetectorConstruction::ConstructDisk(G4VSolid *solidDisk) {
  diskL = new G4LogicalVolume(solidDisk, mat_steel, "DiskL");
  new G4PVPlacement(0, G4ThreeVector(.0, .0, 122 * mm), diskL, "Disk1", vacuumL,
                    false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(.0, .0, 289 * mm), diskL, "Disk2", vacuumL,
                    false, 1, checkOverlaps);
}

void ESSDetectorConstruction::ConstructIPM(G4VSolid *solidFrame,
                                           G4VSolid *solidPCB_BT,
                                           G4VSolid *solidPCB_HT) {
  CIPML = new G4LogicalVolume(solidFrame, mat_macor, "IPML");
  PCB_BTL = new G4LogicalVolume(solidPCB_BT, mat_pcb, "PCBBTL");
  PCB_HTL = new G4LogicalVolume(solidPCB_HT, mat_pcb, "PCBL");
  auto rotPCB = new G4RotationMatrix();

  rotPCB->rotateZ(90. * deg);
  new G4PVPlacement(nullptr, G4ThreeVector(-50.5 * mm, .0 * mm, 206 * mm),
                    PCB_HTL, "PCB1", vacuumL, false, 0, checkOverlaps);
  new G4PVPlacement(rotPCB, G4ThreeVector(0 * mm, 50.5 * mm, 206 * mm), PCB_HTL,
                    "PCB2", vacuumL, false, 1, checkOverlaps);
  new G4PVPlacement(rotPCB, G4ThreeVector(0 * mm, -50.5 * mm, 206 * mm),
                    PCB_HTL, "PCB3", vacuumL, false, 2, checkOverlaps);

  new G4PVPlacement(nullptr, G4ThreeVector(-50.5 * mm, .0 * mm, 372 * mm),
                    PCB_HTL, "PCB4", vacuumL, false, 3, checkOverlaps);
  new G4PVPlacement(nullptr, G4ThreeVector(50.5 * mm, .0 * mm, 372 * mm),
                    PCB_HTL, "PCB5", vacuumL, false, 4, checkOverlaps);
  new G4PVPlacement(rotPCB, G4ThreeVector(0 * mm, -50.5 * mm, 372 * mm),
                    PCB_HTL, "PCB6", vacuumL, false, 5, checkOverlaps);

  auto rotPCBBT = new G4RotationMatrix();
  rotPCBBT->rotateY(-90. * deg);
  rotPCBBT->rotateZ(90. * deg);
  new G4PVPlacement(new G4RotationMatrix(*rotPCBBT),
                    G4ThreeVector(49.5 * mm, .0 * mm, 206 * mm), PCB_BTL,
                    "PCBBT1", vacuumL, false, 0, checkOverlaps);

  auto rotIPM1 = new G4RotationMatrix();
  rotIPM1->rotateX(-90. * deg);
  rotIPM1->rotateY(-90. * deg);
  new G4PVPlacement(new G4RotationMatrix(*rotIPM1),
                    G4ThreeVector(.0 * mm, 0.0 * mm, 248 * mm), CIPML, "C1IPM1",
                    vacuumL, false, 0, checkOverlaps);

  rotIPM1 = new G4RotationMatrix();
  rotIPM1->rotateX(90. * deg);
  rotIPM1->rotateY(-90. * deg);
  new G4PVPlacement(new G4RotationMatrix(*rotIPM1),
                    G4ThreeVector(.0 * mm, -0.0 * mm, 164 * mm), CIPML,
                    "C2IPM1", vacuumL, false, 1, checkOverlaps);

  // IPM2
  auto rotIPM2 = new G4RotationMatrix();
  rotIPM2->rotateX(90. * deg);
  rotPCBBT = new G4RotationMatrix();
  rotPCBBT->rotateX(90. * deg);

  new G4PVPlacement(rotPCBBT, G4ThreeVector(0 * mm, 49.5 * mm, 372 * mm),
                    PCB_BTL, "PCBBT2", vacuumL, false, 1, checkOverlaps);

  new G4PVPlacement(new G4RotationMatrix(*rotIPM2),
                    G4ThreeVector(0 * mm, 0.0 * mm, 330 * mm), CIPML, "C1IPM2",
                    vacuumL, false, 0, checkOverlaps);

  rotIPM2 = new G4RotationMatrix();
  rotIPM2->rotateX(90. * deg);
  rotIPM2->rotateZ(180. * deg);
  new G4PVPlacement(new G4RotationMatrix(*rotIPM2),
                    G4ThreeVector(0 * mm, .0 * mm, 414 * mm), CIPML, "C2IPM2",
                    vacuumL, false, 1, checkOverlaps);
}

void ESSDetectorConstruction::ConstructMCP(G4VSolid *solidMCP,
                                           G4VSolid *solidPhos,
                                           G4VSolid *solidFrame) {
  mcpL = new G4LogicalVolume(solidMCP, mat_mcp_glass, "MCPL");
  phosL = new G4LogicalVolume(solidPhos, mat_silicon, "PhosL");
  mcp_frameL = new G4LogicalVolume(solidFrame, mat_steel, "FrameMCPL");

  auto rotMCP = new G4RotationMatrix();
  rotMCP->rotateY(90. * deg);
  new G4PVPlacement(new G4RotationMatrix(*rotMCP),
                    G4ThreeVector(58. * mm, .0 * mm, 206 * mm), mcpL, "MCP1",
                    vacuumL, false, 0, checkOverlaps);
  new G4PVPlacement(new G4RotationMatrix(*rotMCP),
                    G4ThreeVector(62.5 * mm, .0 * mm, 206 * mm), phosL, "Phos1",
                    vacuumL, false, 0, checkOverlaps);
  new G4PVPlacement(new G4RotationMatrix(*rotMCP),
                    G4ThreeVector(60. * mm, .0 * mm, 206 * mm), mcp_frameL,
                    "FrameMCP1", vacuumL, false, 0, checkOverlaps);

  rotMCP = new G4RotationMatrix();
  rotMCP->rotateX(90. * deg);
  new G4PVPlacement(new G4RotationMatrix(*rotMCP),
                    G4ThreeVector(0. * mm, 58 * mm, 372 * mm), mcpL, "MCP2",
                    vacuumL, false, 1, checkOverlaps);
  new G4PVPlacement(new G4RotationMatrix(*rotMCP),
                    G4ThreeVector(0. * mm, 62.5 * mm, 372 * mm), phosL, "Phos2",
                    vacuumL, false, 1, checkOverlaps);
  new G4PVPlacement(new G4RotationMatrix(*rotMCP),
                    G4ThreeVector(0. * mm, 60.0 * mm, 372 * mm), mcp_frameL,
                    "FrameMCP2", vacuumL, false, 1, checkOverlaps);
}

void ESSDetectorConstruction::ConstructVisionS(G4VSolid *solidCam,
                                               G4VSolid *solidLens,
                                               G4VSolid *solidSensor) {
  auto rotCam1 = new G4RotationMatrix();
  rotCam1->rotateY(90. * deg);
  rotCam1->rotateZ(90. * deg);

  camL = new G4LogicalVolume(solidCam, mat_steel, "CameraL");
  lensL = new G4LogicalVolume(solidLens, mat_steel, "LensL");
  sensorL = new G4LogicalVolume(solidSensor, mat_mcp_glass, "SensorL");

  new G4PVPlacement(rotCam1, G4ThreeVector(350. * mm, .0 * mm, 206 * mm), camL,
                    "Cam1", worldL, false, 0, checkOverlaps);
  new G4PVPlacement(nullptr, G4ThreeVector(-3. * mm, .0 * mm, 0 * mm), sensorL,
                    "Sensor1", camL, false, 0, checkOverlaps);
  new G4PVPlacement(rotCam1, G4ThreeVector(290. * mm, .0 * mm, 206 * mm), lensL,
                    "Lens1", worldL, false, 0, checkOverlaps);
  auto rotCam2 = new G4RotationMatrix();
  rotCam2->rotateX(-90. * deg);
  new G4PVPlacement(rotCam2, G4ThreeVector(.0 * mm, 350. * mm, 372 * mm), camL,
                    "Cam2", worldL, false, 1, checkOverlaps);
  new G4PVPlacement(rotCam2, G4ThreeVector(.0 * mm, 290. * mm, 372 * mm), lensL,
                    "Lens2", worldL, false, 1, checkOverlaps);

  // Pixels
  auto solidPixelRows = new G4Box("SolidPixelRows", size_pixel / 2,
                                  ny_pixels * size_pixel / 2, thickness_sensor);
  auto solidPixelCells = new G4Box("SolidPixelCells", size_pixel / 2,
                                   size_pixel / 2, thickness_sensor);

  pixelRowsL = new G4LogicalVolume(solidPixelRows, mat_silicon, "PixelRowsL");
  pixelCellsL =
      new G4LogicalVolume(solidPixelCells, mat_silicon, "PixelCellsL");

  new G4PVReplica("PixelRows", pixelRowsL, sensorL, kXAxis, nx_pixels,
                  size_pixel);

  new G4PVReplica("PixelCells", pixelCellsL, pixelRowsL, kYAxis, ny_pixels,
                  size_pixel);
}

void ESSDetectorConstruction::ConstructQuad(G4VSolid *solidQuad) {
  auto rotQuad = new G4RotationMatrix();
  rotQuad->rotateY(90 * deg);
  quadL = new G4LogicalVolume(solidQuad, mat_copper, "QuadL");
  G4double z = -175;
  for (auto i = 0; i < 8; i++) {
    if (i > 3)
      z = 905;
    new G4PVPlacement(
        new G4RotationMatrix(*rotQuad), G4ThreeVector(.0 * mm, 0. * mm, z * mm),
        quadL, "Quad" + std::to_string(i), worldL, false, i, checkOverlaps);
    rotQuad->rotateX(-90 * deg);
  }
}

void ESSDetectorConstruction::ConstructQuadSupport(G4VSolid *solidQuadSupport) {
  auto rotQuadSupport = new G4RotationMatrix();
  quad_SupportL =
      new G4LogicalVolume(solidQuadSupport, mat_steel, "QuadSupportL");
  rotQuadSupport->rotateX(90 * deg);
  rotQuadSupport->rotateZ(90 * deg);
  new G4PVPlacement(new G4RotationMatrix(*rotQuadSupport),
                    G4ThreeVector(0. * mm, 0. * mm, -175. * mm), quad_SupportL,
                    "QuadSupport1", worldL, false, 0, checkOverlaps);
  rotQuadSupport->rotateZ(-180 * deg);
  new G4PVPlacement(rotQuadSupport, G4ThreeVector(0. * mm, 0. * mm, 905. * mm),
                    quad_SupportL, "QuadSupport2", worldL, false, 1,
                    checkOverlaps);
}

void ESSDetectorConstruction::ConstructLWUSupport(G4VSolid *solidSupportLWU) {
  LWU_SupportL = new G4LogicalVolume(solidSupportLWU, mat_steel, "SupportL");
  new G4PVPlacement(nullptr, G4ThreeVector(230. * mm, -510. * mm, 348.6 * mm),
                    LWU_SupportL, "Support", worldL, false, 0, checkOverlaps);
}