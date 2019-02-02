#ifndef ESSBeamStopHit_h
#define ESSBeamStopHit_h 1

#include <G4Allocator.hh>
#include <G4THitsCollection.hh>
#include <G4VHit.hh>
#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>

class ESSBeamStopHit : public G4VHit {
public:
  ESSBeamStopHit();
  virtual ~ESSBeamStopHit();
  ESSBeamStopHit(const ESSBeamStopHit &right);
  const ESSBeamStopHit &operator=(const ESSBeamStopHit &right);
  int operator==(const ESSBeamStopHit &right) const;

  inline void *operator new(size_t);
  inline void operator delete(void *hit);

  virtual void Print();

  void SetTime(G4double time){ fTime = time;}
  void SetPosition(G4ThreeVector position){fPosition = position;}
  void SetMomentum(G4ThreeVector Momentum){fMomentum = Momentum;}
  void AddEdep(G4double edep) { fEdep += edep; }

  G4double GetTime() const { return fTime;}
  G4ThreeVector GetPosition() const { return fPosition;}
  G4ThreeVector GetMomentum() const { return fMomentum;}
  G4double GetEdep() const { return fEdep; }

private:
  G4double fTime;
  G4double fEdep;
  G4ThreeVector fPosition;
  G4ThreeVector fMomentum;
  G4RotationMatrix fRot;
};

using ESSBeamStopHitsCollection = G4THitsCollection<ESSBeamStopHit>;

extern G4ThreadLocal G4Allocator<ESSBeamStopHit> *ESSBeamStopHitAllocator;

inline void *ESSBeamStopHit::operator new(size_t) {
  if (!ESSBeamStopHitAllocator)
    ESSBeamStopHitAllocator = new G4Allocator<ESSBeamStopHit>;
  return (void *)ESSBeamStopHitAllocator->MallocSingle();
}

inline void ESSBeamStopHit::operator delete(void *hit) {
  ESSBeamStopHitAllocator->FreeSingle((ESSBeamStopHit *)hit);
}

#endif
