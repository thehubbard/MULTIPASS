//
// File:         MULTIPASS_Det/include/MULTIPASSComptonHit.hh
// Description:  ComptHit header for MULTIPASS simulation.
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
#ifndef MULTIPASSComptonHit_h
#define MULTIPASSComptonHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

#include "tls.hh"

class MULTIPASSComptonHit : public G4VHit
{
  public:

    MULTIPASSComptonHit();
    virtual ~MULTIPASSComptonHit();
    MULTIPASSComptonHit(const MULTIPASSComptonHit &right);
    const MULTIPASSComptonHit& operator=(
                                        const MULTIPASSComptonHit &right);
    G4int operator==(const MULTIPASSComptonHit &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);
 
    virtual void Draw();
    virtual void Print();

    inline void SetEdep(G4double de) { fEdep = de; }
    inline void AddEdep(G4double de) { fEdep += de; }
    inline G4double GetEdep() { return fEdep; }

    inline void SetLocation(G4ThreeVector xyz) { fPos = xyz; }
    inline G4ThreeVector GetLocation() { return fPos; }

    inline void SetTime(G4double t) {fTime=t;};
    inline G4double GetTime() {return fTime;};
    
    inline void SetProcessID(G4int pid) {fProcID=pid;};
    inline G4int GetProcessID() {return fProcID;};


  private:
    G4double fEdep;
    G4ThreeVector fPos;
    G4double fTime;
    G4int fProcID;


};

typedef G4THitsCollection<MULTIPASSComptonHit> 
                                            MULTIPASSComptonHitsCollection;

extern G4ThreadLocal G4Allocator<MULTIPASSComptonHit>* 
                                              MULTIPASSComptonHitAllocator;

inline void* MULTIPASSComptonHit::operator new(size_t)
{
  if(!MULTIPASSComptonHitAllocator)
      MULTIPASSComptonHitAllocator = 
                                    new G4Allocator<MULTIPASSComptonHit>;
  return (void *) MULTIPASSComptonHitAllocator->MallocSingle();
}

inline void MULTIPASSComptonHit::operator delete(void *aHit)
{
  MULTIPASSComptonHitAllocator->FreeSingle(
                                              (MULTIPASSComptonHit*) aHit);
}

#endif
