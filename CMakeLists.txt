#[[

╔═════════════════════════════════════════════════════════════════╗
║              Copyright © 2022 Mykhailo Mushynskyi               ║
╟─────────────────────────────────────────────────────────────────╢
║  This work is licensed under a GNU General Public License v3.0  ║
╚═════════════════════════════════════════════════════════════════╝

]]

message("\nCopyright ©2021 Mykhailo Mushynskyi.\nAll rights reserved.\n")

cmake_minimum_required(VERSION 3.19)

# Initial setup
set(Root ${CMAKE_SOURCE_DIR})
message(DEBUG "KrakenRenderer root folder is set to ${Root}.")
set(ProjectDirectory ${Root}/Project)
message(DEBUG "KrakenRenderer project folder is set to ${ProjectDirectory}.")
set(BuildDirectory ${Root}/Build)
message(DEBUG "KrakenRenderer build folder is set to ${BuildDirectory}.")

# Project-wide settings
project(KrakenRenderer)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CONFIGURATION_TYPES Debug Release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BuildDirectory})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BuildDirectory})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BuildDirectory})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_CMakeTrash")

# System-specific settings
if(APPLE)
    set(CMAKE_GENERATOR "XCode")
    set(CMAKE_OSX_ARCHITECTURES x86_x64 arm64)
    set(CMAKE_XCODE_BUILD_SYSTEM 12)
    set(CMAKE_CXX_FLAGS "-Werror -O3")
elseif(WIN32)
    set(CMAKE_GENERATOR "Visual Studio 17 2022")
    set(CMAKE_VS_PLATFORM_NAME x64)
    set(CMAKE_CXX_FLAGS "/WX /O2 /Oi")
endif()

# Load targets
include(${Root}/KrakenClientConfig.cmake)
