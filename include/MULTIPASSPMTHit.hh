//
// File:         MULTIPASS_Det/include/MULTIPASSPMTHit.hh
// Description:  PMTHit header for MULTIPASS simulation. Based on original
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
#ifndef MULTIPASSPMTHit_h
#define MULTIPASSPMTHit_h 1

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

class MULTIPASSPMTHit : public G4VHit
{
  public:
 
    MULTIPASSPMTHit();
    virtual ~MULTIPASSPMTHit();
    MULTIPASSPMTHit(const MULTIPASSPMTHit &right);

    const MULTIPASSPMTHit& operator=(const MULTIPASSPMTHit &right);
    G4int operator==(const MULTIPASSPMTHit &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);
 
    virtual void Draw();
    virtual void Print();

    inline void SetDrawit(G4bool b){fDrawit=b;}
    inline G4bool GetDrawit(){return fDrawit;}

    inline void IncPhotonCount(){fPhotons++;}
    inline G4int GetPhotonCount(){return fPhotons;}

    inline void SetPMTNumber(G4int n) { fPmtNumber = n; }
    inline G4int GetPMTNumber() { return fPmtNumber; }

    inline void SetPMTPhysVol(G4VPhysicalVolume* physVol){
                                                        this->fPhysVol=physVol;}
    inline G4VPhysicalVolume* GetPMTPhysVol(){return fPhysVol;}

    inline void SetPMTPos(G4double x,G4double y,G4double z){
      fPos=G4ThreeVector(x,y,z);
    }
 
    inline G4ThreeVector GetPMTPos(){return fPos;}

  private:

    G4int fPmtNumber;
    G4int fPhotons;
    G4ThreeVector fPos;
    G4VPhysicalVolume* fPhysVol;
    G4bool fDrawit;

};

typedef G4THitsCollection<MULTIPASSPMTHit> MULTIPASSPMTHitsCollection;

extern G4ThreadLocal G4Allocator<MULTIPASSPMTHit>*
                                                  MULTIPASSPMTHitAllocator;

inline void* MULTIPASSPMTHit::operator new(size_t){
  if(!MULTIPASSPMTHitAllocator)
      MULTIPASSPMTHitAllocator = new G4Allocator<MULTIPASSPMTHit>;
  return (void *) MULTIPASSPMTHitAllocator->MallocSingle();
}

inline void MULTIPASSPMTHit::operator delete(void *aHit){
  MULTIPASSPMTHitAllocator->FreeSingle((MULTIPASSPMTHit*) aHit);
}

#endif
