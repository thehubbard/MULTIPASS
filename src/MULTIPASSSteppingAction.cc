//
// File:         MULTIPASS_Det/src/MULTIPASSSteppingAction.cc
// Description:  SteppingAction for MULTIPASS simulation. Based on original
//               code from Geant4 optical/LXe from 10.4.p01
// Version:      1.0.0
// Created:      2020-03-18
// Author:       Michael Hubbard
// Modified:     
//
//
//
// Tested with:
//              Geant4 v 10.4.p01
//              ROOT v 6.12.06
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//

#include "MULTIPASSSteppingAction.hh"
#include "MULTIPASSEventAction.hh"
#include "MULTIPASSTrackingAction.hh"
#include "MULTIPASSTrajectory.hh"
#include "MULTIPASSPMTSD.hh"
#include "MULTIPASSScintSD.hh"
#include "MULTIPASSUserTrackInformation.hh"
#include "MULTIPASSUserEventInformation.hh"
#include "MULTIPASSSteppingMessenger.hh"

#include "G4SteppingManager.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSSteppingAction::MULTIPASSSteppingAction()
  : fOneStepPrimaries(false)
{
  fSteppingMessenger = new MULTIPASSSteppingMessenger(this);

  fExpectedNextStatus = Undefined;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSSteppingAction::~MULTIPASSSteppingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSSteppingAction::UserSteppingAction(const G4Step * theStep){

  G4Track* theTrack = theStep->GetTrack();

  if ( theTrack->GetCurrentStepNumber() == 1 ) fExpectedNextStatus = Undefined;
 
  MULTIPASSUserTrackInformation* trackInformation
    =(MULTIPASSUserTrackInformation*)theTrack->GetUserInformation();
  MULTIPASSUserEventInformation* eventInformation
    =(MULTIPASSUserEventInformation*)G4EventManager::GetEventManager()
    ->GetConstCurrentEvent()->GetUserInformation();

  G4StepPoint* thePrePoint = theStep->GetPreStepPoint();
  G4VPhysicalVolume* thePrePV = thePrePoint->GetPhysicalVolume();

  G4StepPoint* thePostPoint = theStep->GetPostStepPoint();
  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();

  G4OpBoundaryProcessStatus boundaryStatus=Undefined;
  static G4ThreadLocal G4OpBoundaryProcess* boundary=NULL;

  // Find the boundary process only once
  if(!boundary){
    G4ProcessManager* pm
      = theStep->GetTrack()->GetDefinition()->GetProcessManager();
    G4int nprocesses = pm->GetProcessListLength();
    G4ProcessVector* pv = pm->GetProcessList();
    G4int i;
    for( i=0;i<nprocesses;i++){
      if((*pv)[i]->GetProcessName()=="OpBoundary"){
        boundary = (G4OpBoundaryProcess*)(*pv)[i];
        break;
      }
    }
  }

  if(theTrack->GetParentID()==0){
    //This is a primary track
    G4TrackVector* fSecondary=fpSteppingManager->GetfSecondary();
    G4int tN2ndariesTot = fpSteppingManager->GetfN2ndariesAtRestDoIt()
      + fpSteppingManager->GetfN2ndariesAlongStepDoIt()
      + fpSteppingManager->GetfN2ndariesPostStepDoIt();

    //If we havent already found the conversion position and there were
    //secondaries generated, then search for it
    if(!eventInformation->IsConvPosSet() && tN2ndariesTot>0 ){
      for(size_t lp1=(*fSecondary).size()-tN2ndariesTot;
          lp1<(*fSecondary).size(); lp1++){
        const G4VProcess* creator=(*fSecondary)[lp1]->GetCreatorProcess();
        if(creator){
          G4String creatorName=creator->GetProcessName();
          if(creatorName=="phot"||creatorName=="compt"||creatorName=="conv"){
            //since this is happening before the secondary is being tracked
            //the Vertex position has not been set yet(set in initial step)
            eventInformation->SetConvPos((*fSecondary)[lp1]->GetPosition());
          }
        }
      }
    }

    if(fOneStepPrimaries&&thePrePV->GetName()=="scintillator")
      theTrack->SetTrackStatus(fStopAndKill);
  }

  if(!thePostPV){ // out of world
    fExpectedNextStatus=Undefined;
    return;
  }

  G4ParticleDefinition* particleType = theTrack->GetDefinition();
  if(particleType==G4OpticalPhoton::OpticalPhotonDefinition()){
    // Optical photon only

    if(thePostPV->GetName()=="expHall")
      // Kill photons entering expHall from something other than Slab
      theTrack->SetTrackStatus(fStopAndKill);

    //Was the photon absorbed by the absorption process
    if(thePostPoint->GetProcessDefinedStep()->GetProcessName()
       =="OpAbsorption"){
      eventInformation->IncAbsorption();
      trackInformation->AddTrackStatusFlag(absorbed);
    }

    boundaryStatus=boundary->GetStatus();

    //Check to see if the partcile was actually at a boundary
    //Otherwise the boundary status may not be valid
    //Prior to Geant4.6.0-p1 this would not have been enough to check
    if(thePostPoint->GetStepStatus()==fGeomBoundary){
      if(fExpectedNextStatus==StepTooSmall){
        if(boundaryStatus!=StepTooSmall){
          G4ExceptionDescription ed;
          ed << "MULTIPASSSteppingAction::UserSteppingAction(): "
                << "No reallocation step after reflection!"
                << G4endl;
          G4Exception("MULTIPASSSteppingAction::UserSteppingAction()", 
            "MULTIPASSExpl01",
          EventMustBeAborted,ed,
          "Something is wrong with the surface normal or geometry");
        }
      }
      fExpectedNextStatus=Undefined;
      switch(boundaryStatus){
      case Absorption:
        trackInformation->AddTrackStatusFlag(boundaryAbsorbed);
        eventInformation->IncBoundaryAbsorption();
        break;
      case Detection: //Note, this assumes that the volume causing detection
                      //is the photocathode because it is the only one with
                      //non-zero efficiency
        {
        //Triger sensitive detector manually since photon is
        //absorbed but status was Detection
        G4SDManager* SDman = G4SDManager::GetSDMpointer();
        G4String sdName="/MULTIPASSDet/pmtSD";
        MULTIPASSPMTSD* pmtSD = 
                          (MULTIPASSPMTSD*)SDman->FindSensitiveDetector(sdName);
        if(pmtSD)pmtSD->ProcessHits_constStep(theStep,NULL);
        trackInformation->AddTrackStatusFlag(hitPMT);
        break;
        }
      case FresnelReflection:
      case TotalInternalReflection:
      case LambertianReflection:
      case LobeReflection:
      case SpikeReflection:
      case BackScattering:
        eventInformation->IncReflections();
        trackInformation->IncReflections();
        fExpectedNextStatus=StepTooSmall;
        break;
      default:
        break;
      }
    }
  }
  else if (particleType==G4Gamma::GammaDefinition()) {

    if(thePostPoint->GetProcessDefinedStep()->GetProcessName()
      !="Transportation"){


      G4SDManager* SDman = G4SDManager::GetSDMpointer();
      G4String sdName="/MULTIPASSDet/scintSD";
      MULTIPASSScintSD* scintSD = 
                        (MULTIPASSScintSD*)SDman->FindSensitiveDetector(sdName);
      if(thePostPoint->GetProcessDefinedStep()->GetProcessName() =="compt"){
        eventInformation->IncCompt();
        trackInformation->IncCompt();
        if(scintSD)scintSD->ProcessHits_compton(theStep,NULL,0);
      } else if (thePostPoint->GetProcessDefinedStep()->GetProcessName() ==
                                                                        "phot"){
        if(scintSD)scintSD->ProcessHits_compton(theStep,NULL,1);
      } else if (thePostPoint->GetProcessDefinedStep()->GetProcessName() ==
                                                                        "conv"){
        if(scintSD)scintSD->ProcessHits_compton(theStep,NULL,2);
      } else if (thePostPoint->GetProcessDefinedStep()->GetProcessName() ==
                                                                        "Rayl"){
        if(scintSD)scintSD->ProcessHits_compton(theStep,NULL,3);
      } else {
        if(scintSD)scintSD->ProcessHits_compton(theStep,NULL,4);
        // OBSERVED in simulations:
        //
        // eIoni - electron ionisation
        // msc - Multiple scattering. Process for charged particles. 
        //       It computes the change of energy and direction due to 
        //       multiple scattering on nuclei
      } 
      // 0-compt, 1-phot, 2-conv, 3-Rayl, 4-unknown
    
    }


    if(thePostPV->GetName()=="expHall" && thePrePV->GetName()=="housing"){
      // Kill photons exiting detector to expHall
      // and grab energy
      //
      // To do get track energy

      eventInformation->IncEscapeEn(thePrePoint->GetKineticEnergy());
      // Energy escaped however can still come back in!
      // unless track is killed
      // theTrack->SetTrackStatus(fStopAndKill);
    }
  }

}
