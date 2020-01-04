#ifndef SHARED_H
#define SHARED_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif // HAVE_CONFIG_H

#include <ngtcp2/ngtcp2.h>

namespace ngtcp2 {

enum class QUICErrorType {
  Application,
  Transport,
  TransportVersionNegotiation,
};

struct QUICError {
  QUICError(QUICErrorType type, uint64_t code) : type(type), code(code) {}

  QUICErrorType type;
  uint64_t code;
};

QUICError quic_err_transport(int liberr);

QUICError quic_err_tls(int alert);

QUICError quic_err_app(int liberr);

} // namespace ngtcp2

#endif // SHARED_H