//
// File:         MULTIPASS_Det/include/MULTIPASSStackingAction.hh
// Description:  StackingAction header for MULTIPASS simulation. Based on 
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

#ifndef MULTIPASSStackingAction_H
#define MULTIPASSStackingAction_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"

class MULTIPASSStackingAction : public G4UserStackingAction
{
  public:

    MULTIPASSStackingAction();
    virtual ~MULTIPASSStackingAction();
 
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
    virtual void NewStage();
    virtual void PrepareNewEvent();
 
  private:
};

#endif
