#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "StatPhysQC::StatPhysQC" for configuration "Release"
set_property(TARGET StatPhysQC::StatPhysQC APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(StatPhysQC::StatPhysQC PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libStatPhysQC.so"
  IMPORTED_SONAME_RELEASE "libStatPhysQC.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS StatPhysQC::StatPhysQC )
list(APPEND _IMPORT_CHECK_FILES_FOR_StatPhysQC::StatPhysQC "${_IMPORT_PREFIX}/lib/libStatPhysQC.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
