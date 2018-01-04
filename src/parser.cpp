#include <climits>
#include <http_parser.h>
#include <httpxx-parser/parser.hpp>
#include <string>

#include <iostream>

namespace httpxx_parser {

namespace internal {

class ParserPrivate {

public:
  ParserPrivate(Parser *p, Parser::Type type) : q(p) {
    http_parser_init(&parser, (http_parser_type)type);
    http_parser_settings_init(&settings);

    parser.data = this;

    settings.on_message_begin = on_message_begin;
    settings.on_message_complete = on_message_complete;
    settings.on_header_field = on_header_field;
    settings.on_header_value = on_header_value;
    settings.on_headers_complete = on_headers_complete;
    settings.on_body = on_body;
    settings.on_chunk_header = on_chunk_header;
    settings.on_chunk_complete = on_chunk_complete;
  }

  Parser *q;

  http_parser parser;
  http_parser_settings settings;
  Header header;
  Parser::State state = Parser::Initial;

  std::string current_field;

private:
  static int on_message_begin(http_parser *);
  static int on_message_complete(http_parser *);

  static int on_header_field(http_parser *, const char *, size_t);
  static int on_header_value(http_parser *, const char *, size_t);
  static int on_headers_complete(http_parser *);

  static int on_body(http_parser *, const char *, size_t);
  static int on_chunk_header(http_parser *);
  static int on_chunk_complete(http_parser *);
};

int ParserPrivate::on_message_begin(http_parser *parser) {
  auto p = reinterpret_cast<ParserPrivate *>(parser->data);
  p->state = Parser::MessageStarted;
  return 0;
}

int ParserPrivate::on_message_complete(http_parser *parser) {
  auto p = reinterpret_cast<ParserPrivate *>(parser->data);
  p->state = Parser::MessageComplete;
  p->q->publish(EndEvent());
  return 0;
}

int ParserPrivate::on_header_field(http_parser *parser, const char *data,
                                   size_t size) {
  auto p = reinterpret_cast<ParserPrivate *>(parser->data);
  p->current_field = std::string(data, size);
  p->state = Parser::HeadersStarted;
  return 0;
}

int ParserPrivate::on_header_value(http_parser *parser, const char *data,
                                   size_t size) {
  auto p = reinterpret_cast<ParserPrivate *>(parser->data);
  p->header[std::move(p->current_field)] = std::string(data, size);

  return 0;
}

int ParserPrivate::on_headers_complete(http_parser *parser) {
  auto p = reinterpret_cast<ParserPrivate *>(parser->data);
  p->state = Parser::HeadersComplete;
  p->q->publish(HeaderEvent(p->header, parser->status_code));
  return 0;
}

int ParserPrivate::on_body(http_parser *parser, const char *data, size_t size) {
  auto p = reinterpret_cast<ParserPrivate *>(parser->data);
  p->q->publish(DataEvent(std::string(data, size)));
  return 0;
}

int ParserPrivate::on_chunk_header(http_parser *p) { return 0; }

int ParserPrivate::on_chunk_complete(http_parser *p) { return 0; }

} // namespace internal

Parser::Parser(Parser::Type type)
    : d(new internal::ParserPrivate(this, type)) {}
Parser::~Parser() {}

int Parser::execute(const std::string &data) {
  return http_parser_execute(&d->parser, &d->settings, data.data(),
                             data.size());
}

const Header &Parser::header() const { return d->header; }
uint16_t Parser::status() const { return d->parser.status_code; }

Parser::State Parser::state() const { return d->state; }

void Parser::reset() {
  d->header.clear();
  d->state = Initial;
}

} // namespace httpxx_parser