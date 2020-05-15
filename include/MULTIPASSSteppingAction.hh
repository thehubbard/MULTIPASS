//
// File:         MULTIPASS_Det/include/MULTIPASSSteppingAction.hh
// Description:  SteppingAction header for MULTIPASS simulation. Based on 
//               original code from Geant4 optical/LXe from 10.4.p01
// Version:      1.0.0
// Created:      2020-03-18
// Author:       Michael Hubbard
// Modified:     
// 
//
// Tested with:
//              Geant4 v 10.4.p01
//              ROOT v 6.12.06
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//

#ifndef MULTIPASSSteppingAction_H
#define MULTIPASSSteppingACtion_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

#include "G4OpBoundaryProcess.hh"

class MULTIPASSEventAction;
class MULTIPASSTrackingAction;
class MULTIPASSSteppingMessenger;

class MULTIPASSSteppingAction : public G4UserSteppingAction
{
  public:

    MULTIPASSSteppingAction();
    virtual ~MULTIPASSSteppingAction();
    virtual void UserSteppingAction(const G4Step*);

    void SetOneStepPrimaries(G4bool b){fOneStepPrimaries=b;}
    G4bool GetOneStepPrimaries(){return fOneStepPrimaries;}
 
  private:

    G4bool fOneStepPrimaries;
    MULTIPASSSteppingMessenger* fSteppingMessenger;

    G4OpBoundaryProcessStatus fExpectedNextStatus;
};

#endif
