// Copyright (c) 2017 Rasmus Kildevæld
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
#include <httpxx-parser/request.hpp>
#include <sstream>

namespace httpxx_parser {

namespace internal {
class RequestPrivate {

public:
  RequestPrivate() {}
  ~RequestPrivate() {}
  Method method;
  URL url;
  Header header;
  std::string body;
};
} // namespace internal

Request::Request(Method method, const URL &url)
    : d(new internal::RequestPrivate()) {
  d->method = method;
  d->url = url;
}

Request::Request(Request &&other) : d(std::move(other.d)) {}

Request::~Request() {}

Request &Request::set_url(const URL &url) {
  d->url = url;
  return *this;
}
Request &Request::set_url(URL &&url) {
  d->url = std::move(url);
  return *this;
}

URL Request::url() const { return d->url; }

Request &Request::set_method(Method method) {
  d->method = method;
  return *this;
}
Method Request::method() const { return d->method; }

Request &Request::set_header(const Header &header) {
  d->header = header;
  return *this;
}

Request &Request::set_header(Header &&header) {
  d->header = std::move(header);
  return *this;
}

Request &Request::set_header(const std::string &field,
                             const std::string &value) {
  d->header[field] = value;
  return *this;
}

static inline std::string method_to_str(Method method) {
  switch (method) {
  case Post:
    return "POST";
  case Get:
    return "GET";
  case Put:
    return "PUT";
  case Patch:
    return "PATCH";
  case Option:
    return "OPTION";
  case Delete:
    return "DELETE";
  case Head:
    return "HEAD";
  }
}

Header Request::header() const { return d->header; }

Request &Request::set_body(const std::string &body) {
  d->body = body;
  return *this;
}
Request &Request::set_body(std::string &&body) {
  d->body = std::move(body);
  return *this;
}

Request &Request::set_body(const char *data, size_t size) {
  d->body.append(data, size);
  return *this;
}

std::string Request::body() const { return d->body; }

std::string Request::str() const {
  std::stringstream ss;
  ss << *this;
  return ss.str();
}

bool Request::valid() const { return d->url.valid(); }

Request::operator bool() const { return valid(); }

std::ostream &operator<<(std::ostream &s, const Request &r) {
  s << method_to_str(r.d->method) << " " << r.d->url.path() << " HTTP/1.1\r\n";

  for (const auto &h : r.d->header) {
    s << h.first << ": " << h.second << "\r\n";
  }

  if (r.d->header.find("Host") == r.d->header.end() &&
      r.d->url.host().size() > 0) {
    s << "Host: " << r.d->url.host() << "\r\n";
  }

  auto m = r.d->method;
  if (r.d->body.size() > 0 && (m == Post || m == Put || m == Patch)) {
    if (r.d->header.find("Content-Length") == r.d->header.end()) {
      s << "Content-Length: " << r.d->body.size() << "\r\n";
    }
  }

  s << "\r\n";

  if (r.d->body.size() > 0) {
    if (m == Post || m == Put || m == Patch) {
      s << r.d->body;
    }
  }

  return s;
}

} // namespace httpxx_parser