# Client for the Keba Wallbox UDP API

This is an implementation for the Keba Wallbox UDP API. It is written in C and has no dependencies.

## Compile

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr .
make -C build
```

## Run

```sh
Usage: build/bin/kebacc [OPTION]...

Keba UDP API Client 0.0.1
(c) 2025 Juergen Mang <mail@jcgames.de>
https://github.com/jcorporation/kebacc

Options:
  -h, --help              Displays this help
  -i, --wallbox <ip>      IP Adress of your Keba wallbox
  -l, --listen <uri>      Listen URI (default: udp://0.0.0.0:7090)
  -o, --loglevel <level>  Syslog loglevel (default: 5 - NOTICE)
  -p, --poll <seconds>    Polling interval in seconds (default: 60)
  -v, --version           Displays this help
  -w, --workdir <folder>  Working directory (default: /var/lib/kebacc)
```
