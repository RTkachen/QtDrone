# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/moj_projekt_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/moj_projekt_autogen.dir/ParseCache.txt"
  "moj_projekt_autogen"
  )
endif()
