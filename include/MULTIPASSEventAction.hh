//
// File:         MULTIPASS_Det/include/MULTIPASSEventAction.hh
// Description:  EventAction header for MULTIPASS simulation. Based on 
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

#ifndef MULTIPASSEventAction_h
#define MULTIPASSEventAction_h 1

#include "MULTIPASSEventMessenger.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class G4Event;

class MULTIPASSEventAction : public G4UserEventAction
{
  public:

    MULTIPASSEventAction();
    virtual ~MULTIPASSEventAction();

  public:

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    void SetSaveThreshold(G4int );

    void SetEventVerbose(G4int v){fVerbose=v;}

    void SetPMTThreshold(G4int t){fPMTThreshold=t;}

    void SetForceDrawPhotons(G4bool b){fForcedrawphotons=b;}
    void SetForceDrawNoPhotons(G4bool b){fForcenophotons=b;}

  private:

    MULTIPASSEventMessenger* fEventMessenger;

    G4int              fSaveThreshold;

    G4int              fScintCollID;
    G4int              fPMTCollID;
    G4int              fOptCollID;
    G4int              fComptCollID;

    G4int              fVerbose;

    G4int              fPMTThreshold;

    G4bool fForcedrawphotons;
    G4bool fForcenophotons;

};

#endif
