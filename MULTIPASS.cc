//
// File:         MULTIPASS/MULTIPASS.cc
// Description:  Main program for MULTIPASS simulation. Based on original code 
//               from Geant4 optical/LXe from 10.4.p01
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

// MT doesn't appear to work with this example
// Therefore uses regular sequential mode
#include "G4RunManager.hh"


#include "G4UImanager.hh"
#include "G4String.hh"

#include "MULTIPASSPhysicsList.hh"
#include "MULTIPASSDetectorConstruction.hh"

#include "MULTIPASSActionInitialization.hh"

#include "Randomize.hh"
#include "time.h"

#ifdef G4VIS_USE
  #include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
  #include "G4UIExecutive.hh"
#endif

#include "cxxopts.hpp"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv)
{

  // Note on use of cxx:
  // Options are declared with a long and an optional short option. 
  // A description must be provided. 
  // The third argument is the value, if omitted it is boolean. 
  // Any type can be given as long as it can be parsed, with operator>>.
  cxxopts::Options options(argv[0],
                        " - a simulation software for MULTIPASS detector");
  options.positional_help("[optional args]")
      .show_positional_help();

  options.add_options()
    ("m,macro", "run macro", cxxopts::value<std::string>())
    ("r,runmode", "Run mode [Full, Gamma, Optics]", 
                         cxxopts::value<std::string>()->default_value("Optics"))
    ("h,help", "Print help")
    ;

  auto result = options.parse(argc, argv);
  if (result.count("help"))
  {
    G4cout << options.help({""}) << G4endl;
    exit(0);
  }
  // If macro detected run in batch mode
  G4bool interactiveMode = not(result.count("macro") > 0);


  // Run mode options

  G4String runMode = result["runmode"].as<std::string>();

  std::vector<G4String> runOpt = {"Full", "Gamma", "Optics"};

  G4bool runOptOK = false;

  for (const G4String& i : runOpt) { // access by const reference
    if (runMode==i ) runOptOK = true;
  }
  if (not runOptOK) {
    G4cout << runMode << " is not a valid runmode" << G4endl;
    G4cout << "Valid options: ";
    G4String delim = "";
    for (const G4String& i : runOpt) { // access by const reference
      G4cout << delim << i;
      delim = ", ";
    }
    G4cout << G4endl; 
    exit(1);
  }

  // If mode is Gamma optical physics is disabled
  G4bool enableOptics = (runMode!="Gamma") ; 


  //set random seed with system time
  G4long myseed = time(NULL);

  // Choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  // Seed the random number generator manually
  G4Random::setTheSeed(myseed);

  G4RunManager * runManager = new G4RunManager;

  runManager->SetUserInitialization(
                              new MULTIPASSDetectorConstruction());
  runManager->SetUserInitialization(
                              new MULTIPASSPhysicsList(enableOptics));

  runManager->SetUserInitialization(
                              new MULTIPASSActionInitialization(runMode));


  runManager->Initialize();
  
#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif


 
  // get the pointer to the UI manager and set verbosities
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if(interactiveMode){
#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute vis.mac");
#endif
    if (ui->IsGUI())
       UImanager->ApplyCommand("/control/execute gui.mac");
    ui->SessionStart();
    delete ui;
#endif
  }
  else{
    G4String command = "/control/execute ";
    G4String filename = result["macro"].as<std::string>();
    UImanager->ApplyCommand(command+filename);
  }

#ifdef G4VIS_USE
  delete visManager;
#endif

  // job termination
  delete runManager;
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
