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
#ifndef ESSDetectorConstruction_h
#define ESSDetectorConstruction_h 1

#include <G4Material.hh>
#include <G4VSolid.hh>
#include <G4VUserDetectorConstruction.hh>

class G4VPhysicalVolume;

/**
 * Detector construction class to define materials and geometry.
 */
class ESSDetectorConstruction : public G4VUserDetectorConstruction {
public:
  ESSDetectorConstruction();
  virtual ~ESSDetectorConstruction();

  virtual G4VPhysicalVolume *Construct();
  virtual void ConstructSDandField();

protected:
  /**
   * Setup materials.
   */
  virtual void ConstructMaterials();
  /**
   * Construct World solid.
   */
  G4VSolid *ConstructSolidWorld();
  /**
   * Construct LWU solid inner shell.
   */
  G4VSolid *ConstructSolidInnerLWU();
  /**
   * Construct LWU solid outer shell.
   */
  G4VSolid *ConstructSolidOuterLWU();
  /**
   * Construct Disk solid.
   */
  G4VSolid *ConstructSolidDisk();
  /**
   * Construck Camera solid.
   */
  G4VSolid *ConstructSolidCamera();
  /**
   * Construck Lens solid.
   */
  G4VSolid *ConstructSolidLens();
  /**
   * Construck IPM Frame solid.
   */
  G4VSolid *ConstructSolidFrame();
  /**
   * Construck IPM PCB solid.
   */
  G4VSolid *ConstructSolidPCB();
  /**
   * Construck IPM MCP solid.
   */
  G4VSolid *ConstructSolidMCP();
  /**
   * Construct Quadrupole solid.
   */
  G4VSolid *ConstructSolidQuad();
  /**
   * Construct Support for quadrupoles solid.
   */
  G4VSolid *ConstructSolidQuadSupport();
  /**
   * Construct Support for LWU solid.
   */
  G4VSolid *ConstructSolidLWUSupport();
  /**
   * Placement of LWU.
   */
  void ConstructLWU(G4VSolid *solidOuter, G4VSolid *solidInner);
  /**
   * Placement of Disks.
   */
  void ConstructDisk(G4VSolid *solidDisk);
  /**
   * Placement of camera + lens.
   */
  void ConstructVisionS(G4VSolid *solidCam, G4VSolid *solidLens);
  /**
   * Placement of IPMS.
   */
  void ConstructIPM(G4VSolid *solidFrame, G4VSolid *solidPCB_BT,
                    G4VSolid *solidPCB_HT);
  /**
   * Placement of MCPs.
   */
  void ConstructMCP();
  /**
   * Placement of Quadrupoles.
   */
  void ConstructQuad(G4VSolid *solidQuad);
  /**
   * Placement of Quadrupole Support.
   */
  void ConstructQuadSupport(G4VSolid *solidQuadSupport);
  /**
   * Placement of LWU support.
   */
  void ConstructLWUSupport(G4VSolid *solidLWUSupport);
  /**
   * Placement of Template.
   */
  void ConstructTemplate(G4VSolid *solidTemp);

private:
  G4bool checkOverlaps;
  G4Material *mat_air;
  G4Material *mat_copper;
  G4Material *mat_gold;
  G4Material *mat_macor;
  G4Material *mat_steel;
  G4Material *mat_vacuum;

  G4LogicalVolume *worldL;
  G4LogicalVolume *pipeL;
  G4LogicalVolume *vacuumL;
  G4LogicalVolume *diskL;
  G4LogicalVolume *CIPML;
  G4LogicalVolume *PCB_BTL;
  G4LogicalVolume *PCB_HTL;
  G4LogicalVolume *camL;
  G4LogicalVolume *lensL;
  G4LogicalVolume *quadL;
  G4LogicalVolume *quad_SupportL;
  G4LogicalVolume *LWU_SupportL;
};

#endif