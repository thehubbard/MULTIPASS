//
// File:         MULTIPASS_Det/include/
//                                         MULTIPASSDetectorConstruction.hh
// Description:  DetectorConstruction header for MULTIPASS simulation. 
//               Based on original code from Geant4 optical/LXe from 10.4.p01
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

#ifndef MULTIPASSDetectorConstruction_H
#define MULTIPASSDetectorConstruction_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Box;
class G4Tubs;
class MULTIPASSMainVolume;

#include "G4Material.hh"
#include "MULTIPASSDetectorMessenger.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"

#include "MULTIPASSScintSD.hh"
#include "MULTIPASSPMTSD.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4Cache.hh"


class MULTIPASSDetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    MULTIPASSDetectorConstruction();
    virtual ~MULTIPASSDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    void SetSlabDimensions(G4double );
    void SetDimensions(G4ThreeVector );
    void SetHousingThickness(G4double );
    void SetNX(G4int );
    void SetNY(G4int );
    void SetNZ(G4int );
    void SetPMTRadius(G4double );
    void SetDefaults();

    //Get values
    G4int GetNX()const {return fNx;}
    G4int GetNY()const {return fNy;}
    G4int GetNZ()const {return fNz;}
    G4double GetScintX()const {return fScint_x;}
    G4double GetScintY()const {return fScint_y;}
    G4double GetScintZ()const {return fScint_z;}
    G4double GetHousingThickness()const {return fD_mtl;}
    G4double GetPMTRadius()const {return fOuterRadius_pmt;}
 
    void SetHousingReflectivity(G4double );
    G4double GetHousingReflectivity()const{return fRefl;}

    void SetMainVolumeOn(G4bool b);
    G4bool GetMainVolumeOn()const{return fMainVolumeOn;}

    void SetMainScintYield(G4double );

  private:

    void DefineMaterials();
    G4VPhysicalVolume* ConstructDetector();

    MULTIPASSDetectorMessenger* fDetectorMessenger;

    G4Box* fExperimentalHall_box;
    G4LogicalVolume* fExperimentalHall_log;
    G4VPhysicalVolume* fExperimentalHall_phys;

    G4bool fMaterialsCreated;
    //Materials & Elements
    G4Material* fAl;
    G4Element* fN;
    G4Element* fO;
    G4Material* fAir;
    G4Material* fVacuum;
    G4Element* fC;
    G4Element* fH;
    G4Material* fGlass;

    G4Material* fEJ276;

    //Geometry
    G4double fScint_x;
    G4double fScint_y;
    G4double fScint_z;
    G4double fD_mtl;
    G4int fNx;
    G4int fNy;
    G4int fNz;
    G4double fOuterRadius_pmt;
    G4double fRefl;
    G4bool fMainVolumeOn;

    MULTIPASSMainVolume* fMainVolume;

    
    G4MaterialPropertiesTable* fEJ276_mpt;

    //Sensitive Detectors
    G4Cache<MULTIPASSScintSD*> fScint_SD;
    G4Cache<MULTIPASSPMTSD*> fPmt_SD;

};

#endif
