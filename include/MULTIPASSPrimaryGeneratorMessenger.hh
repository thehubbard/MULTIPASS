//
// File:         MULTIPASS_Det/include/
//                                    MULTIPASSPrimaryGeneratorMessenger.hh
// Description:  PrimaryGeneratorMessenger header for MULTIPASS simulation. 
//
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
#ifndef MULTIPASSPrimaryGeneratorMessenger_h
#define MULTIPASSPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MULTIPASSPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MULTIPASSPrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    MULTIPASSPrimaryGeneratorMessenger(
                                        MULTIPASSPrimaryGeneratorAction* );
    virtual ~MULTIPASSPrimaryGeneratorMessenger();
 
    virtual void SetNewValue(G4UIcommand*, G4String);
 
  private:

    MULTIPASSPrimaryGeneratorAction* fMULTIPASSAction;

    G4UIdirectory*                  fGunDir;
    G4UIcmdWithAString*             fParticleTCModel;
  	G4UIcmdWithADoubleAndUnit*      fParticleEnergyCmd;
  	G4UIcmdWithADouble*             fScintYieldCmd;
    G4UIcmdWithAString*             fOriginCmd;
    G4UIcmdWithADoubleAndUnit*      fOffsetCmd;
    G4UIcmdWithADouble*             fWavelengthCmd;

    G4UIcmdWithAnInteger*           fScanTableXCmd;
    G4UIcmdWithAnInteger*           fScanTableYCmd;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif