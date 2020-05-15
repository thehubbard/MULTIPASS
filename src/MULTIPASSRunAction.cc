//
// File:         MULTIPASS_Det/src/MULTIPASSRunAction.cc
// Description:  RunAction for MULTIPASS simulation. Based on original
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

#include "MULTIPASSRunAction.hh"
#include "MULTIPASSDetectorConstruction.hh"
#include "MULTIPASSPrimaryGeneratorAction.hh"
#include "MULTIPASSHistoManager.hh"


#include "G4Run.hh" 

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"

#include "G4SystemOfUnits.hh"    // For units keV etc.
#include "G4UIcmdWithAString.hh" // Converts G4types to string
#include "G4UnitsTable.hh"       // For G4BestUnit
#include "Randomize.hh"


#include <iomanip>
#include <ctime>
#include <sstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSRunAction::MULTIPASSRunAction(G4String runMode): G4UserRunAction()
{
  fHisto = new MULTIPASSHistoManager();
  fRunMode = runMode;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSRunAction::~MULTIPASSRunAction()
{

    delete fHisto;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSRunAction::BeginOfRunAction(const G4Run* aRun)
{
  // save Rndm status
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  if (isMaster) G4Random::showEngineStatus();

  // Note this is not thread safe
  std::time_t t = std::time(nullptr);
  std::ostringstream time_stream;
  time_stream << std::put_time(std::localtime(&t), "%Y%m%dT%H%M%S%Z");

  G4RunManager * runManager =G4RunManager::GetRunManager();
  G4String runMode;
  if (runManager->GetRunManagerType() == G4RunManager::sequentialRM) {
    // G4cout << "Sequential mode: "<<G4endl;
    runMode="Seq";
  } else {
    // MT disabled for this example but this is how to add MT to name on output
    // files
    // G4cout << "MT mode: "<<G4endl;
    G4String threads =  G4UIcommand::ConvertToString(
               static_cast<G4MTRunManager*>(runManager)->GetNumberOfThreads());
    runMode="MT"+threads;
  }


  const MULTIPASSPrimaryGeneratorAction* primary =
    static_cast<const MULTIPASSPrimaryGeneratorAction*>(
      runManager->GetUserPrimaryGeneratorAction());

  // Open an output file
  G4int nbEventInRun = aRun->GetNumberOfEventToBeProcessed();
  G4String fileName = "";

  fileName += fRunMode;
  fileName += "_MULTIPASS_";

  fileName += runMode;
  fileName += "_Events_";
  fileName += G4UIcommand::ConvertToString(nbEventInRun); 

  // Add creation time to file if needed
  // fileName += time_stream.str();


  if (fRunMode != "Gamma" && fRunMode != "Full" ) {
    fileName+="_Origin-";
    fileName+=primary->GetOrigin();
  } else{
    fileName+="_ScanTable_X";
    fileName+=PadZeros(G4UIcommand::ConvertToString(primary->GetScanOffsetX()));
    fileName+="_Y";
    fileName+=PadZeros(G4UIcommand::ConvertToString(primary->GetScanOffsetY()));
  }
  fHisto->SetNumberOfEvents(nbEventInRun);
  fHisto->InitOpticalMode();

  fHisto->SetFileName(fileName);


  fHisto->BeginOfRun();

  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl; 


}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSRunAction::EndOfRunAction(const G4Run* aRun)
{
  // show Rndm status
  if (isMaster) G4Random::showEngineStatus(); 
  G4cout << "number of event " << aRun->GetNumberOfEvent()
         << " start." << G4endl; 

  fHisto->EndOfRun(isMaster);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4String MULTIPASSRunAction::PadZeros(G4String unpad)
{
  // Pad Zeros to string for example 3 becomes 003
  G4String padded = unpad;
  padded.insert(padded.begin(), 3 - padded.length(), '0');
  return padded;
}

