//
// File:         MULTIPASS_Det/include/MULTIPASSScintHit.hh
// Description:  ScintHit header for MULTIPASS simulation. Based on 
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
//
#ifndef MULTIPASSScintHit_h
#define MULTIPASSScintHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

#include "tls.hh"

class MULTIPASSScintHit : public G4VHit
{
  public:

    MULTIPASSScintHit();
    MULTIPASSScintHit(G4VPhysicalVolume* pVol);
    virtual ~MULTIPASSScintHit();
    MULTIPASSScintHit(const MULTIPASSScintHit &right);
    const MULTIPASSScintHit& operator=(
                                          const MULTIPASSScintHit &right);
    G4int operator==(const MULTIPASSScintHit &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);
 
    virtual void Draw();
    virtual void Print();

    inline void SetEdep(G4double de) { fEdep = de; }
    inline void AddEdep(G4double de) { fEdep += de; }
    inline G4double GetEdep() { return fEdep; }

    inline void SetPos(G4ThreeVector xyz) { fPos = xyz; }
    inline G4ThreeVector GetPos() { return fPos; }

    inline const G4VPhysicalVolume * GetPhysV() { return fPhysVol; }
    
    inline void SetTime(G4double t) {fTime=t;};
    inline G4double GetTime() {return fTime;};


  private:
    G4double fEdep;
    G4ThreeVector fPos;
    const G4VPhysicalVolume* fPhysVol;
    G4double fTime;

};

typedef G4THitsCollection<MULTIPASSScintHit> 
                                              MULTIPASSScintHitsCollection;

extern G4ThreadLocal G4Allocator<MULTIPASSScintHit>* 
                                                MULTIPASSScintHitAllocator;

inline void* MULTIPASSScintHit::operator new(size_t)
{
  if(!MULTIPASSScintHitAllocator)
      MULTIPASSScintHitAllocator = new G4Allocator<MULTIPASSScintHit>;
  return (void *) MULTIPASSScintHitAllocator->MallocSingle();
}

inline void MULTIPASSScintHit::operator delete(void *aHit)
{
  MULTIPASSScintHitAllocator->FreeSingle((MULTIPASSScintHit*) aHit);
}

#endif
