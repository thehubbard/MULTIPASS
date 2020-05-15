//
// File:         MULTIPASS_Det/include/MULTIPASSRunAction.hh
// Description:  RunAction header for MULTIPASS simulation. Based on
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
//

#ifndef MULTIPASSRunAction_h
#define MULTIPASSRunAction_h 1


#include "G4UserRunAction.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;
class MULTIPASSHistoManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MULTIPASSRunAction : public G4UserRunAction
{
  public:

    MULTIPASSRunAction(G4String runMode);
    virtual ~MULTIPASSRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

  private:
  	G4String PadZeros(G4String);

  private:
  	  MULTIPASSHistoManager*         fHisto;

  	  G4String fRunMode;

};

#endif
