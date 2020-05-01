# quicsy
A standalone QUIC client and server based on ngtcp2 and nghttp3.

Note:  The QUIC and HTTP/3 side of code is from the /examples of ngtcp2.

Get a copy:

`https://github.com/Aniketh01/quicsy.git`

Initialise the submodules

```
git submodule update --init --progress
```

Once inside the directory

1) Run `source script.sh` to build the dependencies.
2) Next, run `sh bootstrap.sh` to initialise quicsy build system.
3) Finally, compile quicsy using `source compile.sh`.

Once installed a `client` and `server` binary should reside in `bin/`.

Note: If quicsy fails to fetch libssl.so shared library file of patched openssl, then export `LD_LIBRARY_PATH`.
The error can look similar to this:
`error while loading shared libraries: libssl.so.3: cannot open shared object file: No such file or directory`

`$ export LD_LIBRARY_PATH=$rootdir/deps/build_openssl/lib`
