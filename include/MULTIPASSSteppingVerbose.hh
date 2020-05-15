//
// File:         MULTIPASS_Det/include/MULTIPASSSteppingVerbose.hh
// Description:  SteppingVerbose header for MULTIPASS simulation. Based on 
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

#ifndef MULTIPASSSteppingVerbose_h
#define MULTIPASSSteppingVerbose_h 1

#include "G4SteppingVerbose.hh"

class MULTIPASSSteppingVerbose : public G4SteppingVerbose
{
  public:

    MULTIPASSSteppingVerbose();
    virtual ~MULTIPASSSteppingVerbose();

    virtual void StepInfo();
    virtual void TrackingStarted();

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
