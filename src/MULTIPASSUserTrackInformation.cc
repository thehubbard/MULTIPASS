//
// File:         MULTIPASS_Det/src/MULTIPASSUserTrackInformation.cc
// Description:  UserTrackInformation for MULTIPASS simulation. Based on
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

#include "MULTIPASSUserTrackInformation.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSUserTrackInformation::MULTIPASSUserTrackInformation()
  : fStatus(active),fReflections(0),fForcedraw(false),fComptScat(0)
{
  originalPosition = G4ThreeVector(0.,0.,0.);
  originalMomentum = G4ThreeVector(0.,0.,0.);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSUserTrackInformation::MULTIPASSUserTrackInformation(
                                                          const G4Track* aTrack)
  : fStatus(active),fReflections(0),fForcedraw(false),fComptScat(0) 
{
  originalPosition = aTrack->GetPosition();
  G4ThreeVector originalMomentum_not_unitless = aTrack->GetMomentum(); 
  originalMomentum = originalMomentum_not_unitless/
                                            originalMomentum_not_unitless.mag();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSUserTrackInformation::~MULTIPASSUserTrackInformation() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSUserTrackInformation::AddTrackStatusFlag(int s)
{
  if(s&active) //track is now active
    fStatus&=~inactive; //remove any flags indicating it is inactive
  else if(s&inactive) //track is now inactive
    fStatus&=~active; //remove any flags indicating it is active
  fStatus|=s; //add new flags
}
