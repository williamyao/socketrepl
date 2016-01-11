#socketrepl

[![Build Status](https://travis-ci.org/williamyao/socketrepl.svg?branch=master)](https://travis-ci.org/williamyao/socketrepl)

Trivial application to open a connection to a remote host, and treat
it as if it was an asynchronous REPL.

##Usage

```
socketrepl <host> <port>
```

Connects to the specified host and port. Standard input is then sent on
the connection, and responses from the server are printed to standard
output asynchronously.

Report bugs to `/dev/null`. Seriously, it's like 100 lines total. If you
need a fix, it'd be faster to write it yourself.
