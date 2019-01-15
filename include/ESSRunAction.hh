#ifndef ESSRunAction_h
#define ESSRunAction_h 1

#include <G4UserRunAction.hh>
#include <globals.hh>

class G4Run;

class ESSRunAction : public G4UserRunAction {
public:
  ESSRunAction();
  virtual ~ESSRunAction();

  virtual void BeginOfRunAction(const G4Run *);
  virtual void EndOfRunAction(const G4Run *);
};

#endif