# Geant4ESS

This Geant4 simulation tries to cover the case of interaction between proton losses and the critical parts of the IPMs.

Geometrie come from ESS LWU STEP file or are created from scratch with Geant4 primitives. STEP file is converted into many STL files thanks to FreeCAD. Then CADMesh library loads them during the DetectorConstruction phase.

![IPM LWU](data/scene.png)

## Requierements

- CMake >= version 3.6
- Geant4 >= version 10.04
- CADMesh >= version 2 (beta)
- FreeCAD 0.16
- ROOT6

## Build

```sh
mkdir build
cd build
cmake ..
make -jN
```

## Usage

To run in UI interactive mode:
```sh
./Geant4ESS
```

To execute a macro:
```sh
./Geant4ESS run.mac
```