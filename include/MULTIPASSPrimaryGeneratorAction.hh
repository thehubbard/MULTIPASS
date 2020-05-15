//
// File:         MULTIPASS_Det/include/
//                                       MULTIPASSPrimaryGeneratorAction.hh
// Description:  PrimaryGeneratorAction header for MULTIPASS simulation. 
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
#ifndef MULTIPASSPrimaryGeneratorAction_h
#define MULTIPASSPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"

// #include "G4PhysicsOrderedFreeVector.hh"

class G4ParticleGun;
class G4Event;
class MULTIPASSDetectorConstruction;
class MULTIPASSPrimaryGeneratorMessenger;


class MULTIPASSPrimaryGeneratorAction : public 
                                                G4VUserPrimaryGeneratorAction
{
  public:

    MULTIPASSPrimaryGeneratorAction(G4String runMode);
    virtual ~MULTIPASSPrimaryGeneratorAction();
 
  public:

    virtual void GeneratePrimaries(G4Event* anEvent);
    
    void GeneratePrimariesOptics(G4Event* anEvent);
    void GeneratePrimariesGamma(G4Event* anEvent);
    
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
    void SetTCModelParticle(G4String);
    G4String GetTCModelParticle() const { return fTheParticle; }

    inline void SetEparticle(G4double enp) { fE_particle = enp; }
    G4double GetEparticle()const { return fE_particle; }

    inline void SetScintYield(G4double sy) { fScintyield = sy/MeV; }
    G4double GetScintYield() const { return fScintyield; }

    inline void SetOrigin(G4String ori) { fOrigin = ori; }
    G4String GetOrigin() const { return fOrigin; }

    inline void SetOffset(G4double off) { fOffset = off; }
    G4double GetOffset() const { return fOffset; }

    inline void SetWavelength(G4double wl) { fWavelength = wl*nm; }
    G4double GetWavelength() const { return fWavelength; }

    inline void SetScanOffsetX(G4double off) { fScanOffsetX = off; }
    G4double GetScanOffsetX() const { return fScanOffsetX; }
    G4double GetScanPosX(); 
    
    inline void SetScanOffsetY(G4double off) { fScanOffsetY = off; }
    G4double GetScanOffsetY() const { return fScanOffsetY; }
    G4double GetScanPosY(); 


  private:

    G4String        fRunMode;

    G4ParticleGun* fParticleGun;
    MULTIPASSPrimaryGeneratorMessenger* fGunMessenger;
    const MULTIPASSDetectorConstruction*    fDetector; 
    G4String          fTheParticle;

    G4double            fE_particle;
    G4double            fScintyield;
    G4String            fOrigin;
    G4double            fOffset;
    G4double            fWavelength;
    G4double            fScanOffsetX;
    G4double            fScanOffsetY;


	G4double single_exp(G4double t, G4double tau2);
	G4double bi_exp(G4double t, G4double tau1, G4double tau2);

	// emission time distribution when there is a finite rise time
	G4double sample_time(G4double tau1, G4double tau2);
};

////////////////////
// Inline methods
////////////////////

inline
G4double MULTIPASSPrimaryGeneratorAction::single_exp(G4double t, 
                                                                G4double tau2)
{
    return std::exp(-1.0*t/tau2)/tau2;
}

inline
G4double MULTIPASSPrimaryGeneratorAction::bi_exp(G4double t, G4double tau1,
                                                                G4double tau2)
{
    return std::exp(-1.0*t/tau2)*(1-std::exp(-1.0*t/tau1))/tau2/tau2*(tau1+
                                                                        tau2);
}

#endif
