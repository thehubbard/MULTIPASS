//
// File:         MULTIPASS_Det/src/MULTIPASSPrimaryGeneratorAction.cc
// Description:  PrimaryGeneratorAction for MULTIPASS simulation. Based on 
//               original code from Geant4 optical/LXe from 10.4.p01
// Version:      1.0.0
// Created:      2020-03-18
// Author:       Michael Hubbard
// Modified:     
//
//
//
//
// Tested with:
//              Geant4 v 10.4.p01
//              ROOT v 6.12.06
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//

#include "MULTIPASSPrimaryGeneratorAction.hh"
#include "MULTIPASSPrimaryGeneratorMessenger.hh"
#include "MULTIPASSDetectorConstruction.hh"

#include "G4RunManager.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "globals.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSPrimaryGeneratorAction::MULTIPASSPrimaryGeneratorAction(
                                                              G4String runMode){
  fRunMode = runMode;
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);
  fTheParticle = "Gamma";
  fOrigin = "Center";
  fE_particle = 477 *keV;
  fScintyield = 8600/MeV;
  fOffset = 1 *mm;
  fWavelength = 420*nm;

  fScanOffsetX = 0*mm;
  fScanOffsetY = 0*mm;

   //create a messenger for this class
  fGunMessenger = new MULTIPASSPrimaryGeneratorMessenger(this);

  fDetector = static_cast<const MULTIPASSDetectorConstruction*>
    (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  

  // Placed here such that it can be changed with messenger - in future release
  // Just use gun messenger!
  fParticleGun->SetParticlePosition(G4ThreeVector(0.0 , 0.0, 0.0));

  if (fRunMode!="Gamma" && fRunMode !="Full") {
    fParticleGun->SetParticleEnergy((1240.*nm/fWavelength) * eV);//default 420nm 
  }


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSPrimaryGeneratorAction::~MULTIPASSPrimaryGeneratorAction(){
    delete fParticleGun;
    delete fGunMessenger;
}

void MULTIPASSPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
  if (fRunMode!="Gamma" && fRunMode !="Full") {
    MULTIPASSPrimaryGeneratorAction::GeneratePrimariesOptics(anEvent);
  } else {
    MULTIPASSPrimaryGeneratorAction::GeneratePrimariesGamma(anEvent);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSPrimaryGeneratorAction::GeneratePrimariesOptics(G4Event* anEvent){
  // Location Location Location

  G4double locationX = 0.5*(fDetector->GetScintX()/mm) - fOffset/mm;
  G4double locationY = 0.5*(fDetector->GetScintY()/mm) - fOffset/mm;
  G4double locationZ = 0.5*(fDetector->GetScintZ()/mm) - fOffset/mm;

  
  if( fOrigin=="Center"){
    fParticleGun->SetParticlePosition(G4ThreeVector(0.0, 0.0, 0.0));
  } else if (fOrigin=="Back"){
    fParticleGun->SetParticlePosition(G4ThreeVector(0.0, 0.0, locationZ));
  } else if (fOrigin=="Top"){
    fParticleGun->SetParticlePosition(G4ThreeVector(0.0, locationY, 0.0));
  } else if (fOrigin=="Topback"){
    fParticleGun->SetParticlePosition(G4ThreeVector(0.0, locationY, locationZ));
  } else if (fOrigin=="Front"){
    fParticleGun->SetParticlePosition(G4ThreeVector(0.0, 0.0, -1*locationZ));
  } else if (fOrigin=="Topfront"){
    fParticleGun->SetParticlePosition(
                                  G4ThreeVector(0.0 , locationY, -1*locationZ));
  } else if (fOrigin=="Side"){
    fParticleGun->SetParticlePosition(G4ThreeVector(locationX, 0.0, 0.0));
  } else if (fOrigin=="Sideback"){
    fParticleGun->SetParticlePosition(G4ThreeVector(locationX, 0.0, locationZ));
  } else if (fOrigin=="Sidefront"){
    fParticleGun->SetParticlePosition(
                                  G4ThreeVector(locationX, 0.0, -1*locationZ));
  } else if (fOrigin=="Cornerback"){
    fParticleGun->SetParticlePosition(
                                G4ThreeVector(locationX, locationY, locationZ));
  } else if (fOrigin=="Cornerfront"){
    fParticleGun->SetParticlePosition(
                            G4ThreeVector(locationX , locationY, -1*locationZ));
  } else if (fOrigin=="Edge"){
    fParticleGun->SetParticlePosition(G4ThreeVector(locationX, locationY, 0.0));
  } else {
    G4cout << "Something went horribly wrong." << G4endl;
    G4cout << "Setting Origin to Center." << G4endl;
    fOrigin = "Center";
    fParticleGun->SetParticlePosition(G4ThreeVector(0.0, 0.0, 0.0));
  }
  
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  fParticleGun->SetParticleDefinition(particleTable->
                                     FindParticle(particleName="opticalphoton"));
  
  fParticleGun->SetParticleEnergy((1240.*nm/fWavelength)  *eV);

  G4double t0 = 0.0;

  G4int num_photons = fE_particle * fScintyield;

  G4double A = 0;
  G4double B = 0;
  G4double C = 0;



  G4double tau1 = 0*ns;
  G4double tau2 = 0*ns;
  G4double tau3 = 0*ns;

  if (fTheParticle=="Neutron") {
        A = 0.58;
    B = 0.18;
    C = 0.24;
    tau1 = 4.5*ns;
    tau2 = 20*ns;
    tau3 = 170*ns;


  } else {
    if (fTheParticle!="Gamma"){
      G4cout << "Invalid particle type." << G4endl;
      G4cout << "Setting particle to Gamma." << G4endl;
    }
    A = 0.74;
    B = 0.14;
    C = 0.12;
    tau1 = 4.3*ns;
    tau2 = 18*ns;
    tau3 = 140*ns;

  }


  G4double abc_tot = A + B + C;
  G4double tau1stop = A/abc_tot * num_photons;
  G4double tau2stop = (B/abc_tot) * num_photons + tau1stop;
  G4double scintillationDecayTime = 0;


  for (int i = 0; i < num_photons; ++i)
  {
    // Generate random isotropic photon direction
    // cost = cos(theta), sint sin(theta) 
    G4double cost = 1. - 2.*G4UniformRand();
    G4double sint = std::sqrt((1.-cost)*(1.+cost));

    G4double phi = twopi*G4UniformRand();
    G4double sinp = std::sin(phi);
    G4double cosp = std::cos(phi);

    // unit vectors so r = unity
    // theta [0,pi]
    // phi   [0, 2*pi)
    // x = r cos(phi)sin(theta) 
    // y = r sin(phi)sin(theta) 
    // z = r cos(theta)
    G4double px = sint*cosp;
    G4double py = sint*sinp;
    G4double pz = cost;

    // set photon momentum direction vector
    G4ParticleMomentum photonMomentum(px, py, pz);
    fParticleGun->SetParticleMomentumDirection(photonMomentum);

    // Determine polarization of new photon

    G4double sx = cost*cosp;
    G4double sy = cost*sinp;
    G4double sz = -sint;

    G4ThreeVector photonPolarization(sx, sy, sz);

    G4ThreeVector perp = photonMomentum.cross(photonPolarization);

    phi = twopi*G4UniformRand();
    sinp = std::sin(phi);
    cosp = std::cos(phi);

    photonPolarization = cosp * photonPolarization + sinp * perp;

    photonPolarization = photonPolarization.unit();

    fParticleGun->SetParticlePolarization(photonPolarization);

    // More time constants EJ299 etc                    
    if (i < tau1stop) {
      scintillationDecayTime = tau1;
      // G4cout<<"Alpha"<<G4endl;
    } else if (i < tau2stop) {
      scintillationDecayTime = tau2;
      // G4cout<<"Beta"<<G4endl;
    } else {
      scintillationDecayTime = tau3;
      // G4cout<<"Oh no"<<G4endl;
    }
    G4double startTime = 
                  t0 - scintillationDecayTime * std::log(G4UniformRand()); 
                  

    fParticleGun->SetParticleTime(startTime);

    fParticleGun->GeneratePrimaryVertex(anEvent);
  }


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSPrimaryGeneratorAction::GeneratePrimariesGamma(G4Event* anEvent) {
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  
  // Perpendicular distance from source to detector. 
  G4double distance = 40*cm;

  //
  // Currently have to manually change time constants in detector contruction
  //
  if(fTheParticle ==  "Neutron") {
    fParticleGun->SetParticleDefinition(particleTable->
                             FindParticle("neutron"));

  } else {
    if (fTheParticle != "Gamma"){
    G4cout << "Invalid particle type." << G4endl;
    G4cout << "Setting particle to Gamma." << G4endl;
    }
      fParticleGun->SetParticleDefinition(particleTable->
                                     FindParticle("gamma"));

  } 


  G4double scintX = fDetector->GetScintX()/mm;
  G4double scintY = fDetector->GetScintY()/mm;
  G4double scintZ = fDetector->GetScintZ()/mm;
  G4double housingThickness = fDetector->GetHousingThickness()/mm;

  G4double scanX0 = -1*scintX/2;
  G4double scanY0 = scintZ/2;

  scanX0 += fScanOffsetX; // Needs to be added for X
  scanY0 -= fScanOffsetY; // Needs to be taken away for Y

  fParticleGun->SetParticlePosition(
    G4ThreeVector(scanX0 , -1*((scintY/2)+housingThickness+distance), scanY0));


  // Direction uniform in solid angle
  // It is all in radians if degree ad *deg
  G4double radius = 5*mm; // Interaction spot radius 
  G4double theta = std::atan(radius /(distance) );
  G4double cosThetaMax = std::cos(theta);
  G4double cosThetaMin = 1.0; // Theta = 0
  G4double ux = 0,
           uy = 0,
           uz = 0;

  G4double cosTheta = cosThetaMin - G4UniformRand()*(cosThetaMin - cosThetaMax);
  G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
  G4double phi = twopi*G4UniformRand();
  ux = sinTheta*std::cos(phi);
  uy = sinTheta*std::sin(phi);
  uz = cosTheta;


  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(uy,uz,ux));
  fParticleGun->SetParticleEnergy(fE_particle);
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double MULTIPASSPrimaryGeneratorAction::sample_time(G4double tau1,
                                                                 G4double tau2)
{
  // From the G4Scintillation code

  // tau1: rise time 
  // tau2: decay time

  // Loop checking, 07-Aug-2015, Vladimir Ivanchenko
        while(1) {
          // two random numbers
          G4double ran1 = G4UniformRand();
          G4double ran2 = G4UniformRand();
          //
          // exponential distribution as envelope function: very efficient
          //
          G4double d = (tau1+tau2)/tau2;
          // make sure the envelope function is
          // always larger than the bi-exponential
          G4double t = -1.0*tau2*std::log(1-ran1);
          G4double gg = d*single_exp(t,tau2);
          if (ran2 <= bi_exp(t,tau1,tau2)/gg) return t;
        }
        return -1.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSPrimaryGeneratorAction::SetTCModelParticle(G4String part) {
  this->fTheParticle=part;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double MULTIPASSPrimaryGeneratorAction::GetScanPosX() {
  return -1* (fDetector->GetScintX()/mm)/2 + fScanOffsetX;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double MULTIPASSPrimaryGeneratorAction::GetScanPosY() {
  return -1* (fDetector->GetScintY()/mm)/2 + fScanOffsetY;
}