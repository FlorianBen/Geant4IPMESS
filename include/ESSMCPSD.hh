#ifndef ESSMCPSD_h
#define ESSMCPSD_h 1

#include <G4VSensitiveDetector.hh>

#include "ESSMCPHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class ESSMCPSD : public G4VSensitiveDetector {
public:
  ESSMCPSD(const G4String &name, const G4String &hitsCollectionName);
  virtual ~ESSMCPSD();

  virtual void Initialize(G4HCofThisEvent *hce);
  virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history);

private:
  ESSMCPHitsCollection *fHitsCollection;
};

#endif