# Install script for directory: /Users/fkm/Dropbox/tiziu/programming/branch_and_cut_and_connectivity/connectivity/lab_mc658/lemon/lemon-1.3.1/tools

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/Users/fkm/Dropbox/tiziu/programming/branch_and_cut_and_connectivity/connectivity/lab_mc658/lemon/lemon-1.3.1")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xbinx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/fkm/Dropbox/tiziu/programming/branch_and_cut_and_connectivity/connectivity/lab_mc658/lemon/lemon-1.3.1/tools/lgf-gen")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/lgf-gen" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/lgf-gen")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/Users/fkm/Dropbox/tiziu/programming/branch_and_cut_and_connectivity/connectivity/lab_mc658/lemon/lemon-1.3.1/lemon"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/lgf-gen")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/lgf-gen")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xbinx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/fkm/Dropbox/tiziu/programming/branch_and_cut_and_connectivity/connectivity/lab_mc658/lemon/lemon-1.3.1/tools/dimacs-to-lgf")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/dimacs-to-lgf" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/dimacs-to-lgf")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/Users/fkm/Dropbox/tiziu/programming/branch_and_cut_and_connectivity/connectivity/lab_mc658/lemon/lemon-1.3.1/lemon"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/dimacs-to-lgf")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/dimacs-to-lgf")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xbinx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/fkm/Dropbox/tiziu/programming/branch_and_cut_and_connectivity/connectivity/lab_mc658/lemon/lemon-1.3.1/tools/dimacs-solver")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/dimacs-solver" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/dimacs-solver")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/Users/fkm/Dropbox/tiziu/programming/branch_and_cut_and_connectivity/connectivity/lab_mc658/lemon/lemon-1.3.1/lemon"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/dimacs-solver")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/dimacs-solver")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xbinx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM FILES "/Users/fkm/Dropbox/tiziu/programming/branch_and_cut_and_connectivity/connectivity/lab_mc658/lemon/lemon-1.3.1/tools/lemon-0.x-to-1.x.sh")
endif()

