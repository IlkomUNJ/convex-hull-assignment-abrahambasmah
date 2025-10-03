# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Tugas_1_Komgraf_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Tugas_1_Komgraf_autogen.dir\\ParseCache.txt"
  "Tugas_1_Komgraf_autogen"
  )
endif()
