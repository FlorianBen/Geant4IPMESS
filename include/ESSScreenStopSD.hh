#ifndef ESSScreenStopSD_h
#define ESSScreenStopSD_h 1

#include <G4VSensitiveDetector.hh>

#include "ESSScreenStopHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class ESSScreenStopSD : public G4VSensitiveDetector {
public:
  ESSScreenStopSD(const G4String &name, const G4String &hitsCollectionName);
  virtual ~ESSScreenStopSD();

  virtual void Initialize(G4HCofThisEvent *hce);
  virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history);

private:
  ESSScreenStopHitsCollection *fHitsCollection;
};

#endif