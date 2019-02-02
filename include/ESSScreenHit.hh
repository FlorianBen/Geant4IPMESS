#ifndef ESSScreenStopHit_h
#define ESSScreenStopHit_h 1

#include <G4Allocator.hh>
#include <G4THitsCollection.hh>
#include <G4VHit.hh>
#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>

class ESSScreenStopHit : public G4VHit {
public:
  ESSScreenStopHit();
  virtual ~ESSScreenStopHit();
  ESSScreenStopHit(const ESSScreenStopHit &right);
  const ESSScreenStopHit &operator=(const ESSScreenStopHit &right);
  int operator==(const ESSScreenStopHit &right) const;

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

using ESSScreenStopHitsCollection = G4THitsCollection<ESSScreenStopHit>;

extern G4ThreadLocal G4Allocator<ESSScreenStopHit> *ESSScreenStopHitAllocator;

inline void *ESSScreenStopHit::operator new(size_t) {
  if (!ESSScreenStopHitAllocator)
    ESSScreenStopHitAllocator = new G4Allocator<ESSScreenStopHit>;
  return (void *)ESSScreenStopHitAllocator->MallocSingle();
}

inline void ESSScreenStopHit::operator delete(void *hit) {
  ESSScreenStopHitAllocator->FreeSingle((ESSScreenStopHit *)hit);
}

#endif
