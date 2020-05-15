//
// File:         MULTIPASS_Det/src/MULTIPASSSteppingMessenger.cc
// Description:  SteppingMessenger for MULTIPASS simulation. Based on original
//               code from Geant4 optical/LXe from 10.4.p01
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

#include "MULTIPASSSteppingMessenger.hh"
#include "MULTIPASSSteppingAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSSteppingMessenger::MULTIPASSSteppingMessenger(
												MULTIPASSSteppingAction* step)
 : fStepping(step)
{
  fOneStepPrimariesCmd = 
  					new G4UIcmdWithABool("/MULTIPASS/oneStepPrimaries",this);
  fOneStepPrimariesCmd->
      SetGuidance("Only allows primaries to go one step before being killed.");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSSteppingMessenger::~MULTIPASSSteppingMessenger(){
  delete fOneStepPrimariesCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void 
MULTIPASSSteppingMessenger::SetNewValue(G4UIcommand* command,G4String newValue){
  if( command == fOneStepPrimariesCmd ){
    fStepping->SetOneStepPrimaries(fOneStepPrimariesCmd
                                  ->GetNewBoolValue(newValue));
  }
}
