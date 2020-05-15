# Change Log
All notable changes to this project will be documented in this file.

## [Unreleased]
### Added
### Changed
### Deprecated
### Removed
### Fixed
### Security

Added for new features.
Changed for changes in existing functionality.
Deprecated for once-stable features removed in upcoming releases.
Removed for deprecated features removed in this release.
Fixed for any bug fixes.
Security to invite users to upgrade in case of vulnerabilities.

Dates: YYYY-MM-DD https://xkcd.com/1179/

=====================================================================
## Known Bugs

### Double counting in MT mode:
 - Reflections are double, triple etc counted unknown cause.

### Vis issue in batch mode
 - WARNING: G4VisManager::IsValidView(): Attempt to draw when no graphics system
  has been instantiated.  Use "/vis/open" or "/vis/sceneHandler/create".
  Alternatively, to avoid this message, suppress instantiation of vis
  manager (G4VisExecutive) and ensure drawing code is executed only if
  G4VVisManager::GetConcreteInstance() is non-zero.

### No onscreen visualisation in MT mode
 - Particle tracks etc. do not appear in MT mode, they do in
   sequential mode. Observed in 10.4.1 and 10.3.1 etc.

### EventAction reconstruction:
 - Currently list position of interaction at (0,0,0) this is incorrect due to 
   incorrect passing of PMT position in PMTSD.

### LXE Bugs
 - Incorrect #include gaurd in LXeWLSFiber.hh (Fixed on bugzilla)
 - Incorrect naming of G4LogicalSkinSurface("housing_surf") in MainVolume 
   it is set to "photocath_surf" which is incorrect
 - Position reconstruction in eventAction also does not work.

### Seg fault
 - boundaryStatus=boundary->GetStatus();
 - when run in Gamma runmode. Might be caused by optical photon generated in PGA

=====================================================================
## ToDos
 - 

=====================================================================
### Notes:
 - Gammas event information increase escape energy may now be wrong as 
   gammas are no longer just killed

  - MT SEGFAULTS trying to use get functions for primaries no idea why

=====================================================================
## 2020-04-02 
### Added 
### Changed
### Deprecated
### Removed
 - Code clean up completed
### Fixed
### Security

## 2020-03-18 
### Added 
### Changed
 - EJ276 code is now MULTIPASS example code

### Deprecated
### Removed
### Fixed
### Security


