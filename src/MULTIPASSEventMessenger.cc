//
// File:         MULTIPASS_Det/src/NaIEventMessenger.cc
// Description:  EventMessenger for MULTIPASS simulation. Based on original
//               code from Geant4 optical/LXe from 10.4.p01
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

#include "MULTIPASSEventMessenger.hh"
#include "MULTIPASSEventAction.hh"

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSEventMessenger::MULTIPASSEventMessenger(MULTIPASSEventAction* event)
 : fEJ276Event(event)
{
  fSaveThresholdCmd = new G4UIcmdWithAnInteger("/MULTIPASS/saveThreshold",this);
  fSaveThresholdCmd->
SetGuidance("Set the photon count threshold for saving the random number seed");
  fSaveThresholdCmd->SetParameterName("photons",true);
  fSaveThresholdCmd->SetDefaultValue(4500);
  fSaveThresholdCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fVerboseCmd = new G4UIcmdWithAnInteger("/MULTIPASS/eventVerbose",this);
  fVerboseCmd->SetGuidance("Set the verbosity of event data.");
  fVerboseCmd->SetParameterName("verbose",true);
  fVerboseCmd->SetDefaultValue(1);

  fPmtThresholdCmd = new G4UIcmdWithAnInteger("/MULTIPASS/pmtThreshold",this);
  fPmtThresholdCmd->SetGuidance("Set the pmtThreshold (in # of photons)");

  fForceDrawPhotonsCmd = 
                      new G4UIcmdWithABool("/MULTIPASS/forceDrawPhotons",this);
  fForceDrawPhotonsCmd->SetGuidance("Force drawing of photons.");
  fForceDrawPhotonsCmd
    ->SetGuidance("(Higher priority than /MULTIPASS/forceDrawNoPhotons)");

  fForceDrawNoPhotonsCmd =
                    new G4UIcmdWithABool("/MULTIPASS/forceDrawNoPhotons",this);
  fForceDrawNoPhotonsCmd->SetGuidance("Force no drawing of photons.");
  fForceDrawNoPhotonsCmd
    ->SetGuidance("(Lower priority than /MULTIPASS/forceDrawPhotons)");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSEventMessenger::~MULTIPASSEventMessenger(){
  delete fSaveThresholdCmd;
  delete fVerboseCmd;
  delete fPmtThresholdCmd;
  delete fForceDrawPhotonsCmd;
  delete fForceDrawNoPhotonsCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSEventMessenger::SetNewValue(G4UIcommand* command, 
                                                            G4String newValue){
  if( command == fSaveThresholdCmd ){
    fEJ276Event->SetSaveThreshold(fSaveThresholdCmd->GetNewIntValue(newValue));
  }
  else if( command == fVerboseCmd ){
    fEJ276Event->SetEventVerbose(fVerboseCmd->GetNewIntValue(newValue));
  }
  else if( command == fPmtThresholdCmd ){
    fEJ276Event->SetPMTThreshold(fPmtThresholdCmd->GetNewIntValue(newValue));
  }
  else if(command == fForceDrawPhotonsCmd){
    fEJ276Event->SetForceDrawPhotons(fForceDrawPhotonsCmd
                                  ->GetNewBoolValue(newValue));
  }
  else if(command == fForceDrawNoPhotonsCmd){
    fEJ276Event->SetForceDrawNoPhotons(fForceDrawNoPhotonsCmd
                                  ->GetNewBoolValue(newValue));
  }
}
