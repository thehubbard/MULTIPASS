//
// File:         MULTIPASS_Det/include/MULTIPASSTrajectory.hh
// Description:  Trajectory header for MULTIPASS simulation. Based on 
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
#ifndef MULTIPASSTrajectory_h
#define MULTIPASSTrajectory_h 1

#include "G4Trajectory.hh"
#include "G4Allocator.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4TrajectoryPoint.hh"
#include "G4Track.hh"
#include "G4Step.hh"

class G4Polyline;                   // Forward declaration.

class MULTIPASSTrajectory : public G4Trajectory
{
  public:

    MULTIPASSTrajectory();
    MULTIPASSTrajectory(const G4Track* aTrack);
    MULTIPASSTrajectory(MULTIPASSTrajectory &);
    virtual ~MULTIPASSTrajectory();
 
    virtual void DrawTrajectory() const;
 
    inline void* operator new(size_t);
    inline void  operator delete(void*);

    void SetDrawTrajectory(G4bool b){fDrawit=b;}
    void WLS(){fWls=true;}
    void SetForceDrawTrajectory(G4bool b){fForceDraw=b;}
    void SetForceNoDrawTrajectory(G4bool b){fForceNoDraw=b;}

  private:

    G4bool fWls;
    G4bool fDrawit;
    G4bool fForceNoDraw;
    G4bool fForceDraw;
    G4ParticleDefinition* fParticleDefinition;
};

extern G4ThreadLocal G4Allocator<MULTIPASSTrajectory>* 
                                              MULTIPASSTrajectoryAllocator;

inline void* MULTIPASSTrajectory::operator new(size_t)
{
  if(!MULTIPASSTrajectoryAllocator)
      MULTIPASSTrajectoryAllocator = 
                                      new G4Allocator<MULTIPASSTrajectory>;
  return (void*)MULTIPASSTrajectoryAllocator->MallocSingle();
}

inline void MULTIPASSTrajectory::operator delete(void* aTrajectory)
{
  MULTIPASSTrajectoryAllocator->FreeSingle(
                                        (MULTIPASSTrajectory*)aTrajectory);
}

#endif
