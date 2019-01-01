#ifndef ESSCameraSD_h
#define ESSCameraSD_h 1

#include <G4VSensitiveDetector.hh>

#include "ESSCameraHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class ESSCameraSD : public G4VSensitiveDetector {
public:
  ESSCameraSD(const G4String &name, const G4String &hitsCollectionName);
  virtual ~ESSCameraSD();

  virtual void Initialize(G4HCofThisEvent *hce);
  virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history);

private:
  ESSCameraHitsCollection *fHitsCollection;
};

#endif