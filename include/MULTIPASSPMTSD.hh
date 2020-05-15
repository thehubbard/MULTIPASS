//
// File:         MULTIPASS_Det/include/MULTIPASSPMTSD.hh
// Description:  PMTSD header for MULTIPASS simulation. Based on original
//               code from Geant4 optical/LXe from 10.4.p01
// Version:      1.0.0
// Created:      2020-03-18
// Author:       Michael Hubbard
// Modified:     
//
// Tested with:
//              Geant4 v 10.4.p01
//              ROOT v 6.12.06
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//
#ifndef MULTIPASSPMTSD_h
#define MULTIPASSPMTSD_h 1

#include "G4DataVector.hh"
#include "G4VSensitiveDetector.hh"
#include "MULTIPASSPMTHit.hh"
#include "MULTIPASSOpticalHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

class MULTIPASSPMTSD : public G4VSensitiveDetector
{

  public:

    MULTIPASSPMTSD(G4String name);
    virtual ~MULTIPASSPMTSD();
 
    virtual void Initialize(G4HCofThisEvent* );
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* );
 
    //A version of processHits that keeps aStep constant
    G4bool ProcessHits_constStep(const G4Step* ,
                                 G4TouchableHistory* );
    virtual void EndOfEvent(G4HCofThisEvent* );
    virtual void clear();
    void DrawAll();
    void PrintAll();
 
    //Initialize the arrays to store pmt possitions
    inline void InitPMTs(){
      if(fPMTPositionsX)delete fPMTPositionsX;
      if(fPMTPositionsY)delete fPMTPositionsY;
      if(fPMTPositionsZ)delete fPMTPositionsZ;
      fPMTPositionsX=new G4DataVector();
      fPMTPositionsY=new G4DataVector();
      fPMTPositionsZ=new G4DataVector();
    }

    //Store a pmt position
    void SetPmtPositions(const std::vector<G4ThreeVector>& positions);

  private:

    MULTIPASSPMTHitsCollection* fPMTHitCollection;
    MULTIPASSOpticalHitsCollection* fOptCollection;

    G4DataVector* fPMTPositionsX;
    G4DataVector* fPMTPositionsY;
    G4DataVector* fPMTPositionsZ;

};

#endif
