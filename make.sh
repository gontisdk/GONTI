#!/bin/bash

if command -v tree &> /dev/null; then
    tree -a --noreport --charset=ascii > tree.txt
fi

clear

echo "What do you want to do?"
echo "1 or b - Build engine"
echo "2 or t - Build testbed"
echo "3 or d - Debug testbed"
read -p "Enter command (Build/Testbed/Debug): " userInput

userInput=$(echo "$userInput" | tr '[:upper:]' '[:lower:]')

case "$userInput" in
  "build" | "b" | "1")
    # Build engine
    cd GONTI-SDK/Build/Scripts/linux/ || exit
    ./main.sh
    ;;
  "testbed" | "t" | "2")
    # Build testbed
    bash -c "cd Testbed/BuildScripts/linux/ && ./build.sh"
    ;;
  "debug" | "d" | "3")
    # Debug testbed
    cd Testbed/BuildScripts/linux/ || exit
    ./Debug.sh
    ;;
  *)
    echo "Invalid value, exiting..."
    exit 0
    ;;
esac

exit 0