cmake_minimum_required (VERSION 2.6)
project (libbiosig)

option (USE_ZLIB "Add support for compressed files." OFF)
option (USE_SUITESPARSE "Add support for automatic re-referencing." OFF)

# Version number
set (libbiosig_VERSION_MAJOR 1)
set (libbiosig_VERSION_MINOR 5)
set (libbiosig_VERSION_PATCH 0)

if (USE_ZLIB)
  add_definitions (-D=WITH_ZLIB)
endif ()
if (USE_SUITESPARSE)
  add_definitions (-D=WITH_CHOLMOD)
endif ()

add_definitions (-D=WITHOUT_NETWORK)

find_program (GAWK NAMES gawk)

add_custom_target (eventcodes
  COMMAND ${GAWK} -f eventcodes.awk extern/eventcodes.txt
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  DEPENDS eventcodes.awk extern/eventcodes.txt
)

add_custom_target (units
  COMMAND ${GAWK} -f units.awk extern/units.csv > "units.i"
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  DEPENDS units.awk extern/units.csv
)

set (headers
  t210/abfheadr.h
  XMLParser/tinyxml.h
  XMLParser/tinystr.h
  biosig.h
  physicalunits.h
)

set (sources
  t210/sopen_abf_read.c
  t210/sopen_alpha_read.c
  t210/sopen_cfs_read.c
  t210/sopen_heka_read.c
  t210/sopen_igor.c
  t210/sopen_scp_read.c
  t210/scp-decode.cpp
  t220/crc4scp.c
  t220/sopen_scp_write.c
  t230/sopen_hl7aecg.cpp
  test0/sandbox.c
  XMLParser/tinystr.cpp
  XMLParser/tinyxml.cpp
  XMLParser/tinyxmlerror.cpp
  XMLParser/tinyxmlparser.cpp 
  biosig.c
  physicalunits.c
)

if (WIN32)
  list (APPEND sources
    win32/getdelim.c
    win32/getline.c
    win32/getlogin.c
  )
endif ()

add_library (biosig STATIC
  ${headers}
  ${sources}
)

add_dependencies (biosig eventcodes units)
