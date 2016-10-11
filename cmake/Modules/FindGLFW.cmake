# Locate the glfw library
# This module defines the following variables:
# GLFW_LIBRARIES, the name of the library;
# GLFW_INCLUDE_DIR, where to find glfw include files.
# GLFW_FOUND, true if both the GLFW_LIBRARIES and GLFW_INCLUDE_DIR have been found.
#
# To help locate the library and include file, you could define an environment variable called
# GLFW_ROOT which points to the root of the glfw library installation. This is pretty useful
# on a Windows platform.
#
#
# Usage example to compile an "executable" target to the glfw library:
#
# FIND_PACKAGE (glfw REQUIRED)
# INCLUDE_DIRECTORIES (${GLFW_INCLUDE_DIR})
# ADD_EXECUTABLE (executable ${EXECUTABLE_SRCS})
# TARGET_LINK_LIBRARIES (executable ${GLFW_LIBRARIES})
#
# TODO:
# Allow the user to select to link to a shared library or to a static library.

#Search for the include file
find_path(GLFW_INCLUDE_DIR glfw3.h DOC "Path to GLFW include directory"
  HINTS
  $ENV{GLFW_ROOT}
  PATH_SUFFIX include # For finding the include file under the root of the glfw expanded archive, typically on Windows.
  PATHS
  /usr/include
  /usr/local/include
  # By default headers are under GL subfolder
  /usr/include/GL
  /usr/local/include/GL
  /usr/include/GLFW
  /usr/local/include/GLFW
)

find_library(GLFW_LIBRARIES DOC "Absolute path to GLFW library"
  NAMES glfw glfw3
  HINTS
  $ENV{GLFW_ROOT}
  PATH_SUFFIXES lib/win32 # For finding the library file under the root of the glfw expanded archive, typically on Windows.
  PATHS
  /usr/lib
  /usr/local/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW REQUIRED_VARS GLFW_LIBRARIES GLFW_INCLUDE_DIR)

set(GLFW_FOUND 0)
if(GLFW_LIBRARIES AND GLFW_INCLUDE_DIR)
  set(GLFW_FOUND 1)
endif()

