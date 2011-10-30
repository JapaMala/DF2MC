PROJECT (df2mc)
# WTF
include_directories (
    ${dfhack_SOURCE_DIR}/library/depends/md5/
    ${dfhack_SOURCE_DIR}/library/depends/tinyxml/
    ${CMAKE_SOURCE_DIR}
)
add_subdirectory (cNBT)

SET(PROJECT_SRCS
    ${dfhack_SOURCE_DIR}/library/depends/tinyxml/tinystr.cpp
    ${dfhack_SOURCE_DIR}/library/depends/tinyxml/tinyxml.cpp
    ${dfhack_SOURCE_DIR}/library/depends/tinyxml/tinyxmlerror.cpp
    ${dfhack_SOURCE_DIR}/library/depends/tinyxml/tinyxmlparser.cpp
    source/df2minecraft.cpp
)
DFHACK_PLUGIN(df2minecraft ${PROJECT_SRCS} LINK_LIBRARIES nbt z)
