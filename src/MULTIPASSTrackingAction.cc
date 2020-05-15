//
// File:         MULTIPASS_Det/src/MULTIPASSTrackingAction.cc
// Description:  TrackingAction for MULTIPASS simulation. Based on original
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

#include "MULTIPASSTrajectory.hh"
#include "MULTIPASSTrackingAction.hh"
#include "MULTIPASSUserTrackInformation.hh"
#include "MULTIPASSDetectorConstruction.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSTrackingAction::MULTIPASSTrackingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  //Let this be up to the user via vis.mac
  //  fpTrackingManager->SetStoreTrajectory(true);

  //Use custom trajectory class
  fpTrackingManager->SetTrajectory(new MULTIPASSTrajectory(aTrack));

  //This user track information is only relevant to the photons
  fpTrackingManager->SetUserTrackInformation(
                                    new MULTIPASSUserTrackInformation(aTrack));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSTrackingAction::PostUserTrackingAction(const G4Track* aTrack){
  MULTIPASSTrajectory* trajectory=
                    (MULTIPASSTrajectory*)fpTrackingManager->GimmeTrajectory();
  MULTIPASSUserTrackInformation* trackInformation = 
              (MULTIPASSUserTrackInformation*)aTrack->GetUserInformation();

  // Draw only the photons that hit the pmt
  if(aTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition()){

    const G4VProcess* creator=aTrack->GetCreatorProcess();
    if(creator && creator->GetProcessName()=="OpWLS"){
      trajectory->WLS();
      trajectory->SetDrawTrajectory(true);
    }
    
    if(trackInformation->GetTrackStatus()&hitPMT)
      trajectory->SetDrawTrajectory(true);
    
  }
  else //draw all other trajectories
    trajectory->SetDrawTrajectory(true);

  if(trackInformation->GetForceDrawTrajectory())
    trajectory->SetDrawTrajectory(true);


}
