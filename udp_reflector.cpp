/*
 *  udp_reflector tool
 *
 *  Copyright (C) 2011 Harlan Murphy
 *  Orbis Software - orbisoftware@gmail.com
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *   
 */

#include <pcap.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/ioctl.h>
#include <vector>

using namespace std;

struct UDP_Destination
{
   struct sockaddr_in dest_sock_addr;
   unsigned short dest_port;
   char *dest_addr;
};

static int socket_desc = 0;
static bool verbose_debug = 0;

static struct sockaddr_in source_sock_addr;
static unsigned short source_port = 0;
static unsigned short source_bind_port = 0;
static char *source_addr;

static vector<struct UDP_Destination> destination_points;
static vector<unsigned short> ignore_ports;

void create_socket()
{
   /* Create socket descriptor */
   if ((socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
   {
      fprintf(stderr, "Couldn't create socket descriptor\n");
      exit(1);
   }

   /* If the bind port option was set, setup the source socket address */
   if (source_bind_port != 0)
   {
      /* Set source socket address attributes */
      source_sock_addr.sin_family = AF_INET;
      source_sock_addr.sin_port = htons(source_bind_port);
      source_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

      if (bind(socket_desc, (struct sockaddr *) &source_sock_addr,
            sizeof(source_sock_addr)) == -1)
      {
         fprintf(stderr, "Couldn't bind reflector socket (port %i)\n",
               source_bind_port);
         fprintf(stderr, "Is this port address already in use?\n");
         exit(1);
      }
   }

   /* Allow broadcast packets to be sent */
   int broadcast = 1;
   if (setsockopt(socket_desc, SOL_SOCKET, SO_BROADCAST, &broadcast,
         sizeof broadcast) == -1)
   {
      perror("setsockopt (SO_BROADCAST)");
      exit(1);
   }

   /* Set socket address attributes for destination points */
   for (unsigned i = 0; i < destination_points.size(); i++)
   {
      struct hostent *dest_host_info = gethostbyname(
            destination_points[i].dest_addr);

      destination_points[i].dest_sock_addr.sin_family = AF_INET;
      destination_points[i].dest_sock_addr.sin_port = htons(
            destination_points[i].dest_port);
      bcopy((char *) dest_host_info->h_addr,
            (char *) &destination_points[i].dest_sock_addr.sin_addr,
            dest_host_info->h_length);

      if (verbose_debug)
      {
         fprintf(stdout, "  dest address: %s:%i\n",
               destination_points[i].dest_addr,
               destination_points[i].dest_port);
      }
   }

   if (verbose_debug)
   {
      fprintf(stdout, "source address: %s:%i\n", source_addr, source_port);

      if (source_bind_port != 0)
      {
         fprintf(stdout, "reflector socket bound to local port: %i\n",
               source_bind_port);
      }
   }
}

void print_usage()
{
   printf("\n");
   printf("Usage: udp_reflector [OPTION]...\n");
   printf("\n");
   printf(
         "Reflect the udp packets on a source port to a mirrored destination port.  Data\n");
   printf(
         "is captured at the data link layer using libpcap.  This allows another process\n");
   printf(
         "to bind to the port, and avoids the error \"Bind: Address Already in Use\".\n");
   printf(
         "udp_reflector supports both multiple packet destinations and multiple ignore\n");
   printf(
         "ports which are used to prevent the infinite looping of packets between multiple\n");
   printf("udp_reflectors.\n");
   printf("\n");
   printf("   -s, source network interface and port\n");
   printf("   -d, destination ip address and port\n");
   printf(
         "   -b, bind reflector socket to a specific source port (defaults to non-binding)\n");
   printf(
         "   -i, ignore all UDP trafic originating from a specific source port\n");
   printf("   -v, enable verbose debugging\n");
   printf("   -h, show this help message\n");
   printf("\n");
   printf("Examples:\n");
   printf("\n");
   printf(
         "   1) Unidirectional - one way port forward examples (half duplex singleton).\n");
   printf("\n");
   printf(
         "      a) Reflect all UDP traffic on port 3000 from network interface eth0, to\n");
   printf(
         "      port 3200 of the broadcast destination address 192.168.1.255.  Enable\n");
   printf("      verbose debugging.\n");
   printf("\n");
   printf("      udp_reflector -s eth0:3000 -d 192.168.1.255:3200 -v\n");
   printf("\n");
   printf(
         "      b) Reflect all UDP traffic on port 3000 from network loopback device, to\n");
   printf(
         "      port 3100 of the destination address 192.168.1.10 and 3200 of the\n");
   printf(
         "      destination address 192.168.1.15.  Bind reflector socket to port 50000,\n");
   printf(
         "      and ignore UDP packets that originated on ports 25000 and 25001.\n");
   printf("\n");
   printf(
         "      udp_reflector -s lo:3000 -d 192.168.1.10:3100 -d 192.168.1.15:3200 \\ \n");
   printf("         -b 50000 -i 25000 -i 25001\n");
   printf("\n");
   printf(
         "   2) Bidirectional - two way port forward example (full duplex matched pair).\n");
   printf("\n");
   printf(
         "      a) Reflect all UDP traffic on port 3000 from network interface eth0, to\n");
   printf(
         "      port 3200 of the broadcast destination address 192.168.1.255.\n");
   printf(
         "      Reflect all UDP traffic on port 3200 from network interface eth0, to port\n");
   printf("      3000 of the broadcast destination address 192.168.1.255\n");
   printf("\n");
   printf(
         "      Note the use of the -b and -i options.  This allows other applications\n");
   printf(
         "      to bind to these destination ports, but prevents the infinite looping of\n");
   printf("      packets between the udp_reflectors.\n");
   printf("\n");
   printf(
         "      udp_reflector -s eth0:3000 -d 192.168.1.255:3200 -b 50001 -i 50002\n");
   printf(
         "      udp_reflector -s eth0:3200 -d 192.168.1.255:3000 -b 50002 -i 50001\n");
   printf("\n");
}

void process_packet(void *x, const struct pcap_pkthdr *header,
      const u_char *packet)
{
   struct udphdr *udp_hdr = (struct udphdr *) (packet
         + sizeof(struct ether_header) + sizeof(struct iphdr));

   bool ignore_packet = false;

   /* Determine if the packet should be ignored */
   for (unsigned j = 0; j < ignore_ports.size(); j++)
   {
      if (ntohs(udp_hdr->source) == ignore_ports[j])
      {
         ignore_packet = true;

         if (verbose_debug)
         {
            fprintf(stdout, "ignoring %i bytes from ignored port %i\n",
                  header->len, ignore_ports[j]);
         }
         break;
      }
   }

   if (ignore_packet)
      return;

   /* Calculate the data offset within raw ethernet frame */
   int data_offset = sizeof(struct ether_header) + sizeof(struct iphdr)
         + sizeof(struct udphdr);

   int bytes_sent;

   /* Send UDP packet to each destination point */
   for (unsigned i = 0; i < destination_points.size(); i++)
   {
      bytes_sent = sendto(socket_desc, packet + data_offset,
            header->len - data_offset, 0,
            (struct sockaddr *) &destination_points[i].dest_sock_addr,
            sizeof(destination_points[i].dest_sock_addr));

      if (verbose_debug)
      {
         fprintf(stdout, "sent %i bytes to %s:%i\n", bytes_sent,
               destination_points[i].dest_addr,
               destination_points[i].dest_port);
      }

      if (bytes_sent == -1)
      {
         fprintf(stdout, "sendto() failed.  errno=%i\n", errno);
         perror("sendto");
      }
   }
}

int main(int argc, char *argv[])
{
   pcap_t *pcap_handle;
   char *network_dev;
   char errbuf[PCAP_ERRBUF_SIZE];
   struct bpf_program bpf_filter;
   char *source_port_str;
   char filter_exp[255];
   bpf_u_int32 netaddr;
   bpf_u_int32 netmask;

   /* Print warning if not being executed by root */
   if (getuid())
   {
      fprintf(
            stdout,
            "Warning: This program may require additional privileges to run correctly\n");
      fprintf(
            stdout,
            "Try running it through sudo or as root user if problems persist\n");
   }

   /* Print usage if no options were given */
   if (argc < 2)
   {
      print_usage();
      exit(0);
   }

   destination_points.clear();
   ignore_ports.clear();

   /* Process command line options */
   while (argc > 1)
   {
      if (argv[1][0] == '-')
      {
         switch (argv[1][1])
         {
         /* source network interface and port */
         case 's':
            network_dev = strtok(&argv[1][3], ":");

            if (network_dev != NULL)
            {
               /* Retrieve the network address of device w ioctl */
               int sd;
               struct ifreq ifr;

               sd = socket(AF_INET, SOCK_DGRAM, 0);
               ifr.ifr_addr.sa_family = AF_INET;
               strncpy(ifr.ifr_name, network_dev, IFNAMSIZ - 1);
               ioctl(sd, SIOCGIFADDR, &ifr);
               close(sd);

               /* Save device network address */
               source_addr = inet_ntoa(
                     ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr);

               /* Save source port and create filter expression */
               source_port_str = strtok(NULL, "\n");
               strcpy(&filter_exp[0], "udp and port ");
               strcpy(&filter_exp[13], source_port_str);
               source_port = atoi(source_port_str);
            }
            break;

            /* destination ip address and port */
         case 'd':
            struct UDP_Destination udp_dest;

            udp_dest.dest_addr = strtok(&argv[1][3], ":");

            if (network_dev != NULL)
               udp_dest.dest_port = atoi(strtok(NULL, "\n"));

            destination_points.push_back(udp_dest);
            break;

            /* bind reflector socket to a specific source port */
         case 'b':
            source_bind_port = atoi(&argv[1][3]);
            break;

            /* ignore all UDP trafic originating from a specific source port */
         case 'i':
            ignore_ports.push_back(atoi(&argv[1][3]));
            break;

            /* enable verbose debugging */
         case 'v':
            verbose_debug = true;
            break;

            /* show this help message */
         case 'h':
         default:
            print_usage();
            exit(0);
            break;
         }
      }

      argv++;
      argc--;
   }

   /* Verify the network device was defined */
   if (network_dev == NULL)
   {
      fprintf(stderr, "Undefined network device\n");
      exit(-1);
   }

   /* Find the properties for the device */
   if (pcap_lookupnet(network_dev, &netaddr, &netmask, errbuf) == -1)
   {
      fprintf(stderr, "Couldn't get netmask for device %s: %s\n", network_dev,
            errbuf);
      exit(1);
   }

   /* Open the session in promiscuous mode */
   pcap_handle = pcap_open_live(network_dev, BUFSIZ, 1, 0, errbuf);
   if (pcap_handle == NULL)
   {
      fprintf(stderr, "Couldn't open device %s: %s\n", network_dev, errbuf);
      exit(1);
   }

   /* Compile the filter */
   if (pcap_compile(pcap_handle, &bpf_filter, filter_exp, 0, netaddr) == -1)
   {
      fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp,
            pcap_geterr(pcap_handle));
      exit(1);
   }

   /* Apply the filter */
   if (pcap_setfilter(pcap_handle, &bpf_filter) == -1)
   {
      fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp,
            pcap_geterr(pcap_handle));
      exit(1);
   }

   /* Create reflector socket */
   create_socket();

   /* Register pcap_handler callback */
   pcap_loop(pcap_handle, 0, (pcap_handler) process_packet, NULL);

   /* Sleep forever */
   for (;;)
   {
      usleep(1000000);
   }

   return (0);
}
