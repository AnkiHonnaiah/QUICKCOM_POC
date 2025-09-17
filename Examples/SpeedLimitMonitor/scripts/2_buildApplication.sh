#!/bin/bash

# Exit on error
set -e

SCRIPTDIR=$(/usr/bin/dirname $0)
cd "$SCRIPTDIR"

SOURCEDIR="../Cpp/SpeedLimitWarningApplication"
BUILDDIR="../tmp/build"

cmake -S $SOURCEDIR -B $BUILDDIR
cmake --build $BUILDDIR --parallel 4

# Install the application where it is specififed in the cmake file
cmake --install $BUILDDIR
