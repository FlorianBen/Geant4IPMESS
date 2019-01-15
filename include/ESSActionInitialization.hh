#ifndef ESSActionInitialization_h
#define EESActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class ESSActionInitialization : public G4VUserActionInitialization {
public:
  ESSActionInitialization();
  virtual ~ESSActionInitialization();

  virtual void Build() const;
  virtual void BuildForMaster() const;
};

#endif