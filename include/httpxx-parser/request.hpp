// Copyright (c) 2017 Rasmus Kildevæld
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
#pragma once
#include "header.hpp"
#include "url.hpp"
#include <memory>

namespace httpxx_parser {

namespace internal {
class RequestPrivate;
}

class Request {

public:
  Request(Method method = Get, const URL &url = URL());
  Request(Request &&other);
  Request(const Request &) = delete;
  ~Request();

  Request &operator=(const Request &) = delete;

  Request &set_url(const URL &url);
  Request &set_url(URL &&url);
  URL url() const;

  Request &set_method(Method method);
  Method method() const;

  Request &set_header(const Header &header);
  Request &set_header(Header &&header);
  Request &set_header(const std::string &field, const std::string &value);
  Header header() const;

  Request &set_body(const std::string &body);
  Request &set_body(std::string &&body);
  Request &set_body(const char *data, size_t size);
  std::string body() const;

  bool valid() const;
  operator bool() const;

  std::string str() const;

  friend std::ostream &operator<<(std::ostream &, const Request &);

private:
  std::unique_ptr<internal::RequestPrivate> d;
};
} // namespace httpxx_parser