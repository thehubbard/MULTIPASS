//
// File:         MULTIPASS_Det/include/MULTIPASSPhysicsList.hh
// Description:  PhysicsList header for MULTIPASS simulation. Based on 
//				 original code from Geant4 optical/LXe from 10.4.p01
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
#ifndef MULTIPASSPhysicsList_h
#define MULTIPASSPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"


class MULTIPASSPhysicsList: public G4VModularPhysicsList
{
  public:

    MULTIPASSPhysicsList(G4bool optics);
    virtual ~MULTIPASSPhysicsList();

  public:

    virtual void SetCuts();

};

#endif
