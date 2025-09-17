#!/bin/bash

# Exit on error
set -e

SCRIPTDIR=$(/usr/bin/dirname $0)
cd "$SCRIPTDIR"

SDK_DIR=../../../SDK
DEPLOY_FOLDER=../deploy

# If SomeIP deployment is used for communication then the SomeIP Daemon should executed 
# before the application executables.
# the following commands will deploy the SomeIP daemon and the necessary configuration 
# in the same folder that the application executables are deployed.
echo "Copying Someip daemon and configuration files..."
mkdir -p ${DEPLOY_FOLDER}/opt/amsr_someipd_daemon/bin/
cp -p ${SDK_DIR}/opt/amsr_someipd_daemon/bin/amsr_someipd_daemon -t ${DEPLOY_FOLDER}/opt/amsr_someipd_daemon/bin

mkdir -p ${DEPLOY_FOLDER}/opt/amsr_someipd_daemon/etc/
cp -p ${SDK_DIR}/opt/amsr_someipd_daemon/etc/logging_config.json -t ${DEPLOY_FOLDER}/opt/amsr_someipd_daemon/etc
cp -p ${SDK_DIR}/opt/amsr_someipd_daemon/etc/logging_config.json.validation -t ${DEPLOY_FOLDER}/opt/amsr_someipd_daemon/etc

# Create configuration file with the location of the someip config files of the executables
echo "{
    \"applications\": [
        \"../SpeedMonitorExecutable/etc/someip_config.json\",
        \"../WarningDisplayExecutable/etc/someip_config.json\"
    ]
}" > ${DEPLOY_FOLDER}/opt/amsr_someipd_daemon/etc/someipd-posix.json

# Start the someip daemon
echo "Starting SomeIP daemon..."
cd $DEPLOY_FOLDER/opt/amsr_someipd_daemon
export AMSR_PROCESS_SHORT_NAME_PATH="/vector/someipdaemon/someipd"
./bin/amsr_someipd_daemon -c "./etc/someipd-posix.json"
