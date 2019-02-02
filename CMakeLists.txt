###############################################################################
#                                  Geant4 ESS                                 #
###############################################################################
cmake_minimum_required(VERSION 3.6)
project(Geant4ESS CXX)

# External library
## Geant4
option(WITH_GEANT4_UIVIS "Build simulation with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()
include(${Geant4_USE_FILE})
## CADMesh
option(WITH_STATIC_CADMESH "Build and static link CADMesh" OFF)
if(WITH_STATIC_CADMESH)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/CADMesh)
else()
  find_package(cadmesh)
  include_directories(${CADMESH_INCLUDE_DIRS})
endif()
## YAML-CPP
## TODO: Implementer la configuration en YAML
option(WITH_STATIC_YAMLCPP "Build and static link yaml-cpp" OFF)
if(WITH_STATIC_YAMLCPP)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/yaml-cpp)
else()
  message(STATUS "Not implemented yet")
endif()
## MPI
option(WITH_MPI_SUPPORT "Build simulation with MPI support" OFF)
if(WITH_MPI_SUPPORT)
  find_package(G4mpi REQUIRED)
  include_directories(${G4mpi_INCLUDE_DIR})
  #add_definitions(-DTOOLS_USE_NATIVE_MPI) 
endif()

# Debug
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Tools
## Clang tidy
find_program(
  CLANG_TIDY_EXE
  NAMES "clang-tidy"
  DOC "Path to clang-tidy executable"
  )
if(NOT CLANG_TIDY_EXE)
  message(STATUS "clang-tidy not found.")
else()
  message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
  set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}" "-checks=*,-clang-analyzer-alpha.*")
endif()

# Define source files
set(Geant4ESS_inc
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ESSActionInitialization.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ESSAnalysis.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ESSBeamStopHit.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ESSBeamStopSD.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ESSCameraHit.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ESSCameraSD.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ESSConfigManager.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ESSConstants.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ESSDetectorConstruction.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ESSEventAction.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ESSMCPHit.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ESSMCPSD.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ESSPrimaryGeneratorAction.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ESSRunAction.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ESSSensorParameterisation.hh
  )

set(Geant4ESS_src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ESSActionInitialization.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ESSBeamStopHit.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ESSBeamStopSD.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ESSCameraHit.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ESSCameraSD.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ESSConfigManager.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ESSDetectorConstruction.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ESSEventAction.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ESSPrimaryGeneratorAction.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ESSMCPHit.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ESSMCPSD.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ESSRunAction.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ESSSensorParameterisation.cc
  )

# Define target
add_executable(Geant4ESS main.cpp ${Geant4ESS_inc} ${Geant4ESS_src})
if(WITH_MPI_SUPPORT)
  target_link_libraries(Geant4ESS ${G4mpi_LIBRARIES} ${Geant4_LIBRARIES})
  target_compile_definitions(Geant4ESS PUBLIC -DG4MPI)
else()
  target_link_libraries(Geant4ESS ${Geant4_LIBRARIES})
endif()

if(WITH_STATIC_CADMESH)
  target_link_libraries(Geant4ESS cadmesh)
else()
  target_link_libraries(Geant4ESS ${cadmesh_LIBRARIES})
endif()

if(WITH_STATIC_YAMLCPP)
  target_link_libraries(Geant4ESS yaml-cpp)
else()
  message(STATUS "Not implemented yet")
endif()

target_include_directories(Geant4ESS PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE src)

# Configure file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/init_vis.mac ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/icons.mac ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/vis.mac ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run.mac ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/camera.C ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/MPIhost ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_custom_command(TARGET Geant4ESS POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:Geant4ESS>)

# if(CLANG_TIDY_EXE)
#   set_target_properties(
#     Geant4ESS PROPERTIES
#     CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
#   )
# endif()