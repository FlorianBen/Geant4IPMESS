#ifndef ESSEventAction_h
#define ESSEventAction_h 1

#include <G4UserEventAction.hh>

class ESSEventAction : public G4UserEventAction {
public:
  ESSEventAction(bool verbose = true);
  virtual ~ESSEventAction();

  virtual void BeginOfEventAction(const G4Event *event);
  virtual void EndOfEventAction(const G4Event *event);

private:
  bool fVerbose;
};

#endif
