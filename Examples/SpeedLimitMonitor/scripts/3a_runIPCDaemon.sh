#!/bin/bash

# Exit on error
set -e

SCRIPTDIR=$(/usr/bin/dirname $0)
cd "$SCRIPTDIR"

SDK_DIR=../../../SDK
DEPLOY_FOLDER=../deploy

# If IPC deployment is used for communication then the IPC service discovery Daemon should be executed 
# before the application executables.
# The following commands will deploy the IPC daemon and the necessary configuration 
# in the same folder that the application executables are deployed.
echo "Copying IPC service discovery daemon and configuration files..."
mkdir -p ${DEPLOY_FOLDER}/opt/amsr_ipcservicediscovery_daemon/bin/
cp -p ${SDK_DIR}/opt/amsr_ipcservicediscovery_daemon/bin/amsr_ipcservicediscovery_daemon -t ${DEPLOY_FOLDER}/opt/amsr_ipcservicediscovery_daemon/bin

mkdir -p ${DEPLOY_FOLDER}/opt/amsr_ipcservicediscovery_daemon/etc/
cp -p ${SDK_DIR}/opt/amsr_ipcservicediscovery_daemon/etc/logging_config.json -t ${DEPLOY_FOLDER}/opt/amsr_ipcservicediscovery_daemon/etc
cp -p ${SDK_DIR}/opt/amsr_ipcservicediscovery_daemon/etc/logging_config.json.validation -t ${DEPLOY_FOLDER}/opt/amsr_ipcservicediscovery_daemon/etc

# Start the IPC daemon
echo "Starting IPC daemon..."
cd $DEPLOY_FOLDER/opt/amsr_ipcservicediscovery_daemon
export AMSR_PROCESS_SHORT_NAME_PATH="/vector/example/amsr_ipc_service_discovery_daemon_process"
./bin/amsr_ipcservicediscovery_daemon
