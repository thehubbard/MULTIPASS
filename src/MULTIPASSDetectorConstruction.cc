//
// File:         MULTIPASS_Det/src/MULTIPASSDetectorConstruction.cc
// Description:  DetectorConstruction for MULTIPASS simulation. Based on
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
#include "MULTIPASSDetectorConstruction.hh"
#include "MULTIPASSPMTSD.hh"
#include "MULTIPASSScintSD.hh"
#include "MULTIPASSDetectorMessenger.hh"
#include "MULTIPASSMainVolume.hh"

#include "G4SDManager.hh"
#include "G4RunManager.hh"

#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4OpticalSurface.hh"
#include "G4MaterialTable.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4UImanager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4MaterialPropertyVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSDetectorConstruction::MULTIPASSDetectorConstruction()
: fEJ276_mpt(NULL)
{
  fExperimentalHall_box = NULL;
  fExperimentalHall_log = NULL;
  fExperimentalHall_phys = NULL;

  fAl = fAir = fVacuum = fGlass = NULL;
 
  fMaterialsCreated = false;
  fEJ276 = NULL;

  fN = fO = fC = fH = NULL;

  SetDefaults();

  fDetectorMessenger = new MULTIPASSDetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSDetectorConstruction::~MULTIPASSDetectorConstruction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSDetectorConstruction::DefineMaterials(){
  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;



  //***Elements
  fH = new G4Element("H", "H", z=1., a=1.008*g/mole);
  fC = new G4Element("C", "C", z=6., a=12.0107*g/mole);
  fN = new G4Element("N", "N", z=7., a= 14.01*g/mole);
  fO = new G4Element("O"  , "O", z=8., a= 16.00*g/mole);

  //***Materials
  //Aluminum
  fAl = new G4Material("Al",z=13.,a=26.98*g/mole,density=2.7*g/cm3);

   G4double Al_Energy[]    = { 2.9*eV , 2.95*eV };
  const G4int Alnum = sizeof(Al_Energy)/sizeof(G4double);
  G4double Al_RIND[]  = { 0.54, 0.54 };
  // https://www.filmetrics.com/refractive-index-database/Al/Aluminium
  // https://refractiveindex.info/?shelf=main&book=Al&page=Rakic                                      
  assert(sizeof(Al_RIND) == sizeof(Al_Energy));

  G4MaterialPropertiesTable* Al_mt = new G4MaterialPropertiesTable();
  Al_mt->AddProperty("RINDEX", Al_Energy, Al_RIND,  Alnum);
  fAl->SetMaterialPropertiesTable(Al_mt);


  //Vacuum
  fVacuum = new G4Material("Vacuum",z=1.,a=1.01*g/mole,
                          density=universe_mean_density,kStateGas,0.1*kelvin,
                          1.e-19*pascal);
  //Air
  fAir = new G4Material("Air", density= 1.29*mg/cm3, 2);
  fAir->AddElement(fN, 70*perCent);
  fAir->AddElement(fO, 30*perCent);
  //Glass
  fGlass = new G4Material("Glass", density=1.032*g/cm3,2);
  fGlass->AddElement(fC,91.533*perCent);
  fGlass->AddElement(fH,8.467*perCent);

 
  //***Material properties tables

  G4double lxe_Energy[]    = { 7.0*eV , 7.07*eV, 7.14*eV };
  const G4int lxenum = sizeof(lxe_Energy)/sizeof(G4double);
 
  G4double glass_RIND[]={1.49,1.49,1.49};
  assert(sizeof(glass_RIND) == sizeof(lxe_Energy));
  G4double glass_AbsLength[]={420.*cm,420.*cm,420.*cm};
  assert(sizeof(glass_AbsLength) == sizeof(lxe_Energy));
  G4MaterialPropertiesTable *glass_mt = new G4MaterialPropertiesTable();
  glass_mt->AddProperty("ABSLENGTH",lxe_Energy,glass_AbsLength,lxenum);
  glass_mt->AddProperty("RINDEX",lxe_Energy,glass_RIND,lxenum);
  fGlass->SetMaterialPropertiesTable(glass_mt);

  G4double vacuum_Energy[]={2.0*eV,7.0*eV,7.14*eV};
  const G4int vacnum = sizeof(vacuum_Energy)/sizeof(G4double);
  G4double vacuum_RIND[]={1.,1.,1.};
  assert(sizeof(vacuum_RIND) == sizeof(vacuum_Energy));
  G4MaterialPropertiesTable *vacuum_mt = new G4MaterialPropertiesTable();
  vacuum_mt->AddProperty("RINDEX", vacuum_Energy, vacuum_RIND,vacnum);
  fVacuum->SetMaterialPropertiesTable(vacuum_mt);
  fAir->SetMaterialPropertiesTable(vacuum_mt);//Give air the same rindex

//    _____   ___  _____  __________ 
//   |  ___| |_  |/ __  \|___  / ___|
//   | |__     | |`' / /'   / / /___ 
//   |  __|    | |  / /    / /| ___ \
//   | |___/\__/ /./ /___./ / | \_/ |
//   \____/\____/ \_____/\_/  \_____/

  // Data from:
  // https://eljentechnology.com/products/plastic-scintillators/ej-200-ej-204-ej-208-ej-212
  fEJ276 = new G4Material("EJ276", density= 1.096 *g/cm3, 2,kStateSolid);
  G4double nC = 4.906; // No. of C Atoms per cm3 (x1022)
  G4double nH = 4.546; // No. of H Atoms per cm3 (x1022)
  fEJ276->AddElement(fH, nH/(nH+nC) );
  fEJ276->AddElement(fC, nC/(nH+nC) );


  //***Material properties table

  // Refractive Index  (146 nm to 1570 nm)
  // Unknown max and min currently from 
  // https://github.com/murffer/DetectorSim
  const G4int nRindex = 2;
  G4double photonEnergyRindex[nRindex] = {8.5506*eV,0.7973*eV};
  G4double refractiveIndexMULTIPASS[nRindex]= {1.58, 1.58};

  // Bulk Absorption length
  // Taken asLight Attneuation Length from datasheet
  const G4int nAbsLength = 2;
  G4double photonEnergyAbsLength[nAbsLength] = {8.5506*eV,0.7973*eV};
  G4double absLengthMULTIPASS[nAbsLength]= {3.8*m, 3.8*m};

  // Emission spectra
  const G4int nSpec = 25;
  // Find a cleaner method than *eV !! 
  // Appears to default to keV hence bug in energy and wavelength output!
  // Spectrum is for EJ-200 need to add EJ-276
  G4double photonEnergySpec[25] = 
                  {3.263*eV, 3.204*eV, 3.146*eV, 3.091*eV, 3.038*eV, 2.987*eV,
                   2.937*eV, 2.888*eV, 2.842*eV, 2.797*eV, 2.753*eV, 2.710*eV,
                   2.670*eV, 2.630*eV, 2.591*eV, 2.554*eV, 2.517*eV, 2.481*eV,
                   2.447*eV, 2.413*eV, 2.381*eV, 2.349*eV, 2.318*eV, 2.288*eV,
                   2.259*eV};
  G4double specMULTIPASS[25] = 
                 {0.003, 0.02, 0.025, 0.08, 0.156, 0.373, 0.793,
                  0.988, 0.885, 0.763, 0.729, 0.667, 0.544, 0.411,
                  0.336, 0.276, 0.223, 0.168, 0.133, 0.099, 0.07,
                  0.064, 0.047, 0.031, 0.009};

  // Creating the materials property table and adding entries into
  // properties table
  fEJ276_mpt = new G4MaterialPropertiesTable();
  fEJ276_mpt->AddProperty("RINDEX",
                      photonEnergyRindex,refractiveIndexMULTIPASS,nRindex);
  fEJ276_mpt->AddProperty("ABSLENGTH",
                      photonEnergyAbsLength,absLengthMULTIPASS,nAbsLength);
  
  fEJ276_mpt->AddProperty("FASTCOMPONENT",
                                    photonEnergySpec,specMULTIPASS,nSpec);


  fEJ276_mpt->AddConstProperty("SCINTILLATIONYIELD", 8600/MeV);

  const G4int numtc = 3;

  G4double dummyvar[3] = {0,1,2}; // must increase linearly

  G4double gamrat[3] = {0.74,0.14, 0.12};
  G4double gamtc[3] = {4.3*ns,18*ns, 140*ns};

  G4double neurat[3] = {0.58,0.18, 0.24};
  G4double neutc[3] = {4.5*ns,20*ns, 170*ns};

  // For simulating Gamma

  fEJ276_mpt->AddProperty("GAMMARATIOS",dummyvar,gamrat,numtc);
  fEJ276_mpt->AddProperty("GAMMATC",dummyvar,gamtc,numtc);
  // Fail safe for electron events
  fEJ276_mpt->AddProperty("NEUTRONRATIOS",dummyvar,gamrat,numtc);
  fEJ276_mpt->AddProperty("NEUTRONTC",dummyvar,gamtc,numtc);

  // For simulating Neutron

  // fEJ276_mpt->AddProperty("NEUTRONRATIOS",dummyvar,neurat,numtc);
  // fEJ276_mpt->AddProperty("NEUTRONTC",dummyvar,neutc,numtc);
  // // Fail safe for electron events
  // fEJ276_mpt->AddProperty("GAMMARATIOS",dummyvar,neurat,numtc);
  // fEJ276_mpt->AddProperty("GAMMATC",dummyvar,neutc,numtc);


  fEJ276_mpt->AddConstProperty("YIELDRATIO", 1.0);
  fEJ276_mpt->AddConstProperty("RESOLUTIONSCALE", 1.0); // Can be tuned
  fEJ276->SetMaterialPropertiesTable(fEJ276_mpt);


  // Unknown value. Current value left over from Polystyrene in LXe code
  fEJ276->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

  fMaterialsCreated=true;
  G4cout << "Materials Created" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* MULTIPASSDetectorConstruction::Construct(){

  if (fExperimentalHall_phys) {
     G4GeometryManager::GetInstance()->OpenGeometry();
     G4PhysicalVolumeStore::GetInstance()->Clean();
     G4LogicalVolumeStore::GetInstance()->Clean();
     G4SolidStore::GetInstance()->Clean();
     G4LogicalSkinSurface::CleanSurfaceTable();
     G4LogicalBorderSurface::CleanSurfaceTable();
  }
  if ( !fMaterialsCreated ) {
    DefineMaterials();
  }

  G4cout << "Constructing detector" << fMaterialsCreated << G4endl;
  return ConstructDetector();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* MULTIPASSDetectorConstruction::ConstructDetector()
{
  //The experimental hall walls are all 1m away from housing walls
  G4double expHall_x = fScint_x+fD_mtl+1.*m;
  G4double expHall_y = fScint_y+fD_mtl+1.*m;
  G4double expHall_z = fScint_z+fD_mtl+1.*m;

  // G4cout << "Corbyn Dallas: "<< fAir->GetName() << G4endl;
  //Create experimental hall
  fExperimentalHall_box
    = new G4Box("expHall_box",expHall_x,expHall_y,expHall_z);
  fExperimentalHall_log = new G4LogicalVolume(fExperimentalHall_box,
                                             fAir,"expHall_log",0,0,0);
  fExperimentalHall_phys = new G4PVPlacement(0,G4ThreeVector(),
                              fExperimentalHall_log,"expHall",0,false,0);

  fExperimentalHall_log->SetVisAttributes(G4VisAttributes::GetInvisible());

  //Place the main volume
  if(fMainVolumeOn){
    fMainVolume
      = new MULTIPASSMainVolume(0,G4ThreeVector(),fExperimentalHall_log,
                                                                  false,0,this);
  }


  return fExperimentalHall_phys;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSDetectorConstruction::ConstructSDandField() {

  if (!fMainVolume) return;

  // PMT SD

  if (!fPmt_SD.Get()) {
    //Created here so it exists as pmts are being placed
    G4cout << "Construction /MULTIPASSDet/pmtSD" << G4endl;
    MULTIPASSPMTSD* pmt_SD =
                           new MULTIPASSPMTSD("/MULTIPASSDet/pmtSD");
    fPmt_SD.Put(pmt_SD);

    pmt_SD->InitPMTs();

    pmt_SD->SetPmtPositions(fMainVolume->GetPmtPositions());

  }
  G4SDManager::GetSDMpointer()->AddNewDetector(fPmt_SD.Get());
  //sensitive detector is not actually on the photocathode.
  //processHits gets done manually by the stepping action.
  //It is used to detect when photons hit and get absorbed&detected at the
  //boundary to the photocathode (which doesnt get done by attaching it to a
  //logical volume.
  //It does however need to be attached to something or else it doesnt get
  //reset at the begining of events

  SetSensitiveDetector(fMainVolume->GetLogPhotoCath(), fPmt_SD.Get());

  // Scint SD

  if (!fScint_SD.Get()) {
    G4cout << "Construction /MULTIPASSDet/scintSD" << G4endl;
    MULTIPASSScintSD* scint_SD = 
                        new MULTIPASSScintSD("/MULTIPASSDet/scintSD");
    fScint_SD.Put(scint_SD);
  }
  G4SDManager::GetSDMpointer()->AddNewDetector(fScint_SD.Get());
  SetSensitiveDetector(fMainVolume->GetLogScint(), fScint_SD.Get());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSDetectorConstruction::SetSlabDimensions(G4double xdim) {
  this->fScint_x=xdim;
  this->fScint_y=xdim;
  this->fScint_z=xdim;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSDetectorConstruction::SetDimensions(G4ThreeVector dims) {
  this->fScint_x=dims[0];
  this->fScint_y=dims[1];
  this->fScint_z=dims[2];
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSDetectorConstruction::SetHousingThickness(G4double d_mtl) {
  this->fD_mtl=d_mtl;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSDetectorConstruction::SetNX(G4int nx) {
  this->fNx=nx;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSDetectorConstruction::SetNY(G4int ny) {
  this->fNy=ny;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSDetectorConstruction::SetNZ(G4int nz) {
  this->fNz=nz;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSDetectorConstruction::SetPMTRadius(G4double outerRadius_pmt)
{
  this->fOuterRadius_pmt=outerRadius_pmt;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSDetectorConstruction::SetDefaults() {

  //Resets to default values
  fD_mtl=1.5*mm; // Reflector thickness

  fScint_x = 254*mm;
  fScint_y = 25.4*mm;
  fScint_z = 254*mm;


  fNx = 1;
  fNy = 1;
  fNz = 0;

  fOuterRadius_pmt = 22*mm/2.;

  // Default 95% reflectivity
  fRefl=0.95;

  fMainVolumeOn=true;
  fMainVolume=NULL;


  G4UImanager::GetUIpointer()
    ->ApplyCommand("/MULTIPASS/detector/scintYieldFactor 1.");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSDetectorConstruction::SetHousingReflectivity(G4double r) {
  fRefl=r;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSDetectorConstruction::SetMainVolumeOn(G4bool b) {
  fMainVolumeOn=b;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSDetectorConstruction::SetMainScintYield(G4double y) {
  fEJ276_mpt->AddConstProperty("SCINTILLATIONYIELD",y/MeV);
}

