//
// File:         MULTIPASS_Det/include/
//                                         MULTIPASSActionInitialization.hh
// Description:  ActionInitialization header for MULTIPASS simulation. 
//               Based on original code from Geant4 optical/LXe from 10.4.p01
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

#ifndef MULTIPASSActionInitialization_h
#define MULTIPASSActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "globals.hh"

class B4DetectorConstruction;

/// Action initialization class.
///

class MULTIPASSActionInitialization : public G4VUserActionInitialization
{
  public:
    MULTIPASSActionInitialization(G4String runMode);
    virtual ~MULTIPASSActionInitialization();


    virtual void BuildForMaster() const;
    virtual void Build() const;

    virtual G4VSteppingVerbose* InitializeSteppingVerbose() const;

  private:
    G4String fRunMode;
    
};

#endif
