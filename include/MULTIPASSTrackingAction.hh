//
// File:         MULTIPASS_Det/include/MULTIPASSTrackingAction.hh
// Description:  TrackingAction header for MULTIPASS simulation. Based on 
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
#ifndef MULTIPASSTrackingAction_h
#define MULTIPASSTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class MULTIPASSTrackingAction : public G4UserTrackingAction {

  public:

    MULTIPASSTrackingAction();
    virtual ~MULTIPASSTrackingAction() {};

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

  private:


};

#endif
