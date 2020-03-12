#!/bin/bash

# Set up environment variables
USER="$(wrun powershell -command "& {&'echo' \$env:username}")" # MUST have outbash
USER=$(echo "$USER" | tr -d '\r') # Remove STUPID carriage return
DESKTOP="/mnt/c/Users/${USER}/Desktop"
DIR="/mnt/c/Users/${USER}/dev/ece391/qemu_win"
C_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

if [ ! -d "$DIR" ]; then
  echo Target directory $DIR does not exist. Please ensure this path is corrent and try again.
  exit
fi


# Copy over icons and make shortcuts
yes | cp -f icons/qdevel.ico "$DIR"
yes | cp -f icons/qtest.ico "$DIR"

# Remove old shortcuts
rm -f "${DIR}/testDebug.lnk"
rm -f "${DIR}/devel.lnk"

# Install updater
yes | cp -f update_scripts.ps1 "$DESKTOP"

# Call vb script to make shortcuts
RUN_PATH="${C_DIR}/shortcuts.vbs"
RUN_PATH="${RUN_PATH:6}"
RUN_PATH="C:$RUN_PATH"

wcmd $(echo $RUN_PATH)