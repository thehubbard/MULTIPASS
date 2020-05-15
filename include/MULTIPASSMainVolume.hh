//
// File:         MULTIPASS_Det/include/MULTIPASSMainVolume.hh
// Description:  MainVolume header for MULTIPASS simulation. Based on 
//               original code from Geant4 optical/LXe from 10.4.p01
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
#ifndef MULTIPASSMainVolume_H
#define MULTIPASSMainVolume_H 1

#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4OpticalSurface.hh"

#include "MULTIPASSDetectorConstruction.hh"

class MULTIPASSMainVolume : public G4PVPlacement
{
  public:

    MULTIPASSMainVolume(G4RotationMatrix *pRot,
                 const G4ThreeVector &tlate,
                 G4LogicalVolume *pMotherLogical,
                 G4bool pMany,
                 G4int pCopyNo,
                 MULTIPASSDetectorConstruction* c);

    G4LogicalVolume* GetLogPhotoCath() {return fPhotocath_log;}
    G4LogicalVolume* GetLogScint()     {return fScint_log;}

    std::vector<G4ThreeVector> GetPmtPositions() {return fPmtPositions;}

  private:

    void VisAttributes();
    void SurfaceProperties();

    void PlacePMTs(G4LogicalVolume* pmt_Log,
                   G4RotationMatrix* rot,
                   G4double &a, G4double &b, G4double da,
                   G4double db, G4double amin, G4double bmin,
                   G4int na, G4int nb,
                   G4double &x, G4double &y, G4double &z, G4int &k);

    void CopyValues();

    MULTIPASSDetectorConstruction* fConstructor;

    G4double fScint_x;
    G4double fScint_y;
    G4double fScint_z;
    G4double fD_mtl;
    G4int fNx;
    G4int fNy;
    G4int fNz;
    G4double fOuterRadius_pmt;
    G4double fRefl;

    //Basic Volumes
    //
    G4Box* fScint_box;
    G4Box* fHousing_box;
    G4Tubs* fPmt;
    G4Tubs* fPhotocath;
    // G4Box* fPmt;
    // G4Box* fPhotocath;


    // Logical volumes
    //
    G4LogicalVolume* fScint_log;
    G4LogicalVolume* fHousing_log;
    G4LogicalVolume* fPmt_log;
    G4LogicalVolume* fPhotocath_log;

    // Sensitive Detectors positions
    std::vector<G4ThreeVector> fPmtPositions;

};

#endif
