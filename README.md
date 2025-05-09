# Client for the Keba Wallbox UDP API

This is an implementation for the Keba Wallbox UDP API. It is written in C and has no dependencies besides standard C library functions.

It is based on the great [Mongoose - Embedded Web Server / Embedded Network Library](https://github.com/cesanta/mongoose).

## Compile

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr .
make -C build
```

## Run

```sh
Usage: kebacc [OPTION]...

Keba UDP API Client 0.0.1
(c) 2025 Juergen Mang <mail@jcgames.de>
https://github.com/jcorporation/kebacc

Options:
  -d, --rrdcached <uri>   rrdcached uri (default: )
  -h, --help              Displays this help
  -i, --wallbox <ip>      IP Address of your Keba wallbox
  -l, --listen <uri>      Wallbox listen URI (default: udp://0.0.0.0:7090)
  -o, --loglevel <level>  Syslog loglevel (default: 5 - NOTICE)
  -p, --poll <seconds>    Polling interval in seconds (default: 60)
  -r, --rest <uri>        REST listen URI (default: http://0.0.0.0:8090)
  -v, --version           Displays this help
  -w, --workdir <folder>  Working directory (default: /var/lib/kebacc)
```

## Wallbox data polling

- `i` is sent once, on startup
- `report 1` is sent once, 10 seconds after startup
- `report 2` is sent each poll interval seconds
- `report 3` is sent each poll interval seconds, with an offset of 10 seconds from `report 2`

The received data is then published in different ways.

- As files in the working directory.
- Via REST-API
- Sent to [rrdcached](https://oss.oetiker.ch/rrdtool/doc/rrdcached.en.html)

## REST API

The REST API response with a timestamp and the original response from the wallbox.

| METHOD | PATH | DESCRIPTION |
| ------ | ---- | ----------- |
| GET | `/status` | Response for all below commands. |
| GET | `/status/i` | Response to `i` command. |
| GET | `/status/report1` | Response to `report 1` command. |
| GET | `/status/report2` | Response to `report 2` command. |
| GET | `/status/report3` | Response to `report 3` command. |

## Rrdcached

Wallbox data from `report 2` and `report 3` can be sent to a rrdcached instance. To enable this, set a valid rrdcached URI (e. g. `tcp://127.0.0.1:42217`).

### RRD Definitions

```sh
export RRDCACHED_ADDRESS=127.0.0.1
rrdtool create report1.rrd --start now-2h --step 1m \
    DS:state:GAUGE:2m:0:U \
    DS:maxcurr:GAUGE:2m:0:U \
    RRA:AVERAGE:0.5:1m:2d \
    RRA:AVERAGE:0.5:5m:8d \
    RRA:AVERAGE:0.5:15m:2y \
    RRA:AVERAGE:0.5:1h:20y \
    RRA:AVERAGE:0.5:1d:30y

rrdtool create report2.rrd --start now-2h --step 1m \
    DS:p:GAUGE:2m:0:U \
    RRA:AVERAGE:0.5:1m:2d \
    RRA:AVERAGE:0.5:5m:8d \
    RRA:AVERAGE:0.5:15m:2y \
    RRA:AVERAGE:0.5:1h:20y \
    RRA:AVERAGE:0.5:1d:30y
```
