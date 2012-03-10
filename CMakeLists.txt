PROJECT (df2mc)

SET(PROJECT_SRCS
    ${dfhack_SOURCE_DIR}/library/depends/tinyxml/tinystr.cpp
    ${dfhack_SOURCE_DIR}/library/depends/tinyxml/tinyxml.cpp
    ${dfhack_SOURCE_DIR}/library/depends/tinyxml/tinyxmlerror.cpp
    ${dfhack_SOURCE_DIR}/library/depends/tinyxml/tinyxmlparser.cpp
    source/df2minecraft.cpp
)

DFHACK_PLUGIN(df2minecraft ${PROJECT_SRCS} LINK_LIBRARIES ${ZLIB_LIBRARIES})

INSTALL(FILES df2mc.xml DESTINATION ${DFHACK_DATA_DESTINATION})