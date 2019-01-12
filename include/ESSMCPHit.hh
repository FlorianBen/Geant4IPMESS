#ifndef ESSMCPHit_h
#define ESSMCPHit_h 1

#include <G4Allocator.hh>
#include <G4THitsCollection.hh>
#include <G4VHit.hh>

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

  void SetCamNumber(G4int number) { fCamNumber = number; }
  void AddEdep(G4double edep) { fEdep += edep; }

  G4int GetCamNumber() const { return fCamNumber; }
  G4double GetEdep() const { return fEdep; }

private:
  G4int fCamNumber;
  G4double fEdep;
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
