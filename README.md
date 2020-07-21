# About MULTIPASS

MULtiple TIme constant Psd scintillATion emiSSion example - MULTIPASS, 
Is an example code demonstrating scintillation in Geant4 with multiple time constants for use with scintillators which exhibit pulse shape discrimination (PSD). It is based on the code for simulating the EJ-276 detector system in Michael Hubbard’s PhD thesis. https://doi.org/10.15126/thesis.00857773 However, please note this is an amalgamation of several pieces of code used in the thesis to provide a minimal working example of the implementation of multiple time constant PSD code. The example itself has not been used in the thesis.

# Building
The MULTIPASS source code has been built and tested against Gean4.10.04.p01 other versions may work.
It is suggested you make a build directory for example ```build_MULTIPASS.```
Inside the build directory run:
```bash
cmake -DGeant4_DIR=path/to/geant4/install/directory path/to/MULTIPASS
```
then
```bash
make
```
this will produce an executable called ```MULTIPASS```

# Running
```
./MULTIPASS --help
  - a simulation software for MULTIPASS detector
  Usage:
  ./MULTIPASS [OPTION...]
  -m, --macro arg run macro
  -r, --runmode arg Run mode [Full, Gamma, Optics] (default: Optics)
  -h, --help Print help
```

# Data output
Depending on the run mode the data output changes. Within the HistoManager the data output it currently selected as .root, editing this file allows for other data formats to be selected. The output file name starts with the run mode and includes the number of events in the run. If the optics mode is used the emission location in terms of top right etc is stored in the name. For gamma and full modes, the x-y positional information is stored in the file name. ROOT histograms and ntuples are created for different variables depending on the mode. In short, the gamma mode produces energy deposition histograms whereas optics produces data associated with the optical photons. Full mode combines these two modes together.

# License 
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
If you do use this code for publication, please consider citing the code with the zenodo doi and my thesis 
[![DOI](https://img.shields.io/badge/DOI-0.15126%2Fthesis.00857773-blue)](https://doi.org/10.15126/thesis.00857773) 


