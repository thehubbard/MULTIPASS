//
// File:         MULTIPASS_Det/include/
//                                        MULTIPASSUserTrackInformation.hh
// Description:  UserTrackInformation header for MULTIPASS simulation. 
//               Based on original code from Geant4 optical/LXe from 10.4.p01
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

#ifndef MULTIPASSUserTrackInformation_h
#define MULTIPASSUserTrackInformation_h 1

#include "G4VUserTrackInformation.hh"
#include "globals.hh"

#include "G4ThreeVector.hh"
#include "G4Track.hh"
// #include "G4Allocator.hh"


enum MULTIPASSTrackStatus { active=1, hitPMT=2, absorbed=4, 
                                              boundaryAbsorbed=8, inactive=14};

/*MULTIPASSTrackStatus:
  active: still being tracked
  hitPMT: stopped by being detected in a PMT
  absorbed: stopped by being absorbed with G4OpAbsorbtion
  boundaryAbsorbed: stopped by being aborbed with G4OpAbsorbtion
  inactive: track is stopped for some reason
   -This is the sum of all stopped flags so can be used to remove stopped flags
 
*/

class MULTIPASSUserTrackInformation : public G4VUserTrackInformation
{
  public:

    MULTIPASSUserTrackInformation();
    MULTIPASSUserTrackInformation(const G4Track* aTrack);
    virtual ~MULTIPASSUserTrackInformation();

    //Sets the track status to s (does not check validity of flags)
    void SetTrackStatusFlags(int s){fStatus=s;}
    //Does a smart add of track status flags (disabling old flags that conflict)
    //If s conflicts with itself it will not be detected
    void AddTrackStatusFlag(int s);
 
    int GetTrackStatus()const {return fStatus;}
 
    void IncReflections(){fReflections++;}

    G4int GetReflectionCount()const {return fReflections;}

    void IncCompt(){fComptScat++;}
    G4int GetComptonScatters()const {return fComptScat;}

    void SetForceDrawTrajectory(G4bool b){fForcedraw=b;}
    G4bool GetForceDrawTrajectory(){return fForcedraw;}

    inline G4ThreeVector GetOriginalPosition() const {return originalPosition;}
    inline G4ThreeVector GetOriginalMomentum() const {return originalMomentum;}

    inline virtual void Print() const{};

  private:

    int fStatus;
    G4int fReflections;
    G4bool fForcedraw;
    G4int fComptScat;

    G4ThreeVector         originalPosition;
    G4ThreeVector         originalMomentum;
};

// extern G4ThreadLocal
//  G4Allocator<MULTIPASSUserTrackInformation> * aTrackInformationAllocator;

// inline void* MULTIPASSUserTrackInformation::operator new(size_t)
// {
//   if(!aTrackInformationAllocator)
//     aTrackInformationAllocator = new G4Allocator<MULTIPASSUserTrackInformation>;
//   return (void*)aTrackInformationAllocator->MallocSingle();
// }

// inline void MULTIPASSUserTrackInformation::operator delete(void *aTrackInfo)
// { aTrackInformationAllocator->FreeSingle((MULTIPASSUserTrackInformation*)aTrackInfo);}

#endif
