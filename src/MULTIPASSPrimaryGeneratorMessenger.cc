//
// File:         MULTIPASS_Det/src/MULTIPASSPrimaryGeneratorMessenger.cc
// Description:  PrimaryGeneratorMessenger for MULTIPASS simulation. 
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

#include "MULTIPASSPrimaryGeneratorMessenger.hh"

#include "MULTIPASSPrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSPrimaryGeneratorMessenger::
  MULTIPASSPrimaryGeneratorMessenger(
                                  MULTIPASSPrimaryGeneratorAction* MULTIPASSGun)
  : G4UImessenger(),
    fMULTIPASSAction(MULTIPASSGun)
{
  fGunDir = new G4UIdirectory("/MULTIPASS/gun/");
  fGunDir->SetGuidance("PrimaryGenerator control");

  fParticleTCModel = new G4UIcmdWithAString("/MULTIPASS/gun/particle",this);
  fParticleTCModel->SetGuidance(
                        "Set particle for time constant model (Neutron,Gamma)");
  fParticleTCModel->SetParameterName("tc_particle",true);
  fParticleTCModel->SetDefaultValue("Gamma");
  fParticleTCModel->SetCandidates( "Gamma Neutron" );
  fParticleTCModel->AvailableForStates(G4State_PreInit,G4State_Idle);

  fParticleEnergyCmd 
          = new G4UIcmdWithADoubleAndUnit("/MULTIPASS/gun/particleEnergy",this);
  fParticleEnergyCmd->SetGuidance(
                          "Set Particle Energy for optical photon generation");
  fParticleEnergyCmd->SetParameterName("pEn",false);
  fParticleEnergyCmd->SetRange("pEn>0.");
  fParticleEnergyCmd->SetUnitCategory("Energy");
  fParticleEnergyCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fParticleEnergyCmd->SetToBeBroadcasted(false);

  fScintYieldCmd
            = new G4UIcmdWithADouble("/MULTIPASS/gun/scintYield",this);
  fScintYieldCmd->SetGuidance(
    "Set scintillation yield for optical photon generation in photons / MeV");
  fScintYieldCmd->SetParameterName("sY",false);
  fScintYieldCmd->SetRange("sY>0.");
  fScintYieldCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fScintYieldCmd->SetToBeBroadcasted(false);

  fOriginCmd= new G4UIcmdWithAString("/MULTIPASS/gun/InteractionOrigin",this);
  fOriginCmd->SetGuidance(
                        "Changed to named origin for scintillation point.");
  fOriginCmd->SetParameterName("Location",true);
  fOriginCmd->SetDefaultValue("Center");
  fOriginCmd->SetCandidates(
    "Center Back Top Topback Front Topfront Side Sideback Sidefront Cornerback\
     Cornerfront Edge" );
  fOriginCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fOffsetCmd 
            = new G4UIcmdWithADoubleAndUnit("/MULTIPASS/gun/offset",this);
  fOffsetCmd->SetGuidance(
                        "Set Offset for use in InteractionOrigin calculation.");
  fOffsetCmd->SetGuidance(
                  "(Slab side  half length -  offset )is distance from center");
  fOffsetCmd->SetParameterName("off",false);
  fOffsetCmd->SetRange("off>0.");
  fOffsetCmd->SetUnitCategory("Length");
  fOffsetCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fOffsetCmd->SetToBeBroadcasted(false);


  fWavelengthCmd
            = new G4UIcmdWithADouble("/MULTIPASS/gun/wavelength",this);
  fWavelengthCmd->SetGuidance(
                      "Set wavelength for optical photon generated (nm)");
  fWavelengthCmd->SetParameterName("wl",false);
  // 10 nm to 400 nm UV
  // 400 nm to 700 nm Visible light
  // 700 nm to 1mm IR
  fWavelengthCmd->SetRange("wl > 10 && wl < 700");
  fWavelengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fWavelengthCmd->SetToBeBroadcasted(false);

  fScanTableXCmd = new G4UIcmdWithAnInteger("/MULTIPASS/gun/ScanTableX",this);
  fScanTableXCmd->SetGuidance(
                      "Set scanning table X location (mm)");
  fScanTableXCmd->SetParameterName("sx",false);

  fScanTableXCmd->SetRange("sx >= 0 && sx <= 254");
  fScanTableXCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fScanTableXCmd->SetToBeBroadcasted(false);

  fScanTableYCmd = new G4UIcmdWithAnInteger("/MULTIPASS/gun/ScanTableY",this);
  fScanTableYCmd->SetGuidance(
                      "Set scanning table Y location (mm)");
  fScanTableYCmd->SetParameterName("sy",false);
  fScanTableYCmd->SetRange("sy >= 0 && sy <= 254");
  fScanTableYCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fScanTableYCmd->SetToBeBroadcasted(false);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSPrimaryGeneratorMessenger::~MULTIPASSPrimaryGeneratorMessenger()
{
  delete fParticleTCModel;
  delete fParticleEnergyCmd;
  delete fScintYieldCmd;
  delete fOffsetCmd;  
  delete fWavelengthCmd;
  delete fScanTableXCmd;
  delete fScanTableYCmd;
  delete fGunDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSPrimaryGeneratorMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{
  if( command == fParticleTCModel ) {
    if(newValue=="Gamma" || newValue=="Neutron") {  
      fMULTIPASSAction->SetTCModelParticle(newValue);
    } else {
      G4cout << "You humans act so strange.\
            Everything you create is used to destroy." << G4endl;
    }
  }
  else if (command == fParticleEnergyCmd){
   fMULTIPASSAction->SetEparticle(
                              fParticleEnergyCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fScintYieldCmd){
   fMULTIPASSAction->SetScintYield(fScintYieldCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fOriginCmd){
   fMULTIPASSAction->SetOrigin(newValue);
  }
  else if (command == fOffsetCmd){
   fMULTIPASSAction->SetOffset(fOffsetCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fWavelengthCmd){
    // converted to nm in SetWavelength
   fMULTIPASSAction->SetWavelength(fWavelengthCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fScanTableXCmd){
    // converted to nm in SetWavelength
   fMULTIPASSAction->SetScanOffsetX(fScanTableXCmd->GetNewIntValue(newValue));
  }
  else if (command == fScanTableYCmd){
    // converted to nm in SetWavelength
   fMULTIPASSAction->SetScanOffsetY(fScanTableYCmd->GetNewIntValue(newValue));
  }


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......