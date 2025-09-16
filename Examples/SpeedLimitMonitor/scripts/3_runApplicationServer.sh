#!/bin/bash

# Exit on error
set -e

SCRIPTDIR=$(/usr/bin/dirname $0)
cd "$SCRIPTDIR"

DEPLOY_FOLDER=../deploy

cd $DEPLOY_FOLDER/opt/SpeedMonitorExecutable
export AMSR_PROCESS_SHORT_NAME_PATH="/AppDeployment/Prc_App_ModelBuilderApplication_SpeedMonitorExecutable"
./bin/SpeedMonitorExecutable
