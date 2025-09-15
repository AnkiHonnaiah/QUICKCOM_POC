#!/bin/bash
set -e

# Enable multicast for IPC communication
ip link set lo multicast on
ip route add ff01::0/16 dev lo

if [ $# -gt 0 ]; then
    exec "$@"
fi

