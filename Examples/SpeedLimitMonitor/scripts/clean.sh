#!/bin/bash

# Exit on error
set -e

SCRIPTDIR=$(/usr/bin/dirname $0)
cd "$SCRIPTDIR"

rm -rf ../tmp
rm -rf ../deploy

exit 0
