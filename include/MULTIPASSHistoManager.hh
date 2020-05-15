//
// File:         MULTIPASS/include/MULTIPASSHistoManager.hh
// Description:  Analysis header for MULTIPASS simulation.
//         Defines analysis output for Geant4
//         Options are ROOT, csv or xml
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

#ifndef MULTIPASSHistoManager_H
#define MULTIPASSHistoManager_H 1

#include "g4root.hh"
// #include "g4csv.hh"
// #include "g4xml.hh"
// #include "g4hdf5.hh"

#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MULTIPASSHistoManager
{
  public:
    MULTIPASSHistoManager();
   ~MULTIPASSHistoManager();

  void InitOpticalMode();

  void BeginOfRun();
  void EndOfRun(G4bool);

  inline void SetFileName(G4String fn) { fFileName = fn; }
  G4String GetFileName() const { return fFileName; }

  inline void SetNumberOfEvents(G4int ne) { fNEvents = ne; }
  G4int GetNumberOfEvents() const { return fNEvents; }

  private:
    G4AnalysisManager*     fAnalysisManager;
    G4String               fFileName;
    G4int                  fNEvents;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif