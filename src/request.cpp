// Copyright (c) 2017 Rasmus Kildevæld
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
#include <httpxx-parser/request.hpp>
#include <sstream>

static bool iequals(const std::string &a, const std::string &b) {
  unsigned int sz = a.size();
  if (b.size() != sz)
    return false;
  for (unsigned int i = 0; i < sz; ++i)
    if (std::tolower(a[i]) != std::tolower(b[i]))
      return false;
  return true;
}

static const char *const kCRLF = "\r\n";

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
  s << method_name(r.d->method) << " " << r.d->url.path() << " HTTP/1.1"
    << kCRLF;

  bool has_host, has_cl = false;

  for (const auto &h : r.d->header) {
    if (iequals(h.first, "content-length"))
      has_cl = true;
    else if (iequals(h.first, "host"))
      has_host = true;
    s << h.first << ": " << h.second << kCRLF;
  }

  if (!has_host && r.d->url.host().size() > 0) {
    s << "Host: " << r.d->url.host() << kCRLF;
  }

  auto m = r.d->method;
  if ((r.d->body.size() > 0 && (m == Post || m == Put || m == Patch)) &&
      !has_cl) {
    s << "Content-Length: " << r.d->body.size() << kCRLF;
  }

  s << kCRLF;

  if (r.d->body.size() > 0) {
    if (m == Post || m == Put || m == Patch) {
      s << r.d->body;
    }
  }

  return s;
}

} // namespace httpxx_parser