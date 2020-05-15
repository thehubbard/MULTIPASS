//
// File:         MULTIPASS_Det/src/MULTIPASSDetectorMessenger.cc
// Description:  DetectorMessenger for MULTIPASS simulation. Based on 
//               original code from Geant4 optical/LXe from 10.4.p01
// Version:      1.0.0
// Created:      2020-03-18
// Author:       Michael Hubbard
// Modified:     
//
//
//
// Tested with:
//              Geant4 v 10.4.p01
//              ROOT v 6.12.06
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//

#include "MULTIPASSDetectorMessenger.hh"
#include "MULTIPASSDetectorConstruction.hh"

#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4Scintillation.hh"

#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSDetectorMessenger::MULTIPASSDetectorMessenger(
                                  MULTIPASSDetectorConstruction* detector)
 : fEJ276Detector(detector)
{
  //Setup a command directory for detector controls with guidance
  fDetectorDir = new G4UIdirectory("/MULTIPASS/detector/");
  fDetectorDir->SetGuidance("Detector geometry control");

  fVolumesDir = new G4UIdirectory("/MULTIPASS/detector/volumes/");
  fVolumesDir->SetGuidance("Enable/disable volumes");
 
  //Various commands for modifying detector geometry
  fSlabDimCmd =
    new G4UIcmdWithADoubleAndUnit("/MULTIPASS/detector/slabdim",this);
  fSlabDimCmd->SetGuidance(
    "Set the dimensions of the detector slab, aspect ratio 5:10:2,\
     where x is the 10.");
  fSlabDimCmd->SetParameterName("scint_x",false);
  fSlabDimCmd->SetDefaultUnit("mm");
  fSlabDimCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSlabDimCmd->SetToBeBroadcasted(false);


  fDimensionsCmd =
    new G4UIcmdWith3VectorAndUnit("/MULTIPASS/detector/dimensions",this);
  fDimensionsCmd->SetGuidance("Set the dimensions of the detector volume.");
  fDimensionsCmd->SetParameterName("scint_x","scint_y","scint_z",false);
  fDimensionsCmd->SetDefaultUnit("cm");
  fDimensionsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fDimensionsCmd->SetToBeBroadcasted(false);

  fHousingThicknessCmd = new G4UIcmdWithADoubleAndUnit
    ("/MULTIPASS/detector/housingThickness",this);
  fHousingThicknessCmd->SetGuidance("Set the thickness of the housing.");
  fHousingThicknessCmd->SetParameterName("d_mtl",false);
  fHousingThicknessCmd->SetDefaultUnit("cm");
  fHousingThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fHousingThicknessCmd->SetToBeBroadcasted(false);

  fPmtRadiusCmd = new G4UIcmdWithADoubleAndUnit
    ("/MULTIPASS/detector/pmtRadius",this);
  fPmtRadiusCmd->SetGuidance("Set the radius of the PMTs.");
  fPmtRadiusCmd->SetParameterName("radius",false);
  fPmtRadiusCmd->SetDefaultUnit("cm");
  fPmtRadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fPmtRadiusCmd->SetToBeBroadcasted(false);

  fNxCmd = new G4UIcmdWithAnInteger("/MULTIPASS/detector/nx",this);
  fNxCmd->SetGuidance("Set the number of PMTs along the x-dimension.");
  fNxCmd->SetParameterName("nx",false);
  fNxCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fNxCmd->SetToBeBroadcasted(false);

  fNyCmd = new G4UIcmdWithAnInteger("/MULTIPASS/detector/ny",this);
  fNyCmd->SetGuidance("Set the number of PMTs along the y-dimension.");
  fNyCmd->SetParameterName("ny",false);
  fNyCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fNyCmd->SetToBeBroadcasted(false);

  fNzCmd = new G4UIcmdWithAnInteger("/MULTIPASS/detector/nz",this);
  fNzCmd->SetGuidance("Set the number of PMTs along the z-dimension.");
  fNzCmd->SetParameterName("nz",false);
  fNzCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fNzCmd->SetToBeBroadcasted(false);

  fReflectivityCmd = 
                new G4UIcmdWithADouble("/MULTIPASS/detector/reflectivity",this);
  fReflectivityCmd->SetGuidance("Set the reflectivity of the housing.");
  fReflectivityCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fReflectivityCmd->SetToBeBroadcasted(false);


  fEJ276Cmd = 
            new G4UIcmdWithABool("/MULTIPASS/detector/volumes/MULTIPASS",this);
  fEJ276Cmd->SetGuidance("Enable/Disable the main detector volume.");
  fEJ276Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fEJ276Cmd->SetToBeBroadcasted(false);

  fMainScintYield = 
              new G4UIcmdWithADouble("/MULTIPASS/detector/MainScintYield",this);
  fMainScintYield->SetGuidance("Set scinitillation yield of main volume.");
  fMainScintYield->SetGuidance("Specified in photons/MeV");
  fMainScintYield->AvailableForStates(G4State_PreInit,G4State_Idle);
  fMainScintYield->SetToBeBroadcasted(false);

  fDefaultsCmd = new G4UIcommand("/MULTIPASS/detector/defaults",this);
  fDefaultsCmd->SetGuidance("Set all detector geometry values to defaults.");
  fDefaultsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fDefaultsCmd->SetToBeBroadcasted(false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSDetectorMessenger::~MULTIPASSDetectorMessenger()
{
  delete fSlabDimCmd;
  delete fDimensionsCmd;
  delete fHousingThicknessCmd;
  delete fPmtRadiusCmd;
  delete fNxCmd;
  delete fNyCmd;
  delete fNzCmd;
  delete fDetectorDir;
  delete fVolumesDir;
  delete fEJ276Cmd;
  delete fReflectivityCmd;
  delete fMainScintYield;
  delete fDefaultsCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MULTIPASSDetectorMessenger::SetNewValue(G4UIcommand* command, 
                                                              G4String newValue)
{
  if( command == fDimensionsCmd ){
    fEJ276Detector->SetDimensions(fDimensionsCmd->GetNew3VectorValue(newValue));
  }
  else if (command == fSlabDimCmd){
    fEJ276Detector->SetSlabDimensions(fSlabDimCmd
                                     ->GetNewDoubleValue(newValue));
  }
  else if (command == fHousingThicknessCmd){
    fEJ276Detector->SetHousingThickness(fHousingThicknessCmd
                                     ->GetNewDoubleValue(newValue));
  }
  else if (command == fPmtRadiusCmd){
    fEJ276Detector->SetPMTRadius(fPmtRadiusCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fNxCmd){
    fEJ276Detector->SetNX(fNxCmd->GetNewIntValue(newValue));
  }
  else if (command == fNyCmd){
    fEJ276Detector->SetNY(fNyCmd->GetNewIntValue(newValue));
  }
  else if (command == fNzCmd){
    fEJ276Detector->SetNZ(fNzCmd->GetNewIntValue(newValue));
  }
  else if (command == fReflectivityCmd){
    fEJ276Detector
      ->SetHousingReflectivity(fReflectivityCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fEJ276Cmd){
    fEJ276Detector->SetMainVolumeOn(fEJ276Cmd->GetNewBoolValue(newValue));
  }
  else if (command == fMainScintYield){
   fEJ276Detector->
              SetMainScintYield(fMainScintYield->GetNewDoubleValue(newValue));
  }
  else if (command == fDefaultsCmd){
    fEJ276Detector->SetDefaults();

    G4RunManager::GetRunManager()->ReinitializeGeometry(); 
  }
}
