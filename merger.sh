#!/bin/bash

set -B # For brace expansion

if [ $# -lt 2 ]; then
    echo "Usage: $0 <base filename> <number of file>"
    exit 1
fi

if [  "$1" -lt 2  ]; then
    echo "Usage: $0 <base filename> <number of file>"
    exit 1
fi

echo "Merge $2 $1 ROOT files from Geant4 simulations." 
eval hadd ${1%%.*}_merged.root ${1%%.*}_rank{0..$2}.root