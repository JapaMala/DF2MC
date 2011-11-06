PROJECT (df2mc)

SET(PROJECT_SRCS
    ${dfhack_SOURCE_DIR}/library/depends/tinyxml/tinystr.cpp
    ${dfhack_SOURCE_DIR}/library/depends/tinyxml/tinyxml.cpp
    ${dfhack_SOURCE_DIR}/library/depends/tinyxml/tinyxmlerror.cpp
    ${dfhack_SOURCE_DIR}/library/depends/tinyxml/tinyxmlparser.cpp
    source/df2minecraft.cpp
)

if(WIN32)
    include_directories(
        ${dfhack_SOURCE_DIR}/library/depends/zlib/
        ${dfhack_SOURCE_DIR}/library/depends/tinyxml/
        ${CMAKE_SOURCE_DIR}
    )
    LINK_DIRECTORIES (
        ${LINK_DIRECTORIES}
        ${dfhack_SOURCE_DIR}/library/depends/zlib/
    )
    DFHACK_PLUGIN(df2minecraft ${PROJECT_SRCS} LINK_LIBRARIES zlib.lib)
else()
    include_directories(
        ${dfhack_SOURCE_DIR}/library/depends/tinyxml/
        ${CMAKE_SOURCE_DIR}
    )
    DFHACK_PLUGIN(df2minecraft ${PROJECT_SRCS} LINK_LIBRARIES z)
endif()

INSTALL(FILES df2mc.xml DESTINATION ${DFHACK_DATA_DESTINATION})