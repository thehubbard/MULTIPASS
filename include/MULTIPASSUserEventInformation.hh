//
// File:         MULTIPASS_Det/include/
//                                         MULTIPASSUserEventInformation.hh
// Description:  UserEventInformation header for MULTIPASS simulation. 
//               Based on original code from Geant4 optical/LXe from 10.4.p01
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
#include "G4VUserEventInformation.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

#ifndef MULTIPASSUserEventInformation_h
#define MULTIPASSUserEventInformation_h 1

class MULTIPASSUserEventInformation : public G4VUserEventInformation
{
  public:

    MULTIPASSUserEventInformation();
    virtual ~MULTIPASSUserEventInformation();

    inline virtual void Print()const{};

    void IncPhotonCount_Scint(){fPhotonCount_Scint++;}
    void IncPhotonCount_Ceren(){fPhotonCount_Ceren++;}
    void IncEDep(G4double dep){fTotE+=dep;}
    void IncAbsorption(){fAbsorptionCount++;}
    void IncBoundaryAbsorption(){fBoundaryAbsorptionCount++;}
    void IncHitCount(G4int i=1){fHitCount+=i;}
    void IncReflections(){fReflections++;}

    void SetEWeightPos(const G4ThreeVector& p){fEWeightPos=p;}
    void SetReconPos(const G4ThreeVector& p){fReconPos=p;}
    void SetConvPos(const G4ThreeVector& p){fConvPos=p;fConvPosSet=true;}
    void SetPosMax(const G4ThreeVector& p,G4double edep){fPosMax=p;
                                                                fEdepMax=edep;}

    G4int GetPhotonCount_Scint()const {return fPhotonCount_Scint;}
    G4int GetPhotonCount_Ceren()const {return fPhotonCount_Ceren;}
    G4int GetHitCount()const {return fHitCount;}
    G4double GetEDep()const {return fTotE;}
    G4int GetAbsorptionCount()const {return fAbsorptionCount;}
    G4int GetBoundaryAbsorptionCount() const {return fBoundaryAbsorptionCount;}
    G4int GetReflectionCount()const {return fReflections;}


    G4ThreeVector GetEWeightPos(){return fEWeightPos;}
    G4ThreeVector GetReconPos(){return fReconPos;}
    G4ThreeVector GetConvPos(){return fConvPos;}
    G4ThreeVector GetPosMax(){return fPosMax;}
    G4double GetEDepMax(){return fEdepMax;}
    G4double IsConvPosSet(){return fConvPosSet;}

    G4double GetEscapeEn(){return fEscapeEn;}
    void IncEscapeEn(G4double dep){fEscapeEn+=dep;}

    void IncCompt(){fComptScat++;}
    G4int GetComptonScatters()const {return fComptScat;}


    //Gets the total photon count produced
    G4int GetPhotonCount(){return fPhotonCount_Scint+fPhotonCount_Ceren;}

    void IncPMTSAboveThreshold(){fPMTsAboveThreshold++;}
    G4int GetPMTSAboveThreshold(){return fPMTsAboveThreshold;}

  private:

    G4int fHitCount;
    G4int fPhotonCount_Scint;
    G4int fPhotonCount_Ceren;
    G4int fAbsorptionCount;
    G4int fBoundaryAbsorptionCount;
    G4int fReflections;

    G4double fTotE;

    //These only have meaning if totE > 0
    //If totE = 0 then these wont be set by EndOfEventAction
    G4ThreeVector fEWeightPos;
    G4ThreeVector fReconPos; //Also relies on hitCount>0
    G4ThreeVector fConvPos;//true (initial) converstion position
    G4bool fConvPosSet;
    G4ThreeVector fPosMax;
    G4double fEdepMax;

    G4double fEscapeEn;
    G4int fComptScat;

    G4int fPMTsAboveThreshold;

};

#endif
