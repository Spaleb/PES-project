# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/PES_Display_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/PES_Display_autogen.dir/ParseCache.txt"
  "PES_Display_autogen"
  )
endif()
