#ifndef ESSMCPHit_h
#define ESSMCPHit_h 1

#include <G4Allocator.hh>
#include <G4THitsCollection.hh>
#include <G4VHit.hh>
#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>

class ESSMCPHit : public G4VHit {
public:
  ESSMCPHit();
  virtual ~ESSMCPHit();
  ESSMCPHit(const ESSMCPHit &right);
  const ESSMCPHit &operator=(const ESSMCPHit &right);
  int operator==(const ESSMCPHit &right) const;

  inline void *operator new(size_t);
  inline void operator delete(void *hit);

  virtual void Print();

  void SetTime(G4double time){ fTime = time;}
  void SetPosition(G4ThreeVector position){fPosition = position;}
  void SetMomentum(G4ThreeVector Momentum){fMomentum = Momentum;}
  void SetMCPNumber(G4int number) { fMCPnumber = number; }
  void AddEdep(G4double edep) { fEdep += edep; }

  G4double GetTime() const { return fTime;}
  G4ThreeVector GetPosition() const { return fPosition;}
  G4ThreeVector GetMomentum() const { return fMomentum;}
  G4int GetMCPNumber() const { return fMCPnumber; }
  G4double GetEdep() const { return fEdep; }

private:
  G4int fMCPnumber;
  G4double fTime;
  G4double fEdep;
  G4ThreeVector fPosition;
  G4ThreeVector fMomentum;
  G4RotationMatrix fRot;
};

using ESSMCPHitsCollection = G4THitsCollection<ESSMCPHit>;

extern G4ThreadLocal G4Allocator<ESSMCPHit> *ESSMCPHitAllocator;

inline void *ESSMCPHit::operator new(size_t) {
  if (!ESSMCPHitAllocator)
    ESSMCPHitAllocator = new G4Allocator<ESSMCPHit>;
  return (void *)ESSMCPHitAllocator->MallocSingle();
}

inline void ESSMCPHit::operator delete(void *hit) {
  ESSMCPHitAllocator->FreeSingle((ESSMCPHit *)hit);
}

#endif
