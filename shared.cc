#include "shared.h"

#include <nghttp3/nghttp3.h>

namespace ngtcp2 {

QUICError quic_err_transport(int liberr) {
  if (liberr == NGTCP2_ERR_RECV_VERSION_NEGOTIATION) {
    return {QUICErrorType::TransportVersionNegotiation, 0};
  }
  return {QUICErrorType::Transport,
          ngtcp2_err_infer_quic_transport_error_code(liberr)};
}

QUICError quic_err_tls(int alert) {
  return {QUICErrorType::Transport,
          static_cast<uint64_t>(NGTCP2_CRYPTO_ERROR | alert)};
}

QUICError quic_err_app(int liberr) {
  return {QUICErrorType::Application,
          nghttp3_err_infer_quic_app_error_code(liberr)};
}

} // namespace ngtcp2
