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

#ifndef ESSCameraHit_h
#define ESSCameraHit_h 1

#include <G4Allocator.hh>
#include <G4THitsCollection.hh>
#include <G4VHit.hh>

class ESSCameraHit : public G4VHit {
public:
  ESSCameraHit();
  virtual ~ESSCameraHit();
  ESSCameraHit(const ESSCameraHit &right);
  const ESSCameraHit &operator=(const ESSCameraHit &right);
  int operator==(const ESSCameraHit &right) const;

  inline void *operator new(size_t);
  inline void operator delete(void *hit);

  virtual void Print();

  void SetCamNumber(G4int number) { fCamNumber = number; }
  void AddEdep(G4double edep) { fEdep += edep; }

  G4int GetCamNumber() const { return fCamNumber; }
  G4double GetEdep() const { return fEdep; }

private:
  G4int fCamNumber;
  G4double fEdep;
};

typedef G4THitsCollection<ESSCameraHit> ESSCameraHitsCollection;

extern G4Allocator<ESSCameraHit> *ESSCameraHitAllocator;

inline void *ESSCameraHit::operator new(size_t) {
  if (!ESSCameraHitAllocator)
    ESSCameraHitAllocator = new G4Allocator<ESSCameraHit>;
  return (void *)ESSCameraHitAllocator->MallocSingle();
}

inline void ESSCameraHit::operator delete(void *hit) {
  ESSCameraHitAllocator->FreeSingle((ESSCameraHit *)hit);
}

#endif
