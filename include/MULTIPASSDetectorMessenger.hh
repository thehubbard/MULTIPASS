//
// File:         MULTIPASS_Det/include/MULTIPASSDetectorMessenger.hh
// Description:  DetectorMessenger header for MULTIPASS simulation. 
//               Based on original code from Geant4 optical/LXe from 10.4.p01
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
#ifndef MULTIPASSDetectorMessenger_h
#define MULTIPASSDetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class MULTIPASSDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcommand;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;

class MULTIPASSDetectorMessenger: public G4UImessenger
{
  public:

    MULTIPASSDetectorMessenger(MULTIPASSDetectorConstruction*);
    virtual ~MULTIPASSDetectorMessenger();

 
    virtual void SetNewValue(G4UIcommand*, G4String);
 
  private:

    MULTIPASSDetectorConstruction*   fEJ276Detector;
    G4UIdirectory*                   fDetectorDir;
    G4UIdirectory*                   fVolumesDir;
    G4UIcmdWithADoubleAndUnit*       fSlabDimCmd;
    G4UIcmdWith3VectorAndUnit*       fDimensionsCmd;
    G4UIcmdWithADoubleAndUnit*       fHousingThicknessCmd;
    G4UIcmdWithADoubleAndUnit*       fPmtRadiusCmd;
    G4UIcmdWithAnInteger*            fNxCmd;
    G4UIcmdWithAnInteger*            fNyCmd;
    G4UIcmdWithAnInteger*            fNzCmd;
    G4UIcmdWithADouble*              fReflectivityCmd;
    G4UIcmdWithABool*                fEJ276Cmd;
    G4UIcommand*                     fDefaultsCmd;
    G4UIcmdWithADouble*              fMainScintYield;
};

#endif
