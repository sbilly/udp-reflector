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

#ifdef WIN32
   #define _CRT_SECURE_NO_WARNINGS
   #include <winsock2.h>
   #include <Shlobj.h>
#else
   #include <unistd.h>
   #include <arpa/inet.h>
   #include <net/if.h>
   #include <netdb.h>
#endif

#include <pcap.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <vector>
#include <string.h>
   
using namespace std;

struct UDP_Destination
{
    struct sockaddr_in dest_sock_addr;
    unsigned short dest_port;
    char *dest_addr;
};

struct Network_Device
{
    char device_name[128];
    char device_desc[256];
    char address[16];
};

static int socket_desc = 0;
static bool verbose_debug = 0;
static int PCAP_SNAPLEN_MAX = 65535;
static int max_packet_len = PCAP_SNAPLEN_MAX;

static struct sockaddr_in source_sock_addr;
static unsigned short source_port = 0;
static unsigned short source_bind_port = 0;
static char *source_addr;

static vector<UDP_Destination> destination_points;
static vector<unsigned short> ignore_ports;
static vector<Network_Device> network_devices;

/* Data offset within raw ethernet frame */
static const int DATA_OFFSET = sizeof(struct ether_header) + 
   sizeof(struct iphdr) + sizeof(struct udphdr);

void enumerate_devices()
{
    pcap_if_t *all_devs;
    pcap_if_t *dev;
    pcap_addr_t *dev_addr;

    char errbuf[PCAP_ERRBUF_SIZE + 1];

    network_devices.clear();

    /* Retrieve the interfaces list */
    if (pcap_findalldevs(&all_devs, errbuf) == -1)
    {
        fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
        exit(1);
    }

    /* Generate network device map */
    for (dev = all_devs; dev; dev = dev->next)
    {
        Network_Device net_dev;
        strcpy(net_dev.device_name, dev->name);

        if (dev->description)
            strcpy(net_dev.device_desc, dev->description);

        /* IP addresses */
        for (dev_addr = dev->addresses; dev_addr; dev_addr = dev_addr->next)
        {
            switch (dev_addr->addr->sa_family)
            {
            case AF_INET:
                if (dev_addr->addr)
                    sprintf(net_dev.address, inet_ntoa(
                       ((struct sockaddr_in *) dev_addr->addr)->sin_addr));
                break;

            default:
                break;
            }
        }

        network_devices.push_back(net_dev);
    }

    pcap_freealldevs(all_devs);
}

void list_network_devices()
{
    for (unsigned int n = 0; n < network_devices.size(); n++)
    {
        printf("\npcap%i", n);

        printf("     Device: %s\n", network_devices[n].device_name);
        printf("          Desc: %s\n", network_devices[n].device_desc);
        printf("          IPv4 Address: %s\n", network_devices[n].address);
    }
}

void create_socket()
{

#ifdef WIN32
    WORD wVersionRequested;
    WSADATA wsaData;

    /* Winsock version request 2.2 */
    wVersionRequested = MAKEWORD(2, 2);

    /* Open windows socket applictation startup */
    if (WSAStartup(wVersionRequested, &wsaData) != 0)
    {
        fprintf(stderr, "Couldn't call WSAStartup.\n");
        exit(1);
    }
#endif

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
    if (setsockopt(socket_desc, SOL_SOCKET, SO_BROADCAST, (char *) &broadcast,
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

        if (!dest_host_info)
        {
            fprintf(stderr, "gethostbyname(%s) failed\n",
                   destination_points[i].dest_addr);
            exit(1);
        }

        destination_points[i].dest_sock_addr.sin_family = AF_INET;
        destination_points[i].dest_sock_addr.sin_port = htons(
                destination_points[i].dest_port);
        memcpy((char *) &destination_points[i].dest_sock_addr.sin_addr,
                (char *) dest_host_info->h_addr, dest_host_info->h_length);

        if (verbose_debug)
        {
            printf("  dest address: %s:%i\n", destination_points[i].dest_addr,
                    destination_points[i].dest_port);
        }
    }

    if (verbose_debug)
    {
        printf("source address: %s:%i\n", source_addr, source_port);

        if (source_bind_port != 0)
        {
            printf("reflector socket bound to local port: %i\n",
                    source_bind_port);
        }
    }
}

void print_usage()
{
    printf("\n");
    printf("Usage: udp_reflector [OPTION]...\n");
    printf("\n");
    printf("Reflect the udp packets on a source port to a mirrored destination port.  Data\n");
    printf("is captured at the data link layer using libpcap.  This allows another process\n");
    printf("to bind to the port, and avoids the error \"Bind: Address Already in Use\".\n");
    printf("udp_reflector supports both multiple packet destinations and multiple ignore\n");
    printf("ports which are used to prevent the infinite looping of packets between\n");
    printf("multiple udp_reflectors.\n");
    printf("\n");
    printf("   -s, source pcap interface and port\n");
    printf("   -d, destination ip address and port\n");
    printf("   -b, bind reflector socket to a specific source port (defaults to nonbinding)\n");
    printf("   -i, ignore all UDP trafic originating from a specific source port\n");
    printf("   -v, enable verbose debugging\n");
    printf("   -l, list network devices\n");
    printf("   -h, show this help message\n");
    printf("\n");
    printf("Examples:\n");
    printf("\n");
    printf("   1) Unidirectional - one way port forward examples (half duplex singleton).\n");
    printf("\n");
    printf("      a) Reflect all UDP traffic on port 3000 from source pcap0 interface, to\n");
    printf("      port 3200 of the broadcast destination address 192.168.1.255.  Enable\n");
    printf("      verbose debugging.\n");
    printf("\n");
    printf("      udp_reflector -s pcap0:3000 -d 192.168.1.255:3200 -v\n");
    printf("\n");
    printf("      b) Reflect all UDP traffic on port 3000 from source pcap1 interface, to\n");
    printf("      port 3100 of the destination address 192.168.1.10 and 3200 of the\n");
    printf("      destination address 192.168.1.15.  Bind reflector socket to port 50000,\n");
    printf("      and ignore UDP packets that originated on ports 25000 and 25001.\n");
    printf("\n");
    printf("      udp_reflector -s pcap1:3000 -d 192.168.1.10:3100 -d 192.168.1.15:3200 \\ \n");
    printf("         -b 50000 -i 25000 -i 25001\n");
    printf("\n");
    printf("   2) Bidirectional - two way port forward example (full duplex matched pair).\n");
    printf("\n");
    printf("      a) Reflect all UDP traffic on port 3000 from source pcap3 interface, to\n");
    printf("      port 3200 of the broadcast destination address 192.168.1.255.\n");
    printf("      Reflect all UDP traffic on port 3200 from source pcap5 interface, to port\n");
    printf("      3000 of the broadcast destination address 192.168.1.255\n");
    printf("\n");
    printf("      Note the use of the -b and -i options.  This allows other applications\n");
    printf("      to bind to these destination ports, but prevents the infinite looping of\n");
    printf("      packets between the udp_reflectors.\n");
    printf("\n");
    printf("      udp_reflector -s pcap3:3000 -d 192.168.1.255:3200 -b 50001 -i 50002\n");
    printf("      udp_reflector -s pcap5:3200 -d 192.168.1.255:3000 -b 50002 -i 50001\n");
    printf("\n");
}

static void process_packet(u_char *x, const struct pcap_pkthdr *header,
        const u_char *packet)
{
    struct udphdr *udp_hdr = (struct udphdr *) (packet
            + sizeof(struct ether_header) + sizeof(struct iphdr));

    bool ignore_packet = false;
    int bytes_sent;
    
    /* Determine if the packet should be ignored */
    for (unsigned j = 0; j < ignore_ports.size(); j++)
    {
        if (ntohs(udp_hdr->source) == ignore_ports[j])
        {
            ignore_packet = true;

            if (verbose_debug)
            {
                printf("ignoring %i bytes from ignored port %i\n", header->len,
                        ignore_ports[j]);
            }
            break;
        }
    }

    if (ignore_packet)
        return;

    /* Send UDP packet to each destination point */
    for (unsigned i = 0; i < destination_points.size(); i++)
    {
        bytes_sent = sendto(socket_desc, (const char *) packet + DATA_OFFSET,
                header->len - DATA_OFFSET, 0,
                (struct sockaddr *) &destination_points[i].dest_sock_addr,
                sizeof(destination_points[i].dest_sock_addr));

        if (verbose_debug)
        {
            printf("sent %i bytes to %s:%i\n", bytes_sent,
                    destination_points[i].dest_addr,
                    destination_points[i].dest_port);
        }

        if (bytes_sent == -1)
        {
            printf("sendto() failed.  errno=%i\n", errno);
            perror("sendto");
            exit(1);
        }
    }
}

int main(int argc, char *argv[])
{
    pcap_t *pcap_handle;
    char *pcap_dev;
    char *network_dev;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program bpf_filter;
    char *source_port_str;
    char filter_exp[255];
    bpf_u_int32 netaddr;
    bpf_u_int32 netmask;

#ifdef WIN32
    /* Print warning if not being executed by admin */
    if (!IsUserAnAdmin())
    {
        printf("Warning: This program may require additional privileges to run correctly\n");
        printf("Try running it as administrator if problems persist\n");
    }
#else
    /* Print warning if not being executed by root */
    if (getuid())
    {
       printf("Warning: This program may require additional privileges to run correctly\n");
       printf("Try running it through sudo or as root user if problems persist\n");
    }
#endif

    /* Print usage if no options were given */
    if (argc < 2)
    {
        print_usage();
        exit(0);
    }

    /* Enumerate the network devices */
    enumerate_devices();

    destination_points.clear();
    ignore_ports.clear();

    /* Process command line options */
    while (argc > 1)
    {
        if (argv[1][0] == '-')
        {
            switch (argv[1][1])
            {
            /* Source network interface and port */
            case 's':
                pcap_dev = strtok(&argv[1][3], ":");

                if (pcap_dev != NULL)
                {
                    if (strncmp(pcap_dev, "pcap", 4) != 0)
                    {
                        fprintf(stderr, "Invalid source pcap interface");
                        exit(1);
                    }

                    char device_num[10];
                    memset(device_num, '\0', 10);

                    /* Pcap_dev is in the form pcapN, with N being the enumerated device */
                    strncpy(device_num, pcap_dev + 4, (strlen(pcap_dev) - 4));

                    /* Lookup network device from enumerated list */
                    network_dev = network_devices[atoi(device_num)].device_name;

                    /* Save device network address */
                    source_addr = network_devices[atoi(device_num)].address;

                    /* Save source port and create filter expression */
                    source_port_str = strtok(NULL, "\r\n");
                    strcpy(&filter_exp[0], "udp and port ");
                    strcpy(&filter_exp[13], source_port_str);
                    source_port = atoi(source_port_str);
                }
                break;

            /* Destination ip address and port */
            case 'd':
                struct UDP_Destination udp_dest;

                udp_dest.dest_addr = strtok(&argv[1][3], ":");

                if (pcap_dev != NULL)
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

            /* list network devices */
            case 'l':
                list_network_devices();
                exit(0);
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
        exit(1);
    }

    /* Find the properties for the device */
    if (pcap_lookupnet(network_dev, &netaddr, &netmask, errbuf) == -1)
    {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", network_dev,
                errbuf);
        exit(1);
    }

    /* Open the session in promiscuous mode, with 1 ms read buffering/timeout */
    pcap_handle = pcap_open_live(network_dev, max_packet_len, 1, 1, errbuf);
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
    pcap_loop(pcap_handle, 0, process_packet, NULL);

    return (0);
}
