#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Msnhnet" for configuration "Debug"
set_property(TARGET Msnhnet APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Msnhnet PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libMsnhnet.so"
  IMPORTED_SONAME_DEBUG "libMsnhnet.so"
  )

list(APPEND _cmake_import_check_targets Msnhnet )
list(APPEND _cmake_import_check_files_for_Msnhnet "${_IMPORT_PREFIX}/lib/libMsnhnet.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
