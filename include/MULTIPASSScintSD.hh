//
// File:         MULTIPASS_Det/include/MULTIPASSScintSD.hh
// Description:  ScintSD header for MULTIPASS simulation. Based on 
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
//
#ifndef MULTIPASSScintSD_h
#define MULTIPASSScintSD_h 1

#include "MULTIPASSScintHit.hh"
#include "MULTIPASSComptonHit.hh"

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class MULTIPASSScintSD : public G4VSensitiveDetector
{
  public:

    MULTIPASSScintSD(G4String name);
    virtual ~MULTIPASSScintSD();

    virtual void Initialize(G4HCofThisEvent* );
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* );

    //A version of processHits that keeps aStep constant for Compton tally
    G4bool ProcessHits_compton(const G4Step* ,
                                 G4TouchableHistory*, G4int );

    virtual void EndOfEvent(G4HCofThisEvent* );
    virtual void clear();
    virtual void DrawAll();
    virtual void PrintAll();
 
  private:

    MULTIPASSScintHitsCollection* fScintCollection;
    MULTIPASSComptonHitsCollection* fComptCollection;
 
};

#endif
