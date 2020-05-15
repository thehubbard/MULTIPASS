//
// File:         MULTIPASS_Det/src/MULTIPASSPMTSD.cc
// Description:  MULTIPASSPMTSD for MULTIPASS simulation. Based on original
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

#include "MULTIPASSPMTSD.hh"
#include "MULTIPASSPMTHit.hh"
#include "MULTIPASSOpticalHit.hh"
#include "MULTIPASSDetectorConstruction.hh"
#include "MULTIPASSUserTrackInformation.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSPMTSD::MULTIPASSPMTSD(G4String name)
  : G4VSensitiveDetector(name),fPMTHitCollection(0),fOptCollection(0),
  fPMTPositionsX(0),fPMTPositionsY(0),fPMTPositionsZ(0)
{
  collectionName.insert("pmtHitCollection"); // Counting photons-quick
  collectionName.insert("opticalPMTCollection"); // information regarding
                                                // collected photons
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSPMTSD::~MULTIPASSPMTSD() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSPMTSD::SetPmtPositions(
                                    const std::vector<G4ThreeVector>& positions)
{
  for (G4int i=0; i<G4int(positions.size()); ++i) {
    if(fPMTPositionsX)fPMTPositionsX->push_back(positions[i].x());
    if(fPMTPositionsY)fPMTPositionsY->push_back(positions[i].y());
    if(fPMTPositionsZ)fPMTPositionsZ->push_back(positions[i].z());
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSPMTSD::Initialize(G4HCofThisEvent* hitsCE){
  fPMTHitCollection = new MULTIPASSPMTHitsCollection
                      (SensitiveDetectorName,collectionName[0]);
  //Store collection with event and keep ID
  static G4int hitCID = -1;

  hitCID = GetCollectionID(0);
  hitsCE->AddHitsCollection( hitCID, fPMTHitCollection );

  fOptCollection = new MULTIPASSOpticalHitsCollection
                    (SensitiveDetectorName,collectionName[1]);

  hitCID = GetCollectionID(1);
  hitsCE->AddHitsCollection( hitCID, fOptCollection );

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool MULTIPASSPMTSD::ProcessHits(G4Step* ,G4TouchableHistory* ){
  return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//Generates a hit and uses the postStepPoint's mother volume replica number
//PostStepPoint because the hit is generated manually when the photon is
//absorbed by the photocathode

G4bool MULTIPASSPMTSD::ProcessHits_constStep(const G4Step* aStep,
                                       G4TouchableHistory* ){

  G4Track* theTrack = aStep->GetTrack();
  //need to know if this is an optical photon
  if(theTrack->GetDefinition()
     != G4OpticalPhoton::OpticalPhotonDefinition()) return false;
 
  //User replica number 1 since photocathode is a daughter volume
  //to the pmt which was replicated
  G4int pmtNumber=
    aStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber(1);
  G4VPhysicalVolume* physVol=
    aStep->GetPostStepPoint()->GetTouchable()->GetVolume(1);

  //Find the correct hit collection
  G4int n=fPMTHitCollection->entries();
  MULTIPASSPMTHit* hit=NULL;
  for(G4int i=0;i<n;i++){
    if((*fPMTHitCollection)[i]->GetPMTNumber()==pmtNumber){
      hit=(*fPMTHitCollection)[i];
      break;
    }
  }
 
  if(hit==NULL){//this pmt wasnt previously hit in this event
    hit = new MULTIPASSPMTHit(); //so create new hit
    hit->SetPMTNumber(pmtNumber);
    hit->SetPMTPhysVol(physVol);
    fPMTHitCollection->insert(hit);
    hit->SetPMTPos((*fPMTPositionsX)[pmtNumber],(*fPMTPositionsY)[pmtNumber],
                   (*fPMTPositionsZ)[pmtNumber]);
  }

  hit->IncPhotonCount(); //increment hit for the selected pmt


  hit->SetDrawit(true);

  MULTIPASSUserTrackInformation* trackInfo=
      (MULTIPASSUserTrackInformation*)aStep->GetTrack()->GetUserInformation();


  MULTIPASSOpticalHit* opHit = new MULTIPASSOpticalHit(); 
  // Get PMT -> Useful for multiple PMTs
  opHit->SetPMTNumber(pmtNumber);
  opHit->SetPMTPos((*fPMTPositionsX)[pmtNumber],(*fPMTPositionsY)[pmtNumber],
                   (*fPMTPositionsZ)[pmtNumber]);

  // Arrival time since track was generated.
  opHit->SetLocalTime(theTrack->GetLocalTime());
  // Arrival time in global time
  opHit->SetGlobalTime(theTrack->GetGlobalTime());

  // Number of reflections optical photon took before collection
  opHit->SetReflections(trackInfo->GetReflectionCount());

  // Length of path the optical photon travel
  opHit->SetPathlength(theTrack->GetTrackLength());

  // Energy of optical photon <==> wavelength
  opHit->SetEnergy(theTrack->GetTotalEnergy()/eV);

  // Original Momentum
  opHit->SetStartMom(trackInfo->GetOriginalMomentum());

  fOptCollection->insert(opHit);


  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSPMTSD::EndOfEvent(G4HCofThisEvent* ) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSPMTSD::clear() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSPMTSD::DrawAll() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSPMTSD::PrintAll() {}
