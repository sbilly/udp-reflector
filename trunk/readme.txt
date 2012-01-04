
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
The gnu c++ compiler is required for compilation on Linux, along with the 
libpcap development headers and libraries.  Installation of libpcap is
required for execution.

Visual Studio 2010 Express is required for compilation on Windows.
Installation of WinPcap and the Microsoft Visual C++ 2010 Redistributable 
Package is required for execution. 

Program options
---------------
   -s, source pcap interface and port
   -d, destination ip address and port
   -b, bind reflector socket to a specific source port (defaults to nonbinding)
   -i, ignore all UDP trafic originating from a specific source port
   -v, enable verbose debugging
   -l, list network devices
   -h, show this help message

Usage Examples
--------------
   1) Unidirectional - one way port forward examples (half duplex singleton).

      a) Reflect all UDP traffic on port 3000 from source pcap0 interface, to
      port 3200 of the broadcast destination address 192.168.1.255.  Enable
      verbose debugging.

      udp_reflector -s pcap0:3000 -d 192.168.1.255:3200 -v

      b) Reflect all UDP traffic on port 3000 from source pcap1 interface, to
      port 3100 of the destination address 192.168.1.10 and 3200 of the
      destination address 192.168.1.15.  Bind reflector socket to port 50000,
      and ignore UDP packets that originated on ports 25000 and 25001.

      udp_reflector -s pcap1:3000 -d 192.168.1.10:3100 -d 192.168.1.15:3200 \ 
         -b 50000 -i 25000 -i 25001

   2) Bidirectional - two way port forward example (full duplex matched pair).

      a) Reflect all UDP traffic on port 3000 from source pcap3 interface, to
      port 3200 of the broadcast destination address 192.168.1.255.
      Reflect all UDP traffic on port 3200 from source pcap5 interface, to port
      3000 of the broadcast destination address 192.168.1.255

      Note the use of the -b and -i options.  This allows other applications
      to bind to these destination ports, but prevents the infinite looping of
      packets between the udp_reflectors.

      udp_reflector -s pcap3:3000 -d 192.168.1.255:3200 -b 50001 -i 50002
      udp_reflector -s pcap5:3200 -d 192.168.1.255:3000 -b 50002 -i 50001

