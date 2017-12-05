#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
                          // in one cpp file
#include "catch.hpp"

#include <cstring>
#include <httpxx-parser/parser.hpp>

static const char *SIMPLE_GET_REQUEST = "HTTP/1.1 200 OK\r\n\r\n";

TEST_CASE("Parse request") {

  http_request::Parser parser;

  auto i = parser.execute(SIMPLE_GET_REQUEST);

  REQUIRE(i == std::strlen(SIMPLE_GET_REQUEST));
  REQUIRE(parser.state() == http_request::Parser::HeadersComplete);

  i = parser.execute();

  REQUIRE(i == 0);
  REQUIRE(parser.state() == http_request::Parser::MessageComplete);
}

static const char *GET_REQUEST = "HTTP/1.1 200 OK\r\n"
                                 "Content-Length: 13\r\n\r\n"
                                 "Hello, World!";

TEST_CASE("parser events") {

  http_request::Parser parser;

  bool headerCalled = false;
  bool bodyCalled = false;
  bool endCalled = false;

  parser.once<http_request::HeaderEvent>(
      [&headerCalled](const auto &e, auto &p) {
        REQUIRE(p.status() == 200);
        REQUIRE(e.header.at("content-length") == "13");
        headerCalled = true;
      });

  parser.once<http_request::DataEvent>([&bodyCalled](const auto &e, auto &p) {
    REQUIRE(e.data == "Hello, World!");
    bodyCalled = true;
  });

  parser.once<http_request::EndEvent>(
      [&endCalled](const auto &e, auto &) { endCalled = true; });

  auto i = parser.execute(GET_REQUEST);

  REQUIRE(i == std::strlen(GET_REQUEST));
  REQUIRE(parser.state() == http_request::Parser::MessageComplete);

  REQUIRE(headerCalled == true);
  REQUIRE(bodyCalled == true);
  REQUIRE(endCalled == true);
}