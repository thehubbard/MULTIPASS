//
// File:         MULTIPASS_Det/include/MULTIPASSSteppingMessenger.hh
// Description:  SteppingMessenger header for MULTIPASS simulation. Based 
//				 on original code from Geant4 optical/LXe from 10.4.p01
// Version:      1.0.0
// Created:      2020-03-18
// Author:       Michael Hubbard
// Modified:     
//
// Tested with:
//              Geant4 v 10.4.p01
//              ROOT v 6.12.06
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//
#ifndef MULTIPASSSteppingMessenger_h
#define MULTIPASSSteppingMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class MULTIPASSSteppingAction;
class G4UIcmdWithABool;

class MULTIPASSSteppingMessenger: public G4UImessenger
{
  public:
    MULTIPASSSteppingMessenger(MULTIPASSSteppingAction*);
    virtual ~MULTIPASSSteppingMessenger();
 
    virtual void SetNewValue(G4UIcommand*, G4String);

  private:

    MULTIPASSSteppingAction*        fStepping;
    G4UIcmdWithABool*  fOneStepPrimariesCmd;
 
};

#endif
