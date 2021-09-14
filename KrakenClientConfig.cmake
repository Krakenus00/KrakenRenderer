#[[

╔═════════════════════════════════════════════════════════════════╗
║              Copyright © 2022 Mykhailo Mushynskyi               ║
╟─────────────────────────────────────────────────────────────────╢
║  This work is licensed under a GNU General Public License v3.0  ║
╚═════════════════════════════════════════════════════════════════╝

]]

set(KrakenClient_SrcDir ${CMAKE_CURRENT_LIST_DIR}/Source)

# Including dependencies
set(SDK_Type SHARED)
include(${CMAKE_CURRENT_LIST_DIR}/Modules/MythSDK/MythSDKConfig.cmake)

set(Standalone FALSE)
include(${CMAKE_CURRENT_LIST_DIR}/Modules/KrakenParts/KrakenPartsConfig.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Modules/HydraUI/HydraUIConfig.cmake)

set(Main_SrcDir ${KrakenClient_SrcDir}/Main)
set(Main_Files
    ${Main_SrcDir}/Main.cpp
)

add_executable(Main ${Main_Files})
set_target_properties(Main PROPERTIES
    FOLDER KrakenClient
    OUTPUT_NAME KrakenR
)
include_sdk(Main)
include_hydraIO(Main)
include_hydraGUI(Main)
include_krakenCores(Main)
