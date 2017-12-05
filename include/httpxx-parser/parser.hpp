#pragma once
#include "emitter.hpp"
#include "header.hpp"
#include <memory>

struct http_parser;

namespace http_request {

namespace internal {
class ParserPrivate;
}

struct HeaderEvent {
  HeaderEvent(Header &&h) : header(std::move(h)) {}
  Header header;
};

struct DataEvent {
  DataEvent(std::string &&d) : data(std::move(d)) {}
  std::string data;
};

struct EndEvent {
  EndEvent() {}
};

class Parser : public Emitter<Parser> {

public:
  enum State {
    Initial,
    MessageStarted,
    HeadersStarted,
    HeadersComplete,
    MessageComplete
  };

  Parser();
  ~Parser();
  Parser(const Parser &other) = delete;

  int execute(const std::string &data = std::string(""));

  const Header &header() const;
  uint16_t status() const;

  State state() const;

private:
  friend class internal::ParserPrivate;
  std::unique_ptr<internal::ParserPrivate> d;
};

} // namespace http_request