# SOCKS4-proxy-server

implement the SOCKS 4/4A protocol in the application layer of the OSI model.

SOCKS is similar to a proxy (i.e., intermediary-program) that acts as both server and client for the purpose of making requests on behalf of other clients. Because the SOCKS protocol is independent of application protocols, it can be used for many different services: telnet, ftp, WWW, etc.

There are two types of the SOCKS operations, namely CONNECT and BIND.both of them are covered in this project.

## Prerequisite
- install Boost library
`sudo apt-get install libboost-all-dev`

## How to use
1. Build from source
```shell=
cd socks4-proxy-server
make
```
2. Start
`./socks_server [port]`
3. Modify the browser proxy setting (FireFox)

## SOCKS 4 Implementation

1. After the SOCKS server starts listening, if a SOCKS client connects, use fork() to tackle with it.
2. Receive SOCKS4 REQUEST from the SOCKS client.
3. Get the destination IP and port from SOCKS4 REQUEST.
4. Check the firewall (socks.conf), and send SOCKS4 REPLY to the SOCKS client if rejected
5. Check CD value and choose one of the operations.
  - CONNECT (CD=1)
    - Connect to the destination
    - Send SOCKS4 REPLY to the SOCKS client
    - Start relaying traffic on both directions
  - BIND (CD=2)
    - Bind and listen a port
    - Send SOCKS4 REPLY to SOCKS client to tell which port to connect
    - Accept connection from destination and send another SOCKS4 REPLY to SOCKS client
    - Start relaying traffic on both directions
