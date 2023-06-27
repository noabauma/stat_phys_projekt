#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "StatPhysQC::StatPhysQC" for configuration "Debug"
set_property(TARGET StatPhysQC::StatPhysQC APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(StatPhysQC::StatPhysQC PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/Debug/lib/libStatPhysQCd.so"
  IMPORTED_SONAME_DEBUG "libStatPhysQCd.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS StatPhysQC::StatPhysQC )
list(APPEND _IMPORT_CHECK_FILES_FOR_StatPhysQC::StatPhysQC "${_IMPORT_PREFIX}/Debug/lib/libStatPhysQCd.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
