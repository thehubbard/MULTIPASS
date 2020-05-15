//
// File:         MULTIPASS_Det/src/MULTIPASSPhysicsList.cc
// Description:  PhysicsList for MULTIPASS simulation. Based on original
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

#include "MULTIPASSPhysicsList.hh"


#include "G4PhysListFactory.hh"

#include "G4OpticalPhysicsMod.hh"
#include "G4OpticalProcessIndex.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSPhysicsList::MULTIPASSPhysicsList(G4bool optics) : G4VModularPhysicsList()
{
  // default cut value  (1.0mm)
  defaultCutValue = 1.0*mm;



  G4PhysListFactory physListFactory;
  G4VModularPhysicsList* phys = 
                        physListFactory.GetReferencePhysList("QGSP_BIC_HP_EMZ");
  // EMZ includes option 4 of EM
  //
  // Some EM options:
  // No suffix: standard EM i.e. the default G4EmStandardPhysics constructor
  // “EMV” G4EmStandardPhysics_option1 CTR: HEP, fast but less precise
  // “EMY” G4EmStandardPhysics_option3 CTR: medical, space sci., precise
  // “EMZ” G4EmStandardPhysics_option4 CTR: most precise EM physics 

  // Adds everything from prebuilt physics list
  for (G4int i = 0; ; ++i) {
     G4VPhysicsConstructor* elem =
                const_cast<G4VPhysicsConstructor*> (phys->GetPhysics(i));
     if (elem == NULL) break;
     G4cout << "RegisterPhysics: " << elem->GetPhysicsName() << G4endl;
     RegisterPhysics(elem);
  }

  // If simulation uses optical photons
  if (optics) {
    // Optical Physics
    G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
    // opticalPhysics->SetVerboseLevel(1);
    RegisterPhysics( opticalPhysics );
    // opticalPhysics->SetScintillationByParticleType(true);
    
    opticalPhysics->SetWLSTimeProfile("delta");
  
    opticalPhysics->SetScintillationYieldFactor(1.0);
    opticalPhysics->SetScintillationExcitationRatio(0.0);
  
    opticalPhysics->SetMaxNumPhotonsPerStep(100);
    opticalPhysics->SetMaxBetaChangePerStep(10.0);
  
    opticalPhysics->SetTrackSecondariesFirst(kCerenkov,true);//true
    opticalPhysics->SetTrackSecondariesFirst(kScintillation,true);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSPhysicsList::~MULTIPASSPhysicsList() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSPhysicsList::SetCuts(){
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets
  //   the default cut value for all particle types
  SetCutsWithDefault();
}
