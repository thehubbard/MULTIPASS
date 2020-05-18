# About MULTIPASS

MULtiple TIme constant Psd scintillATion emiSSion example - MULTIPASS, 
Is an example code demonstrating scintillation in Geant4 with multiple time constants for use with scintillators which exhibit pulse shape discrimination (PSD). It is based off of the code for simulating the EJ-276 detector system in Michael Hubbard’s PhD thesis.

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
