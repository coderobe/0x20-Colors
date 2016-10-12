# Locate the PortAudio library
# This module defines the following variables:
# PORTAUDIO_LIBRARIES, the name of the library;
# PORTAUDIO_INCLUDE_DIR, where to find glfw include files.
# PORTAUDIO_FOUND, true if both the PORTAUDIO_LIBRARIES and PORTAUDIO_INCLUDE_DIR have been found.
#
# Usage example to compile an "executable" target to the glfw library:
#
# FIND_PACKAGE (portaudio REQUIRED)
# INCLUDE_DIRECTORIES (${PORTAUDIO_INCLUDE_DIR})
# ADD_EXECUTABLE (executable ${EXECUTABLE_SRCS})
# TARGET_LINK_LIBRARIES (executable ${PORTAUDIO_LIBRARIES})

# Search for the include file
find_path(PORTAUDIO_INCLUDE_DIR
  NAMES
    portaudio.h
  PATHS
    /usr/include
    /usr/local/include
)

find_library(PORTAUDIO_LIBRARIES
  NAMES
    portaudio
  PATHS
    /usr/lib
    /usr/local/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PortAudio REQUIRED_VARS PORTAUDIO_LIBRARIES PORTAUDIO_INCLUDE_DIR)

set(PORTAUDIO_FOUND 0)
if(PORTAUDIO_LIBRARIES AND PORTAUDIO_INCLUDE_DIR)
  set(PORTAUDIO_FOUND 1)
endif()

