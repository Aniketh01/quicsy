#include <stdio.h>

#include <nghttp3/nghttp3.h>
#include <ngtcp2/ngtcp2.h>

int qucisy_quic_version(char *p, size_t len)
{
	ngtcp2_info *ng2 = ngtcp2_version(0);
	nghttp3_info *ht3 = nghttp3_version(0);
	return snprintf(p, len, " ngtcp2/%s nghttp3/%s",
			ng2->version_str, ht3->version_str);
}

int main (int argc, char *argv[]) {
	static char quicbuffer[80];
    qucisy_quic_version(quicbuffer, sizeof(quicbuffer));
	printf("Aniketh %s", quicbuffer);
	return 0;
}
