
set(SOURCES
    header.cpp
    url.cpp
    parser.cpp

    ${PROJECT_ROOT}/vendor/http-parser/http_parser.c
)


add_library(httpxx_parser ${SOURCES})


install(TARGETS httpxx_parser
    DESTINATION lib
)

install(DIRECTORY ${PROJECT_ROOT}/include/httpxx-parser 
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
    )
