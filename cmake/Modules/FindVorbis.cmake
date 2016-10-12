# Locate the Vorbis library
# This module defines the following variables:
# VORBIS_LIBRARIES, the name of the library;
# VORBIS_INCLUDE_DIR, where to find glfw include files.
# VORBIS_FOUND, true if both the VORBIS_LIBRARIES and VORBIS_INCLUDE_DIR have been found.
#
# Usage example to compile an "executable" target to the glfw library:
#
# FIND_PACKAGE (vorbis REQUIRED)
# INCLUDE_DIRECTORIES (${VORBIS_INCLUDE_DIR})
# ADD_EXECUTABLE (executable ${EXECUTABLE_SRCS})
# TARGET_LINK_LIBRARIES (executable ${VORBIS_LIBRARIES})

# Search for the include file
find_path(VORBIS_INCLUDE_DIR
  NAMES
    vorbisfile.h
  PATHS
    /usr/include
    /usr/local/include
  PATH_SUFFIXES
    vorbis
)

find_library(VORBIS_LIBRARIES
  NAMES
    vorbisfile vorbis
  PATHS
    /usr/lib
    /usr/local/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Vorbis REQUIRED_VARS VORBIS_LIBRARIES VORBIS_INCLUDE_DIR)

set(VORBIS_FOUND 0)
if(VORBIS_LIBRARIES AND VORBIS_INCLUDE_DIR)
  set(VORBIS_FOUND 1)
endif()

