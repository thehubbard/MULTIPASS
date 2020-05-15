//
// File:         MULTIPASS_Det/src/MULTIPASSEventAction.cc
// Description:  EventAction for MULTIPASS simulation. Based on original
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

#include "MULTIPASSEventAction.hh"
#include "MULTIPASSScintHit.hh"
#include "MULTIPASSPMTHit.hh"
#include "MULTIPASSOpticalHit.hh"
#include "MULTIPASSComptonHit.hh"
#include "MULTIPASSUserEventInformation.hh"
#include "MULTIPASSTrajectory.hh"
#include "MULTIPASSHistoManager.hh"
#include "MULTIPASSPrimaryGeneratorAction.hh"


#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleMomentum.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSEventAction::MULTIPASSEventAction()
  :fSaveThreshold(0),fScintCollID(-1),fPMTCollID(-1),  fOptCollID(-1),
  fComptCollID(-1),
  fVerbose(0), fPMTThreshold(1),fForcedrawphotons(false),fForcenophotons(false)
{
  fEventMessenger = new MULTIPASSEventMessenger(this);
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSEventAction::~MULTIPASSEventAction(){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSEventAction::BeginOfEventAction(const G4Event* /*anEvent*/){
 
  //New event, add the user information object
  G4EventManager::
    GetEventManager()->SetUserInformation(new MULTIPASSUserEventInformation);

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  if(fScintCollID<0)
    fScintCollID=SDman->GetCollectionID("scintCollection");
  if(fPMTCollID<0)
    fPMTCollID=SDman->GetCollectionID("pmtHitCollection");
  if(fOptCollID<0)
    fOptCollID=SDman->GetCollectionID("opticalPMTCollection");
  if(fComptCollID<0)
    fComptCollID=SDman->GetCollectionID("comptCollection");

}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSEventAction::EndOfEventAction(const G4Event* anEvent){

  MULTIPASSUserEventInformation* eventInformation
    =(MULTIPASSUserEventInformation*)anEvent->GetUserInformation();
 
  G4TrajectoryContainer* trajectoryContainer=anEvent->GetTrajectoryContainer();
 
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

  // extract the trajectories and draw them
  if (G4VVisManager::GetConcreteInstance()){
    for (G4int i=0; i<n_trajectories; i++){
      MULTIPASSTrajectory* trj = (MULTIPASSTrajectory*)
        ((*(anEvent->GetTrajectoryContainer()))[i]);
      if(trj->GetParticleName()=="opticalphoton"){
        trj->SetForceDrawTrajectory(fForcedrawphotons);
        trj->SetForceNoDrawTrajectory(fForcenophotons);
      }
      trj->DrawTrajectory();
    }
  }
 
  MULTIPASSScintHitsCollection* scintHC = 0;
  MULTIPASSPMTHitsCollection* pmtHC = 0;
  MULTIPASSOpticalHitsCollection* optHC = 0;
  MULTIPASSComptonHitsCollection* comptHC = 0;
  G4HCofThisEvent* hitsCE = anEvent->GetHCofThisEvent();
 
  //Get the hit collections
  if(hitsCE){
    if(fScintCollID>=0)
      scintHC = (MULTIPASSScintHitsCollection*)(hitsCE->GetHC(fScintCollID));
    if(fPMTCollID>=0)
      pmtHC = (MULTIPASSPMTHitsCollection*)(hitsCE->GetHC(fPMTCollID));
    if(fOptCollID>=0)
      optHC = (MULTIPASSOpticalHitsCollection*)(hitsCE->GetHC(fOptCollID));
    if(fComptCollID>=0)
      comptHC = (MULTIPASSComptonHitsCollection*)(hitsCE->GetHC(fComptCollID));
  }
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  //Hits in scintillator
  if(scintHC){
    int n_hit = scintHC->entries();
    G4ThreeVector  eWeightPos(0.);
    G4double edep;
    G4double edepMax=0;

    for(int i=0;i<n_hit;i++){ //gather info on hits in scintillator
      edep=(*scintHC)[i]->GetEdep();
      eventInformation->IncEDep(edep); //sum up the edep

      eWeightPos += (*scintHC)[i]->GetPos()*edep;//calculate energy weighted pos
      if(edep>edepMax){
        edepMax=edep;//store max energy deposit
        G4ThreeVector posMax=(*scintHC)[i]->GetPos();
        eventInformation->SetPosMax(posMax,edep);
      }
    }
    if(eventInformation->GetEDep()==0.){
      if(fVerbose>0)G4cout<<"No hits in the scintillator this event."<<G4endl;
    }
    else{
      //Finish calculation of energy weighted position
      eWeightPos/=eventInformation->GetEDep();
      eventInformation->SetEWeightPos(eWeightPos);
      if(fVerbose>0){
        G4cout << "\tEnergy weighted position of hits in MULTIPASS : "
               << eWeightPos/mm << G4endl;
      }
    }
    if(fVerbose>0){
    G4cout << "\tTotal energy deposition in scintillator : "
           << eventInformation->GetEDep() / keV << " (keV)" << G4endl;

    G4cout << "\tTotal energy escaping scintillator : "
           << eventInformation->GetEscapeEn() / keV << " (keV)" << G4endl;

    G4cout << "\tTotal energy scored: "
           << eventInformation->GetEDep() / keV + 
              eventInformation->GetEscapeEn() / keV << " (keV)" << G4endl;

     G4cout << "\tNumber of hits in scintillator : "
       << n_hit  << G4endl;      
    }
  }
 
  if(pmtHC){
    G4ThreeVector reconPos(0.,0.,0.);
    G4int pmts=pmtHC->entries();
    //Gather info from all PMTs
    for(G4int i=0;i<pmts;i++){
      eventInformation->IncHitCount((*pmtHC)[i]->GetPhotonCount());

      reconPos+=(*pmtHC)[i]->GetPMTPos()*(*pmtHC)[i]->GetPhotonCount();
      if((*pmtHC)[i]->GetPhotonCount()>=fPMTThreshold){
        eventInformation->IncPMTSAboveThreshold();
      }
      else{ // Wasnt above the threshold, turn it back off
        (*pmtHC)[i]->SetDrawit(false);
      }
    }
 
    if(eventInformation->GetHitCount()>0){//dont bother unless there were hits
      reconPos/=eventInformation->GetHitCount();
      if(fVerbose>0){
        G4cout << "\tReconstructed position of hits in MULTIPASS : "
               << reconPos/mm << G4endl;
      }
      eventInformation->SetReconPos(reconPos);
    }
    pmtHC->DrawAllHits();
  }

  if(fVerbose>0){
    //End of event output. later to be controlled by a verbose level
    G4cout << "\tNumber of photons that hit PMTs in this event : "
           << eventInformation->GetHitCount() << G4endl;
    G4cout << "\tNumber of total number of reflections in this event : "
           << eventInformation->GetReflectionCount() << G4endl;
    G4cout << "\tNumber of PMTs above threshold("<<fPMTThreshold<<") : "
           << eventInformation->GetPMTSAboveThreshold() << G4endl;
    G4cout << "\tNumber of photons produced by scintillation in this event : "
           << eventInformation->GetPhotonCount_Scint() << G4endl;
    G4cout << "\tNumber of photons produced by cerenkov in this event : "
           << eventInformation->GetPhotonCount_Ceren() << G4endl;
    G4cout << "\tNumber of photons absorbed (OpAbsorption) in this event : "
           << eventInformation->GetAbsorptionCount() << G4endl;
    G4cout << "\tNumber of photons absorbed at boundaries (OpBoundary) in "
           << "this event : " << eventInformation->GetBoundaryAbsorptionCount()
           << G4endl;
    G4cout << "Unacounted for photons in this event : "
           << (eventInformation->GetPhotonCount_Scint() +
               eventInformation->GetPhotonCount_Ceren() -
               eventInformation->GetAbsorptionCount() -
               eventInformation->GetHitCount() -
               eventInformation->GetBoundaryAbsorptionCount())
           << G4endl;
  G4cout << "Reflections: " << eventInformation->GetReflectionCount()
           << G4endl;    
  }
  // Get event ID
  G4int eventID = anEvent->GetEventID();

  //If we have set the flag to save 'special' events, save here
  if(fSaveThreshold&&eventInformation->GetPhotonCount() <= fSaveThreshold)
    G4RunManager::GetRunManager()->rndmSaveThisEvent();

  // Fill histograms
  analysisManager->FillH1(0, eventInformation->GetEDep());
  analysisManager->FillH1(1, eventInformation->GetHitCount());

  const MULTIPASSPrimaryGeneratorAction* PGA =
       static_cast<const MULTIPASSPrimaryGeneratorAction*>
        (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());

  G4ParticleMomentum opticalStartMomentum = 
        PGA->GetParticleGun()->GetParticleMomentumDirection();


  // EventInformation ntuple id = 0 
  analysisManager->FillNtupleIColumn(0,0, eventID);
  analysisManager->FillNtupleDColumn(0,1, eventInformation->GetEDep());
  analysisManager->FillNtupleIColumn(0,2, 
                                  eventInformation->GetPhotonCount_Scint());
  analysisManager->FillNtupleIColumn(0,3, 
                                  eventInformation->GetPhotonCount_Ceren());
  analysisManager->FillNtupleIColumn(0,4,
                                  eventInformation->GetPhotonCount());
  analysisManager->FillNtupleIColumn(0,5, 
                                  eventInformation->GetReflectionCount());
  analysisManager->FillNtupleIColumn(0,6, 
                                  eventInformation->GetAbsorptionCount());
  analysisManager->FillNtupleIColumn(0,7, 
                                eventInformation->GetBoundaryAbsorptionCount());

  analysisManager->AddNtupleRow(0);  


  G4ThreeVector photonStartMom;
  if(pmtHC && optHC){
    G4int optPhotons=optHC->entries();

    for(G4int i=0;i<optPhotons;i++){
      // Fill Ntuple
      if ((*optHC)[i]->GetPMTNumber() == 0){
        analysisManager->FillH2(0,(*optHC)[i]->GetGlobalTime()/ns,eventID);
        photonStartMom=(*optHC)[i]->GetStartMom();

        analysisManager->FillH2(1, photonStartMom.theta(),photonStartMom.phi());
        analysisManager->FillH1(2, (*optHC)[i]->GetReflections());
        analysisManager->FillH1(3, (*optHC)[i]->GetLocalTime()/ns);
        analysisManager->FillH1(4, (*optHC)[i]->GetPathlength()/mm);
        
        // Further analysis turn on corresponding components in HistoManager
        /*
        analysisManager->FillNtupleIColumn(1,0, eventID);
        // Need to find vertex time from event action to provide a better
        // arrival time
        analysisManager->FillNtupleDColumn(1,1, (*optHC)[i]->GetGlobalTime()/ns);
        analysisManager->FillNtupleIColumn(1,2, (*optHC)[i]->GetReflections());
        analysisManager->FillNtupleDColumn(1,3, (*optHC)[i]->GetEnergy());
        analysisManager->FillNtupleDColumn(1,4, (*optHC)[i]->GetWavelength()/nm);
        analysisManager->FillNtupleIColumn(1,5, (*optHC)[i]->GetPMTNumber());
        analysisManager->FillNtupleDColumn(1,6, (*optHC)[i]->GetLocalTime()/ns);
        analysisManager->FillNtupleDColumn(1,7, (*optHC)[i]->GetPathlength()/mm);
        // analysisManager->FillNtupleDColumn(1,8, opticalStartMomentum.x());
        // analysisManager->FillNtupleDColumn(1,9, opticalStartMomentum.y());
        // analysisManager->FillNtupleDColumn(1,10, opticalStartMomentum.z());
        photonStartMom=(*optHC)[i]->GetStartMom();
        analysisManager->FillNtupleDColumn(1,8, photonStartMom.x());
        analysisManager->FillNtupleDColumn(1,9, photonStartMom.y());
        analysisManager->FillNtupleDColumn(1,10, photonStartMom.z());

        analysisManager->FillNtupleDColumn(1,11, (*optHC)[i]->GetGlobalTime()/ns
                                              - (*optHC)[i]->GetLocalTime()/ns);

        analysisManager->AddNtupleRow(1);// need to specify ntuple id*/
      }
    }
  }
  /*
  if(comptHC) {
    G4int comptonScat=comptHC->entries();
    G4ThreeVector location (0,0,0);
    for(G4int i=0;i<comptonScat;i++){
      // Fill Ntuple
      //
      // Compton Histo will be Deprecated
      // Hist ID for Gamma interactions will then change
      //
      if ((*comptHC)[i]->GetProcessID()==0 ) {
        analysisManager->FillNtupleIColumn(2,0, eventID);
        location = (*comptHC)[i]->GetLocation();
        analysisManager->FillNtupleDColumn(2,1,location.x() );
        analysisManager->FillNtupleDColumn(2,2,location.y() );
        analysisManager->FillNtupleDColumn(2,3,location.z() );
        analysisManager->AddNtupleRow(2);// need to specify ntuple id
      }
      // Gamms interactions
      analysisManager->FillNtupleIColumn(3,0, eventID);
      analysisManager->FillNtupleDColumn(3,1,location.x()/mm );
      analysisManager->FillNtupleDColumn(3,2,location.y()/mm );
      analysisManager->FillNtupleDColumn(3,3,location.z()/mm );
      analysisManager->FillNtupleDColumn(3,4,(*comptHC)[i]->GetTime()/ns );
      analysisManager->FillNtupleIColumn(3,5,(*comptHC)[i]->GetProcessID() );
      analysisManager->AddNtupleRow(3);// need to specify ntuple id

    }

  }*/
 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSEventAction::SetSaveThreshold(G4int save){
/*Sets the save threshold for the random number seed. If the number of photons
generated in an event is lower than this, then save the seed for this event
in a file called run###evt###.rndm
*/
  fSaveThreshold=save;
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  G4RunManager::GetRunManager()->SetRandomNumberStoreDir("random/");
  //  G4UImanager::GetUIpointer()->ApplyCommand("/random/setSavingFlag 1");
}
