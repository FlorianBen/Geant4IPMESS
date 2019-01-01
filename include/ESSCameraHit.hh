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

using ESSCameraHitsCollection = G4THitsCollection<ESSCameraHit>;

extern G4ThreadLocal G4Allocator<ESSCameraHit> *ESSCameraHitAllocator;

inline void *ESSCameraHit::operator new(size_t) {
  if (!ESSCameraHitAllocator)
    ESSCameraHitAllocator = new G4Allocator<ESSCameraHit>;
  return (void *)ESSCameraHitAllocator->MallocSingle();
}

inline void ESSCameraHit::operator delete(void *hit) {
  ESSCameraHitAllocator->FreeSingle((ESSCameraHit *)hit);
}

#endif
