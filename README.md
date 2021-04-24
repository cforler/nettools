# nettools

Bunch of small nitty-gritty network tools for GNU/Linux.

Current release state: Works for me. :-)


## Build an run the applications
```shell
$ make release
```

## Getip

Returns the IP address(es) of a given host.

Examples:
```shell
$ getip stackoverflow.com
151.101.1.69
```
```shell
$ getip -a stackoverflow.com
151.101.1.69
151.101.65.69
151.101.193.69
151.101.129.69
```
```shell
getip -a wikipedia.com
91.198.174.194
2620:0:862:ed1a::3
```

## Getwebserver

Returns the webserver entry from the HTTP header.

```shell
$ getwebserver stackoverflow.com
Server: Varnish
```

```shell
$ getwebserver 2620:0:862:ed1a::3
Server: nginx/1.14.2
```

## Getifaddr

Lists descriptions of network interfaces of the local system.

```shell
$ ./getifaddr lo
lo: 
	ether 0:00:00:00:00:00
	tx_packets =       35   rx_packets =       35
	tx_bytes   =     3021   rx_bytes   =     3021
	inet 127.0.0.1   netmask 255.0.0.0   broadcast 127.0.0.1
	inet6 ::1   netmask ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff
```
