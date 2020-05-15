//
// File:         MULTIPASS_Det/src/MULTIPASSScintSD.cc
// Description:  ScintSD for MULTIPASS simulation. Based on original
//               code from Geant4 optical/LXe from 10.4.p01
// Version:      1.0.0
// Created:      2020-03-18
// Author:       Michael Hubbard
// Modified:     2019-05-22 MHUBBARD
//               > Added Time and ProcID to hits
//
//
// Tested with:
//              Geant4 v 10.4.p01
//              ROOT v 6.12.06
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//

#include "MULTIPASSScintSD.hh"
#include "MULTIPASSScintHit.hh"
#include "MULTIPASSComptonHit.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"
#include "G4ParticleTypes.hh"

#include "G4UnitsTable.hh" // For G4BestUnit
#include "G4SystemOfUnits.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSScintSD::MULTIPASSScintSD(G4String name)
  : G4VSensitiveDetector(name)
{
  fScintCollection = NULL;
  fComptCollection = NULL;
  collectionName.insert("scintCollection"); // Energy dep info in scint
  collectionName.insert("comptCollection"); // Compton interaction collection
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSScintSD::~MULTIPASSScintSD() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSScintSD::Initialize(G4HCofThisEvent* hitsCE){
  fScintCollection = new MULTIPASSScintHitsCollection
                      (SensitiveDetectorName,collectionName[0]);
  //A way to keep all the hits of this event in one place if needed
  static G4int hitsCID = -1;
  // if(hitsCID<0){
  //   hitsCID = GetCollectionID(0);
  // }
  hitsCID = GetCollectionID(0);
  hitsCE->AddHitsCollection( hitsCID, fScintCollection );

  fComptCollection = new MULTIPASSComptonHitsCollection
                    (SensitiveDetectorName,collectionName[1]);

  hitsCID = GetCollectionID(1);
  hitsCE->AddHitsCollection( hitsCID, fComptCollection );


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool MULTIPASSScintSD::ProcessHits(G4Step* aStep,G4TouchableHistory* ){
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep==0. || aStep->GetTrack()->GetDefinition()
                 == G4OpticalPhoton::OpticalPhotonDefinition()){
    // No edep so don't count as hit
    // Also do not count optical photon as they do not conserve energy
    // in Geant4 code by design
    return false;
  }

  G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
  G4TouchableHistory* theTouchable =
    (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
  G4VPhysicalVolume* thePrePV = theTouchable->GetVolume();
 
  G4StepPoint* thePostPoint = aStep->GetPostStepPoint();

  //Get the average position of the hit
  G4ThreeVector pos = thePrePoint->GetPosition() + thePostPoint->GetPosition();
  pos/=2.;

  MULTIPASSScintHit* scintHit = new MULTIPASSScintHit(thePrePV);

  scintHit->SetEdep(edep);
  scintHit->SetPos(pos);
  scintHit->SetTime(aStep->GetTrack()->GetGlobalTime());
  
  fScintCollection->insert(scintHit);

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


G4bool MULTIPASSScintSD::ProcessHits_compton(const G4Step* aStep,
                                       G4TouchableHistory*, G4int procID ){


  G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
 
  G4StepPoint* thePostPoint = aStep->GetPostStepPoint();

  //Get the average position of the compton scatter
  G4ThreeVector pos = thePrePoint->GetPosition() + thePostPoint->GetPosition();
  pos/=2.;

  MULTIPASSComptonHit* comptHit = new MULTIPASSComptonHit();
  comptHit->SetLocation(pos);
  
  comptHit->SetTime(aStep->GetTrack()->GetGlobalTime());

  // 0-compt, 1-phot, 2-conv, 3-Rayl, 4-unknown
  comptHit->SetProcessID(procID);

  fComptCollection->insert(comptHit);

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSScintSD::EndOfEvent(G4HCofThisEvent* ) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSScintSD::clear() {} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSScintSD::DrawAll() {} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSScintSD::PrintAll() {} 
