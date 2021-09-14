#!/bin/bash

# ╔═════════════════════════════════════════════════════════════════╗
# ║              Copyright © 2022 Mykhailo Mushynskyi               ║
# ╟─────────────────────────────────────────────────────────────────╢
# ║  This work is licensed under a GNU General Public License v3.0  ║
# ╚═════════════════════════════════════════════════════════════════╝

CleanCache="$0"

function ShowDialog
{
    if [[ "$OSTYPE" == "darwin"* ]]; then
        osascript -e "tell app \"System Events\" to display dialog $1"
    else
        echo "Unsupported platform."
    fi
}

function CheckAvailability
{
    local state = 1
    $1 --version || $state = 0
    if [[$state -eq 1]]; then
        ShowDialog "$1 not found on your system. Make sure it is enabled in PATH and try again."
        exit 1
    fi
}

echo "\n-- Checking tools availability..."
CheckAvailability git
CheckAvailability cmake

echo "\n-- Pulling git submodules........"
git submodule update --remote --merge

echo "\n-- Building the project.........."
if [[$CleanCache == "--clean"]]; then
    rm -rf ./Project
    rm -rf ./Build
fi
mkdir -p ./Project
mkdir -p ./Build
echo "\n"
pushd ./Project
cmake ..
popd
