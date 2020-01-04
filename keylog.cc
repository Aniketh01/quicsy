#include <string>

#include "keylog.h"
#include "util.h"

namespace ngtcp2 {

namespace keylog {

void log_secret(SSL *ssl, const char *name, const unsigned char *secret,
                size_t secretlen) {
  if (auto keylog_cb = SSL_CTX_get_keylog_callback(SSL_get_SSL_CTX(ssl))) {
    unsigned char crandom[32];
    if (SSL_get_client_random(ssl, crandom, 32) != 32) {
      return;
    }
    std::string line = name;
    line += " " + util::format_hex(crandom, 32);
    line += " " + util::format_hex(secret, secretlen);
    keylog_cb(ssl, line.c_str());
  }
}

} // namespace keylog

} // namespace ngtcp2