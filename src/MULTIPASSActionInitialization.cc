//
// File:         MULTIPASS_Det/src/MULTIPASSActionInitialization.cc
// Description:  ActionInitialization for MULTIPASS simulation. Based on
//				 original code from Geant4 optical/LXe from 10.4.p01
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

#include "MULTIPASSActionInitialization.hh"

#include "MULTIPASSPrimaryGeneratorAction.hh"

#include "MULTIPASSRunAction.hh"
#include "MULTIPASSEventAction.hh"
#include "MULTIPASSTrackingAction.hh"
#include "MULTIPASSSteppingAction.hh"
#include "MULTIPASSStackingAction.hh"
#include "MULTIPASSSteppingVerbose.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSActionInitialization::MULTIPASSActionInitialization(
		                                                          G4String runMode)
 : G4VUserActionInitialization()
{
	fRunMode = runMode;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSActionInitialization::~MULTIPASSActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSActionInitialization::BuildForMaster() const
{
  SetUserAction(new MULTIPASSRunAction(fRunMode));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSActionInitialization::Build() const
{
  SetUserAction(new MULTIPASSPrimaryGeneratorAction(fRunMode));

  SetUserAction(new MULTIPASSStackingAction());

  SetUserAction(new MULTIPASSRunAction(fRunMode));
  SetUserAction(new MULTIPASSEventAction());
  SetUserAction(new MULTIPASSTrackingAction());
  SetUserAction(new MULTIPASSSteppingAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VSteppingVerbose* 
          MULTIPASSActionInitialization::InitializeSteppingVerbose() const
{
  return new MULTIPASSSteppingVerbose();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
