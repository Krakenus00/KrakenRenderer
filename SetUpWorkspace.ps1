<#

╔═════════════════════════════════════════════════════════════════╗
║              Copyright © 2022 Mykhailo Mushynskyi               ║
╟─────────────────────────────────────────────────────────────────╢
║  This work is licensed under a GNU General Public License v3.0  ║
╚═════════════════════════════════════════════════════════════════╝

#>

$ErrorActionPreference = "Stop"
$CleanCache = $args[0]

function CreateDir
{
    param ($DirName)

    if($CleanCache -eq "--clean") {
        Remove-Item $DirName -Recurse -Force
    }

    if (!(Test-Path $DirName)) {
        New-Item $DirName -ItemType Directory
    }
}

function ShowDialog
{
    param ($Message)
    
    Add-Type -AssemblyName PresentationFramework
    [System.Windows.MessageBox]::Show("$Message")
}

function CheckAvailability
{
    param ($ToolName)
    
    cmd.exe /c "$ToolName --version"
    if(!$?) {
        ShowDialog "$ToolName not found on your system. Make sure it is enabled in PATH and try again."
        Exit
    }
}

Write-Output "`n-- Checking tools availability..."
CheckAvailability git
CheckAvailability cmake

Write-Output "`n-- Pulling git submodules........"
git submodule update --remote --merge

Write-Output "`n-- Building the project.........."
CreateDir .\Project
CreateDir .\Build
Write-Output "`n"
Push-Location .\Project
cmake ..
Pop-Location
