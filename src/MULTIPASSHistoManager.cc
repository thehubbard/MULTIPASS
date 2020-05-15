//
// File:         MULTIPASS/src/MULTIPASSHistoManager.cc
// Description:  MULTIPASSHistoManager for MULTIPASS simulation.
//
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

#include "MULTIPASSHistoManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"    // For units keV etc.
#include "G4PhysicalConstants.hh" // for Pi
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSHistoManager::MULTIPASSHistoManager()
{
  fAnalysisManager = 0;
  fFileName = "";
  fNEvents = 1;
  fAnalysisManager = G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MULTIPASSHistoManager::~MULTIPASSHistoManager()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSHistoManager::InitOpticalMode() 
{
  // Original runaction intialisation:
  fAnalysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << fAnalysisManager->GetType() << G4endl;

  
  fAnalysisManager->SetVerboseLevel(1);
  // ntuple merging is only possible with ROOT output
  fAnalysisManager->SetNtupleMerging(true);
  
  fAnalysisManager->SetFirstHistoId(0);
  
  fAnalysisManager->SetHistoDirectoryName("Histograms");
  fAnalysisManager->SetNtupleDirectoryName("ntuple");

  // Creating histograms
  // 1D Hist name, title, nbins, xmin, xmax
  fAnalysisManager->CreateH1("Edep","Energy (MeV) deposited in Scint",
                                                              100,0.,1000.*keV);
  fAnalysisManager->CreateH1("DetPhot","Detected Photons",
                                                              7000,1.,7000.);
  // 2
  fAnalysisManager->CreateH1("reflNo","Number of reflections",
                                                            500,0.,500.);
  // 3
  fAnalysisManager->CreateH1("localtime","Local times",
                                                            1000,0.,500.*ns);
  //4
  fAnalysisManager->CreateH1("pathlen","Optical path length (mm)",
                                                            10000,0.,10000.*mm);


  //
  // ID = 0
  fAnalysisManager->CreateH2("pulses","TimeArrival:Events",1000,0,1000*ns, 
                                                    fNEvents,0,fNEvents);

  fAnalysisManager->CreateH2("OpEmThetaPhi","Optical Emission Theta and Phi",
                                                360,            0., CLHEP::pi,
                                                720, -1*CLHEP::pi, CLHEP::pi);



  
  // Creating ntuple
  // Id = 0
  // ntuple name and title
  fAnalysisManager->CreateNtuple("EventInformation", "Event Information");
  fAnalysisManager->CreateNtupleIColumn("eventID");
  fAnalysisManager->CreateNtupleDColumn("eDep");
  fAnalysisManager->CreateNtupleIColumn("scintPhotons");
  fAnalysisManager->CreateNtupleIColumn("cerenPhotons");
  fAnalysisManager->CreateNtupleIColumn("totalPhotons"); // scint + ceren
  fAnalysisManager->CreateNtupleIColumn("reflections");
  fAnalysisManager->CreateNtupleIColumn("opAbsorption");
  fAnalysisManager->CreateNtupleIColumn("opBoundaryAbsorption");
  fAnalysisManager->FinishNtuple();

  /*
  // Id = 1
  fAnalysisManager->CreateNtuple("Ptimes", "Pulse Times");
  fAnalysisManager->CreateNtupleIColumn("EventID");
  fAnalysisManager->CreateNtupleDColumn("TimeArrival");// Global time
  fAnalysisManager->CreateNtupleIColumn("ReflectionCount");
  fAnalysisManager->CreateNtupleDColumn("Energy");
  fAnalysisManager->CreateNtupleDColumn("Wavelength");
  fAnalysisManager->CreateNtupleIColumn("pmtID");
  fAnalysisManager->CreateNtupleDColumn("LocalTime");// Local time
  fAnalysisManager->CreateNtupleDColumn("PathLength");
  fAnalysisManager->CreateNtupleDColumn("StartMomX");
  fAnalysisManager->CreateNtupleDColumn("StartMomY");
  fAnalysisManager->CreateNtupleDColumn("StartMomZ");

  fAnalysisManager->CreateNtupleDColumn("GeneratedTime"); // Global - Local time

  fAnalysisManager->FinishNtuple();


  // Id = 2
  fAnalysisManager->CreateNtuple("Compt", "Compton Scatters");
  fAnalysisManager->CreateNtupleIColumn("EventID");
  fAnalysisManager->CreateNtupleDColumn("Xpos"); 
  fAnalysisManager->CreateNtupleDColumn("Ypos");
  fAnalysisManager->CreateNtupleDColumn("Zpos");
  fAnalysisManager->FinishNtuple();
  //
  
  // Id = 3
  fAnalysisManager->CreateNtuple("GammaInt", "Gamma Interactions");
  fAnalysisManager->CreateNtupleIColumn("EventID");
  fAnalysisManager->CreateNtupleDColumn("Xpos_mm"); 
  fAnalysisManager->CreateNtupleDColumn("Ypos_mm");
  fAnalysisManager->CreateNtupleDColumn("Zpos_mm");
  fAnalysisManager->CreateNtupleDColumn("Time_ns");
  fAnalysisManager->CreateNtupleIColumn("InteractionID");
  fAnalysisManager->FinishNtuple();
  //
  */
  

  // BeginOfRunAction   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSHistoManager::BeginOfRun()
{
    
  fAnalysisManager->SetFileName(fFileName);
  fAnalysisManager->OpenFile(); 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MULTIPASSHistoManager::EndOfRun(G4bool isMastery)
{
    if ( fAnalysisManager->GetH1(0) ) { // Histogram counting started from zero
    G4cout << G4endl << " ----> print histograms statistic ";
    if(isMastery) {
      G4cout << "for the entire run " << G4endl << G4endl; 
    }
    else {
      G4cout << "for the local thread " << G4endl << G4endl; 
    }
    
    G4cout << " Escint : mean = " 
       << G4BestUnit(fAnalysisManager->GetH1(0)->mean(), "Energy") 
       << " rms = " 
       << G4BestUnit(fAnalysisManager->GetH1(0)->rms(),  "Energy") << G4endl;
    
    
  }
    // save histograms & ntuple
    //
    //
    fAnalysisManager->Write();
    fAnalysisManager->CloseFile();
    delete fAnalysisManager;
}