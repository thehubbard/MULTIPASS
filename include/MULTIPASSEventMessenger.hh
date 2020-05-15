//
// File:         MULTIPASS_Det/include/MULTIPASSEventMessenger.hh
// Description:  EventMessenger header for MULTIPASS simulation. Based on 
//               original code from Geant4 optical/LXe from 10.4.p01
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
#ifndef MULTIPASSEventMessenger_h
#define MULTIPASSEventMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class MULTIPASSEventAction;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

class MULTIPASSEventMessenger: public G4UImessenger
{
  public:

    MULTIPASSEventMessenger(MULTIPASSEventAction*);
    virtual ~MULTIPASSEventMessenger();
 
    virtual void SetNewValue(G4UIcommand*, G4String);
 
  private:

    MULTIPASSEventAction*      fEJ276Event;
    G4UIcmdWithAnInteger*  fSaveThresholdCmd;
    G4UIcmdWithAnInteger*  fVerboseCmd;
    G4UIcmdWithAnInteger*  fPmtThresholdCmd;
    G4UIcmdWithABool*      fForceDrawPhotonsCmd;
    G4UIcmdWithABool*      fForceDrawNoPhotonsCmd;
};

#endif
