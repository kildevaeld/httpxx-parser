// Copyright (c) 2017 Rasmus Kildevæld
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
#include "catch.hpp"

#include <httpxx-parser/request.hpp>

#include <iostream>

using namespace httpxx_parser;

TEST_CASE("request builder") {

  Request req(Post);

  req.set_url("rapper.com/testmig")
      .set_header("Content-Type", "plain/text")
      .set_header("Content-Length", "13")
      .set_body("Hello, World!");

  REQUIRE_FALSE(req);

  std::cout << req << std::endl;
}