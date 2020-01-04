#ifndef HTTP_H
#define HTTP_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif // HAVE_CONFIG_H

#include <string>

namespace ngtcp2 {

namespace http {

std::string get_reason_phrase(unsigned int status_code);

} // namespace http

} // namespace ngtcp2

#endif // HTTP_H
