//
// File:         MULTIPASS_Det/include/MULTIPASSOpticalHit.hh
// Description:  MULTIPASSOpticalHit for MULTIPASS simulation. 
//               Tool for collecting optical photons at PMT surface
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

#ifndef MULTIPASSOpticalHit_h
#define MULTIPASSOpticalHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

#include "tls.hh"

class G4VTouchable;

class MULTIPASSOpticalHit : public G4VHit
{
  public:
 
    MULTIPASSOpticalHit();
    virtual ~MULTIPASSOpticalHit();
    MULTIPASSOpticalHit(const MULTIPASSOpticalHit &right);

    const MULTIPASSOpticalHit& operator=(
                                        const MULTIPASSOpticalHit &right);
    G4int operator==(const MULTIPASSOpticalHit &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);
 
    virtual void Draw();
    virtual void Print();

    inline void SetDrawit(G4bool b){fDrawit=b;}
    inline G4bool GetDrawit(){return fDrawit;}


    inline void SetPMTNumber(G4int n) { fPmtNumber = n; }
    inline G4int GetPMTNumber() { return fPmtNumber; }

    inline void SetPMTPhysVol(G4VPhysicalVolume* physVol)
                                                    {this->fPhysVol=physVol;}
    inline G4VPhysicalVolume* GetPMTPhysVol(){return fPhysVol;}

    inline void SetPMTPos(G4double x,G4double y,G4double z){
      fPmtPos=G4ThreeVector(x,y,z);
    }
 
    inline G4ThreeVector GetPMTPos(){return fPmtPos;}

    inline void SetOpticalPos(G4double x,G4double y,G4double z){
      fOptPos=G4ThreeVector(x,y,z);
    } 
    inline G4ThreeVector GetOpticalPos(){return fOptPos;}

    inline void SetLocalTime(G4double t) { fLocalTime = t; }
    inline G4double GetLocalTime() { return fLocalTime; }

    inline void SetGlobalTime(G4double t) { fGlobalTime = t; }
    inline G4double GetGlobalTime() { return fGlobalTime; }

    // The vertex is where this track was created
    inline G4double GetVertexTime() {return fGlobalTime - fLocalTime;}

    inline void SetReflections(G4int r) { fReflections = r; }
    inline G4double GetReflections() { return fReflections; }

    inline void SetPathlength(G4double p) { fPathlength = p; }
    inline G4double GetPathlength() { return fPathlength; }


    inline void SetEnergy(G4double e) { fEnergy = e; }
    inline G4double GetEnergy() { return fEnergy; }
    G4double GetWavelength();

    inline void SetStartMom(G4ThreeVector p) { fStartMom = p; }
    inline G4ThreeVector GetStartMom() { return fStartMom; }

  private:
    // PMT specific
    G4int fPmtNumber;
    G4ThreeVector fPmtPos;
    G4VPhysicalVolume* fPhysVol;
    G4bool fDrawit;

    // Optical photon specific
    G4ThreeVector fOptPos;
    G4double fLocalTime;
    G4double fGlobalTime;
    G4int fReflections;
    G4double fPathlength;
    G4double fEnergy;
    G4ThreeVector fStartMom;


};

typedef G4THitsCollection<MULTIPASSOpticalHit> 
                                            MULTIPASSOpticalHitsCollection;

extern G4ThreadLocal G4Allocator<MULTIPASSOpticalHit>* 
                                              MULTIPASSOpticalHitAllocator;

inline void* MULTIPASSOpticalHit::operator new(size_t){
  if(!MULTIPASSOpticalHitAllocator)
      MULTIPASSOpticalHitAllocator = 
                                      new G4Allocator<MULTIPASSOpticalHit>;
  return (void *) MULTIPASSOpticalHitAllocator->MallocSingle();
}

inline void MULTIPASSOpticalHit::operator delete(void *aHit){
  MULTIPASSOpticalHitAllocator->FreeSingle(
                                              (MULTIPASSOpticalHit*) aHit);
}

#endif
