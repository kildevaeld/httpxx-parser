#include <http_parser.h>
#include <httpxx-parser/url.hpp>
#include <ostream>

namespace http_request {

static inline std::string get_field(http_parser_url *parser,
                                    const std::string &url, int field) {

  if ((parser->field_set & (1 << field)) != 0) {
    return url.substr(parser->field_data[field].off,
                      parser->field_data[field].len);
  }

  return "";
}

URL::URL(const std::string &url) { parse(url); }

URL::URL() {}
URL::~URL() {}

bool URL::parse(const std::string &url) {

  http_parser_url parser;
  http_parser_url_init(&parser);

  auto result = http_parser_parse_url(url.data(), url.size(), false, &parser);
  if (result != 0) {
    m_valid = false;
    return false;
  }

  m_port = (int)parser.port;
  m_host = get_field(&parser, url, UF_HOST);
  m_path = get_field(&parser, url, UF_PATH);
  m_protocol = get_field(&parser, url, UF_SCHEMA);
  m_query = get_field(&parser, url, UF_QUERY);
  m_fragment = get_field(&parser, url, UF_FRAGMENT);

  if (m_port == 0)
    m_port = 80;

  if (m_path.size() == 0)
    m_path = "/";

  m_valid = true;

  return true;
}

int URL::port() const { return m_port; }
void URL::set_port(int port) { m_port = port; }

std::string URL::host() const { return m_host; }
void URL::set_host(const std::string &host) { m_host = host; }

std::string URL::path() const { return m_path; }
void URL::set_path(const std::string &path) { m_path = path; }

std::string URL::protocol() const { return m_protocol; }
void URL::set_protocol(const std::string &protocol) { m_protocol = protocol; }

std::string URL::query() const { return m_query; }
void URL::set_query(const std::string &query) { m_query = query; }

std::string URL::fragment() const { return m_fragment; }
void URL::set_fragment(const std::string &fragment) { m_fragment = fragment; }

bool URL::valid() const { return m_valid; }

std::ostream &operator<<(std::ostream &os, const URL &u) {
  if (!u.valid()) {
    os << "URL(invalid)";
    return os;
  }
  os << "URL(" << u.protocol() << "://" << u.host() << ":" << u.port()
     << u.path();

  if (u.query().size() > 0) {
    os << "?" << u.query();
  }
  if (u.fragment().size() > 0) {
    os << "#" << u.fragment();
  }

  os << ")";

  return os;
}

} // namespace http_request