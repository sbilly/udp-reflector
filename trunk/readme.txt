
udp reflector 
-------------

udp_reflector is a tool to support the reflecting of udp traffic on a source 
port to a mirrored destination port.  Data is captured at the data link layer 
using libpcap.  This allows another process to bind to the port, and avoids 
the error "Bind: Address Already in Use".  udp_reflector supports both multiple
packet destinations and multiple ignore ports which are used to prevent the 
infinite looping of packets between multiple udp_reflectors.

License
-------
udp_reflector is licensed under the GNU GPL 3.0.

Source code is available from the Subversion repository at the project's 
homepage: http://code.google.com/p/udp-reflector/

Required software
-----------------
The gnu c++ compiler is required.
libpcap development headers and libraries.

Compiling
---------
To compile: 
   g++ -o udp_reflector udp_reflector.cpp -lpcap

Program options
---------------
   -s, source network interface and port
   -d, destination ip address and port
   -b, bind reflector socket to a specific source port (defaults to non-binding)
   -i, ignore all UDP trafic originating from a specific source port
   -v, enable verbose debugging
   -h, show this help message

Usage Examples
--------------
   1) Unidirectional - one way port forward examples (half duplex singleton).

      a) Reflect all UDP traffic on port 3000 from network interface eth0, to
      port 3200 of the broadcast destination address 192.168.1.255.  Enable
      verbose debugging.

      udp_reflector -s eth0:3000 -d 192.168.1.255:3200 -v

      b) Reflect all UDP traffic on port 3000 from network loopback device, to
      port 3100 of the destination address 192.168.1.10 and 3200 of the
      destination address 192.168.1.15.  Bind reflector socket to port 50000,
      and ignore UDP packets that originated on ports 25000 and 25001.

      udp_reflector -s lo:3000 -d 192.168.1.10:3100 -d 192.168.1.15:3200 \ 
         -b 50000 -i 25000 -i 25001

   2) Bidirectional - two way port forward example (full duplex matched pair).

      a) Reflect all UDP traffic on port 3000 from network interface eth0, to
      port 3200 of the broadcast destination address 192.168.1.255.
      Reflect all UDP traffic on port 3200 from network interface eth0, to port
      3000 of the broadcast destination address 192.168.1.255

      Note the use of the -b and -i options.  This allows other applications
      to bind to these destination ports, but prevents the infinite looping of
      packets between the udp_reflectors.

      udp_reflector -s eth0:3000 -d 192.168.1.255:3200 -b 50001 -i 50002
      udp_reflector -s eth0:3200 -d 192.168.1.255:3000 -b 50002 -i 50001

