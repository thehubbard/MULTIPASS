
# File:         MULTIPASS/CMakeLists.txt
# Description:  CMakeLists for making MULTIPASS simulation code
#               Based upon version from Geant4 optical/LXe from 10.4.p01
# Version:      1.0.0
# Created:      2020-03-18
# Author:       Michael Hubbard
# Modified:     
#
#
# Tested with:
#              cmake v 3.5.1
#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(MULTIPASS)

#----------------------------------------------------------------------------
# for cxxopts parser
#
set (CMAKE_CXX_STANDARD 11) 

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(MULTIPASS MULTIPASS.cc ${sources} ${headers})
target_link_libraries(MULTIPASS ${Geant4_LIBRARIES} )

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build EJ276. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(ARKTIS_SCRIPTS
    defaults.mac
    photon.mac
    reviewEvent.mac
    gui.mac
    vis.mac
    slabLoop.mac
    subloop_xdim.mac
    ScanningTable.mac
    STsubloop_Y.mac
    STsubloop_X.mac
   )

foreach(_script ${ARKTIS_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS MULTIPASS DESTINATION bin)
