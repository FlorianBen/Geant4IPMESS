#ifndef ESSBeamStopSD_h
#define ESSBeamStopSD_h 1

#include <G4VSensitiveDetector.hh>

#include "ESSBeamStopHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class ESSBeamStopSD : public G4VSensitiveDetector {
public:
  ESSBeamStopSD(const G4String &name, const G4String &hitsCollectionName, const G4int idTuple);
  virtual ~ESSBeamStopSD();

  virtual void Initialize(G4HCofThisEvent *hce);
  virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history);

private:
  ESSBeamStopHitsCollection *fHitsCollection;
  G4int fidTuple;
};

#endif