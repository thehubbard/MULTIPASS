//
// File:         MULTIPASS_Det/src/MULTIPASSUserEventInformation.cc
// Description:  UserEventInformation for MULTIPASS simulation. Based on 
//               original code from Geant4 optical/LXe from 10.4.p01
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

#include "MULTIPASSUserEventInformation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSUserEventInformation::MULTIPASSUserEventInformation()
  :fHitCount(0),fPhotonCount_Scint(0),fPhotonCount_Ceren(0),fAbsorptionCount(0),
   fBoundaryAbsorptionCount(0), fReflections(0),fTotE(0.),fEWeightPos(0.),
   fReconPos(0.),fConvPos(0.), fConvPosSet(false),fPosMax(0.),fEdepMax(0.),
   fEscapeEn(0.),fComptScat(0),
   fPMTsAboveThreshold(0) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSUserEventInformation::~MULTIPASSUserEventInformation() {}
