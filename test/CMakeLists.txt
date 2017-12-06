
set(SOURCES
    main.cpp
    request.cpp
)

add_executable(parser_test ${SOURCES})
target_link_libraries(parser_test httpxx_parser)


add_test(NAME HttpxxParser COMMAND parser_test)