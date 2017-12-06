#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
                          // in one cpp file
#include "catch.hpp"

#include <cstring>
#include <httpxx-parser/parser.hpp>

static const char *SIMPLE_GET_REQUEST = "HTTP/1.1 200 OK\r\n\r\n";

TEST_CASE("Parse request") {

  httpxx_parser::Parser parser(httpxx_parser::Parser::HTTPResponse);

  auto i = parser.execute(SIMPLE_GET_REQUEST);

  REQUIRE(i == std::strlen(SIMPLE_GET_REQUEST));
  REQUIRE(parser.state() == httpxx_parser::Parser::HeadersComplete);

  i = parser.execute();

  REQUIRE(i == 0);
  REQUIRE(parser.state() == httpxx_parser::Parser::MessageComplete);
}

static const char *GET_REQUEST = "HTTP/1.1 200 OK\r\n"
                                 "Content-Length: 13\r\n\r\n"
                                 "Hello, World!";

TEST_CASE("parser events") {

  httpxx_parser::Parser parser(httpxx_parser::Parser::HTTPResponse);

  bool headerCalled = false;
  bool bodyCalled = false;
  bool endCalled = false;

  parser.once<httpxx_parser::HeaderEvent>(
      [&headerCalled](const auto &e, auto &p) {
        REQUIRE(p.status() == 200);
        REQUIRE(e.header.at("content-length") == "13");
        headerCalled = true;
      });

  parser.once<httpxx_parser::DataEvent>([&bodyCalled](const auto &e, auto &p) {
    REQUIRE(e.data == "Hello, World!");
    bodyCalled = true;
  });

  parser.once<httpxx_parser::EndEvent>(
      [&endCalled](const auto &e, auto &) { endCalled = true; });

  auto i = parser.execute(GET_REQUEST);

  REQUIRE(i == std::strlen(GET_REQUEST));
  REQUIRE(parser.state() == httpxx_parser::Parser::MessageComplete);

  REQUIRE(headerCalled == true);
  REQUIRE(bodyCalled == true);
  REQUIRE(endCalled == true);
}

static const char *GET_REQUEST_CHUNKED = "HTTP/1.1 200 OK\r\n"
                                         "Transfer-Encoding: chunked\r\n\r\n"
                                         "6\r\n"
                                         "Hello,\r\n"
                                         "7\r\n"
                                         " World!\r\n"
                                         "0\r\n\r\n";

TEST_CASE("Chunked request") {

  httpxx_parser::Parser parser(httpxx_parser::Parser::HTTPResponse);

  int count = 0;
  std::stringstream ss;

  parser.on<httpxx_parser::DataEvent>([&count, &ss](const auto &e, auto &) {
    count++;
    ss << e.data;
  });

  parser.execute(GET_REQUEST_CHUNKED);

  REQUIRE(count == 2);
  REQUIRE(ss.str() == "Hello, World!");
}