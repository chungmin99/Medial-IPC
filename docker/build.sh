#!/usr/bin/env bash
set -e
set -u
SCRIPTROOT="$( cd "$(dirname "$0")" ; pwd -P )"
cd "${SCRIPTROOT}/.."

sudo docker build -t m-ipc -f docker/Dockerfile .
