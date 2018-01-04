#include <httpxx-parser/header.hpp>

namespace httpxx_parser {

constexpr const char *method_name(Method method) {
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

bool CaseInsensitiveCompare::operator()(const std::string &a,
                                        const std::string &b) const noexcept {
  return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(),
                                      [](unsigned char ac, unsigned char bc) {
                                        return std::tolower(ac) <
                                               std::tolower(bc);
                                      });
}

} // namespace httpxx_parser