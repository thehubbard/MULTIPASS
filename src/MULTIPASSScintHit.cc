//
// File:         MULTIPASS_Det/src/MULTIPASSScintHit.cc
// Description:  ScintHit for MULTIPASS simulation. Based on original
//               code from Geant4 optical/LXe from 10.4.p01
// Version:      1.0.0
// Created:      2020-03-18
// Author:       Michael Hubbard
// Modified:     2019-05-22 MHUBBARD
//               > Added Time 
//
//
// Tested with:
//              Geant4 v 10.4.p01
//              ROOT v 6.12.06
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
#include "MULTIPASSScintHit.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

G4ThreadLocal G4Allocator<MULTIPASSScintHit>* MULTIPASSScintHitAllocator=0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSScintHit::MULTIPASSScintHit() : fEdep(0.), fPos(0.), fPhysVol(0), 
                                                                    fTime(0) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSScintHit::MULTIPASSScintHit(G4VPhysicalVolume* pVol):fPhysVol(pVol) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSScintHit::~MULTIPASSScintHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSScintHit::MULTIPASSScintHit(const MULTIPASSScintHit &right) : G4VHit()
{
  fEdep = right.fEdep;
  fPos = right.fPos;
  fPhysVol = right.fPhysVol;
  fTime = right.fTime;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const MULTIPASSScintHit& MULTIPASSScintHit::operator=(
                                                const MULTIPASSScintHit &right){
  fEdep = right.fEdep;
  fPos = right.fPos;
  fPhysVol = right.fPhysVol;
  fTime = right.fTime;
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int MULTIPASSScintHit::operator==(const MULTIPASSScintHit&) const{
  return false;
  //returns false because there currently isnt need to check for equality yet
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSScintHit::Draw() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSScintHit::Print() {}
