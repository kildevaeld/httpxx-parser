#pragma once
#include <string>
namespace httpxx_parser {

class URL {

public:
  URL(const std::string &url);
  URL(const char *);
  URL();
  ~URL();

  bool parse(const std::string &url);

  int port() const;
  void set_port(int port);

  std::string host() const;
  void set_host(const std::string &host);

  std::string path() const;
  void set_path(const std::string &path);

  std::string protocol() const;
  void set_protocol(const std::string &protocol);

  std::string query() const;
  void set_query(const std::string &query);

  std::string fragment() const;
  void set_fragment(const std::string &fragment);

  bool valid() const;

  std::string str() const;

  friend std::ostream &operator<<(std::ostream &os, const URL &dt);

private:
  std::string m_host;
  int m_port;
  std::string m_path;
  std::string m_protocol;
  std::string m_query;
  std::string m_fragment;
  bool m_valid = false;
};
} // namespace httpxx_parser
