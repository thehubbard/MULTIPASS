//
// File:         MULTIPASS_Det/src/MULTIPASSOpticalHit.cc
// Description:  MULTIPASSOpticalHit for MULTIPASS simulation. 
//               Tool for collecting optical photons at PMT surface

// Version:      1.0.0
// Created:      2020-03-18
// Author:       Michael Hubbard
// Modified:     
//
//
// Tested with:
//              Geant4 v 10.3.p01
//              ROOT v 6.08.00
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//

#include "MULTIPASSOpticalHit.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh" 

G4ThreadLocal G4Allocator<MULTIPASSOpticalHit>* MULTIPASSOpticalHitAllocator=0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSOpticalHit::MULTIPASSOpticalHit()
  : fPmtNumber(-1),fPmtPos(0),fPhysVol(0),fDrawit(false),fOptPos(0),
    fLocalTime(0),fGlobalTime(0), fReflections(0), fPathlength(0.), 
    fEnergy(0.), fStartMom(0) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSOpticalHit::~MULTIPASSOpticalHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSOpticalHit::MULTIPASSOpticalHit(const MULTIPASSOpticalHit &right):
                                                                        G4VHit()
{
  fPmtNumber=right.fPmtNumber;
  fPmtPos=right.fPmtPos;
  fPhysVol=right.fPhysVol;
  fDrawit=right.fDrawit;
  fOptPos=right.fOptPos;
  fLocalTime=right.fLocalTime;
  fGlobalTime=right.fGlobalTime;
  fReflections=right.fReflections;
  fPathlength=right.fPathlength;
  fEnergy=right.fEnergy;
  fStartMom=right.fStartMom;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const MULTIPASSOpticalHit& MULTIPASSOpticalHit::operator=(
                                              const MULTIPASSOpticalHit &right){
  fPmtNumber = right.fPmtNumber;
  fPmtPos=right.fPmtPos;
  fPhysVol=right.fPhysVol;
  fDrawit=right.fDrawit;
  fOptPos=right.fOptPos;
  fLocalTime=right.fLocalTime;
  fGlobalTime=right.fGlobalTime;
  fReflections=right.fReflections;
  fPathlength=right.fPathlength;
  fEnergy=right.fEnergy;
  fStartMom=right.fStartMom;
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int MULTIPASSOpticalHit::operator==(const MULTIPASSOpticalHit &right) const{
  return (this == &right) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSOpticalHit::Draw(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSOpticalHit::Print() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double MULTIPASSOpticalHit::GetWavelength(){
  // hbarc 197.32705e-12 MeV*mm
  G4double hbarc_ev_nm = hbarc*twopi/(1e-6*1e-6);
  G4double wavelength = hbarc_ev_nm/ (fEnergy/eV); // nm
  return wavelength;
}