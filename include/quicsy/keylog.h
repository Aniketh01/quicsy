#ifndef KEYLOG_H
#define KEYLOG_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif // HAVE_CONFIG_H

#include <ngtcp2/ngtcp2.h>

#include <openssl/ssl.h>

namespace ngtcp2 {

namespace keylog {

constexpr char QUIC_CLIENT_EARLY_TRAFFIC_SECRET[] =
    "QUIC_CLIENT_EARLY_TRAFFIC_SECRET";
constexpr char QUIC_CLIENT_HANDSHAKE_TRAFFIC_SECRET[] =
    "QUIC_CLIENT_HANDSHAKE_TRAFFIC_SECRET";
constexpr char QUIC_CLIENT_TRAFFIC_SECRET_0[] = "QUIC_CLIENT_TRAFFIC_SECRET_0";
constexpr char QUIC_SERVER_HANDSHAKE_TRAFFIC_SECRET[] =
    "QUIC_SERVER_HANDSHAKE_TRAFFIC_SECRET";
constexpr char QUIC_SERVER_TRAFFIC_SECRET_0[] = "QUIC_SERVER_TRAFFIC_SECRET_0";

void log_secret(SSL *ssl, const char *name, const unsigned char *secret,
                size_t secretlen);

} // namespace keylog

} // namespace ngtcp2

#endif // KEYLOG_H
