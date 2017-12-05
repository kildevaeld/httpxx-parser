
set(SOURCES
    main.cpp
)

add_executable(parser_test ${SOURCES})
target_link_libraries(parser_test httpxx_parser)