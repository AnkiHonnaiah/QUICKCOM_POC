#!/bin/bash

# Exit on error
set -e

SCRIPTDIR=$(/usr/bin/dirname $0)
cd "$SCRIPTDIR"

# Modify paths to match your environment
SDK_DIR=../../../SDK
MODELBUILDER_DIR=../../../ModelBuilder
MODELCONFIG_DIR=../Model/Config
SRC_GEN_DIR=../tmp/src-gen


# Do not modify
"$SDK_DIR/CLI/Core/DVACfgCmd" \
    -data "../tmp/workspace" \
    --bswPackageRootPath "$SDK_DIR" \
    --scriptLocations  "$MODELBUILDER_DIR" \
    --scriptTask "BuildModel" \
    --taskArgs "--jsonConfigRootDir "$MODELCONFIG_DIR" --generationOutputDir "$SRC_GEN_DIR" "
