###############################################################################
#                                  Geant4 ESS                                 #
###############################################################################
cmake_minimum_required(VERSION 3.6)

project(Geant4ESS CXX)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()
include(${Geant4_USE_FILE})

find_package(cadmesh)
include_directories(${CADMESH_INCLUDE_DIRS})

# Debug
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(Geant4ESS_inc
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ESSEventAction.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ESSDetectorConstruction.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ESSActionInitialization.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ESSPrimaryGeneratorAction.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ESSCameraHit.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ESSCameraSD.hh
  )

set(Geant4ESS_src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ESSEventAction.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ESSDetectorConstruction.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ESSActionInitialization.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ESSPrimaryGeneratorAction.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ESSCameraHit.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ESSCameraSD.cc
  )

add_executable(Geant4ESS main.cpp ${Geant4ESS_inc} ${Geant4ESS_src})
target_link_libraries(Geant4ESS ${Geant4_LIBRARIES})
target_link_libraries(Geant4ESS ${cadmesh_LIBRARIES})
target_include_directories(Geant4ESS PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE src)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/init_vis.mac ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/icons.mac ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/vis.mac ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run.mac ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_custom_command(TARGET Geant4ESS POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:Geant4ESS>)