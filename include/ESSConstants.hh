#ifndef B5Constants_h
#define B5Constants_h 1

#include <G4SystemOfUnits.hh>
#include <globals.hh>

constexpr G4int kNofSensorColumns = 97;
constexpr G4int kNofSensorRows = 61;
constexpr G4int kNofPixels = kNofSensorColumns * kNofSensorRows;
constexpr G4double kPixelSize = 12.0 * um;

#endif