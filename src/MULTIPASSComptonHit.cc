//
// File:         MULTIPASS_Det/src/MULTIPASSComptonHit.cc
// Description:  ComptHit for MULTIPASS simulation.
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
#include "MULTIPASSComptonHit.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

G4ThreadLocal G4Allocator<MULTIPASSComptonHit>*
                                           MULTIPASSComptonHitAllocator=0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSComptonHit::MULTIPASSComptonHit() : fEdep(0.), fPos(0.), 
                                                      fTime(0.), fProcID(-1) {}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSComptonHit::~MULTIPASSComptonHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSComptonHit::MULTIPASSComptonHit(
                              const MULTIPASSComptonHit &right) : G4VHit()
{
  fEdep = right.fEdep;
  fPos = right.fPos;
  fTime = right.fTime;
  fProcID = right.fProcID;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const MULTIPASSComptonHit& MULTIPASSComptonHit::operator=(
                                        const MULTIPASSComptonHit &right){
  fEdep = right.fEdep;
  fPos = right.fPos;
  fTime = right.fTime;
  fProcID = right.fProcID;
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int MULTIPASSComptonHit::operator==(const MULTIPASSComptonHit&) 
                                                                          const{
  return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSComptonHit::Draw() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSComptonHit::Print() {}
