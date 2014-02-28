#include <iostream>

#include "ChannelMappingConfig.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace UMTS
{

static ChannelMappingConfig* instance = NULL;

static const std::string MAPPING_FILE_LOC = "./common/mapping.xml";

/*
 * Channel specific settings read from the mapping.xml file, used for binding
 * socket at channel instantiation.
 */
static std::string ip_address;
static int port;

/*
 * Network endpoints, used for channel routing
 */
static ChannelNetworkEndpoint physicalNetworkEndpoints[NUM_PHYSICAL_CHANNELS];
static ChannelNetworkEndpoint transportNetworkEndpoints[NUM_TRANSPORT_CHANNELS];
static ChannelNetworkEndpoint logicalNetworkEndpoints[NUM_LOGICAL_CHANNELS];

ChannelMappingConfig::ChannelMappingConfig()
{
    initPhysicalChannelMapping();
    initTransportChannelMapping();
    initLogicalChannelMapping();
}

ChannelMappingConfig* ChannelMappingConfig::getInstance()
{

    if (!instance)
    {
        instance = new ChannelMappingConfig();
    }

    return instance;
}

void ChannelMappingConfig::initPhysicalChannelMapping()
{

    boost::property_tree::ptree pt;
    boost::property_tree::xml_parser::read_xml(MAPPING_FILE_LOC, pt);

    physicalNetworkEndpoints[0].ipAddress = pt.get<std::string>(
            "umts_channel_mapping.physical.p_ccpch.address");
    physicalNetworkEndpoints[0].port = pt.get<int>(
            "umts_channel_mapping.physical.p_ccpch.port");

    physicalNetworkEndpoints[1].ipAddress = pt.get<std::string>(
            "umts_channel_mapping.physical.s_ccpch.address");
    physicalNetworkEndpoints[1].port = pt.get<int>(
            "umts_channel_mapping.physical.s_ccpch.port");

    physicalNetworkEndpoints[2].ipAddress = pt.get<std::string>(
            "umts_channel_mapping.physical.d_dpcch.address");
    physicalNetworkEndpoints[2].port = pt.get<int>(
            "umts_channel_mapping.physical.d_dpcch.port");

    physicalNetworkEndpoints[3].ipAddress = pt.get<std::string>(
            "umts_channel_mapping.physical.hs_scch.address");
    physicalNetworkEndpoints[3].port = pt.get<int>(
            "umts_channel_mapping.physical.hs_scch.port");

}

void ChannelMappingConfig::initTransportChannelMapping()
{

    boost::property_tree::ptree pt;
    boost::property_tree::xml_parser::read_xml(MAPPING_FILE_LOC, pt);

    transportNetworkEndpoints[0].ipAddress = pt.get<std::string>(
            "umts_channel_mapping.transport.bch.address");
    transportNetworkEndpoints[0].port = pt.get<int>(
            "umts_channel_mapping.transport.bch.port");

    transportNetworkEndpoints[1].ipAddress = pt.get<std::string>(
            "umts_channel_mapping.transport.pch.address");
    transportNetworkEndpoints[1].port = pt.get<int>(
            "umts_channel_mapping.transport.pch.port");

    transportNetworkEndpoints[2].ipAddress = pt.get<std::string>(
            "umts_channel_mapping.transport.fach.address");
    transportNetworkEndpoints[2].port = pt.get<int>(
            "umts_channel_mapping.transport.fach.port");

    transportNetworkEndpoints[3].ipAddress = pt.get<std::string>(
            "umts_channel_mapping.transport.dch.address");
    transportNetworkEndpoints[3].port = pt.get<int>(
            "umts_channel_mapping.transport.dch.port");

    transportNetworkEndpoints[4].ipAddress = pt.get<std::string>(
            "umts_channel_mapping.transport.hs_dsch.address");
    transportNetworkEndpoints[4].port = pt.get<int>(
            "umts_channel_mapping.transport.hs_dsch.port");

}

void ChannelMappingConfig::initLogicalChannelMapping()
{

    boost::property_tree::ptree pt;
    boost::property_tree::xml_parser::read_xml(MAPPING_FILE_LOC, pt);

    logicalNetworkEndpoints[0].ipAddress = pt.get<std::string>(
            "umts_channel_mapping.logical.bcch.address");
    logicalNetworkEndpoints[0].port = pt.get<int>(
            "umts_channel_mapping.logical.bcch.port");

    logicalNetworkEndpoints[1].ipAddress = pt.get<std::string>(
            "umts_channel_mapping.logical.pcch.address");
    logicalNetworkEndpoints[1].port = pt.get<int>(
            "umts_channel_mapping.logical.pcch.port");

    logicalNetworkEndpoints[2].ipAddress = pt.get<std::string>(
            "umts_channel_mapping.logical.ccch.address");
    logicalNetworkEndpoints[2].port = pt.get<int>(
            "umts_channel_mapping.logical.ccch.port");

    logicalNetworkEndpoints[3].ipAddress = pt.get<std::string>(
            "umts_channel_mapping.logical.ctch.address");
    logicalNetworkEndpoints[3].port = pt.get<int>(
            "umts_channel_mapping.logical.ctch.port");

    logicalNetworkEndpoints[4].ipAddress = pt.get<std::string>(
            "umts_channel_mapping.logical.dcch.address");
    logicalNetworkEndpoints[4].port = pt.get<int>(
            "umts_channel_mapping.logical.dcch.port");

    logicalNetworkEndpoints[5].ipAddress = pt.get<std::string>(
            "umts_channel_mapping.logical.dtch.address");
    logicalNetworkEndpoints[5].port = pt.get<int>(
            "umts_channel_mapping.logical.dtch.port");

    logicalNetworkEndpoints[6].ipAddress = pt.get<std::string>(
            "umts_channel_mapping.logical.mcch.address");
    logicalNetworkEndpoints[6].port = pt.get<int>(
            "umts_channel_mapping.logical.mcch.port");

    logicalNetworkEndpoints[7].ipAddress = pt.get<std::string>(
            "umts_channel_mapping.logical.mtch.address");
    logicalNetworkEndpoints[7].port = pt.get<int>(
            "umts_channel_mapping.logical.mtch.port");
}

int ChannelMappingConfig::loadPhysicalChannel(std::string channelName)
{

    boost::property_tree::ptree pt;
    boost::property_tree::xml_parser::read_xml(MAPPING_FILE_LOC, pt);

    ip_address = pt.get<std::string>(
            "umts_channel_mapping.physical." + channelName + ".address");
    port = pt.get<int>(
            "umts_channel_mapping.physical." + channelName + ".port");

    return 0;
}

int ChannelMappingConfig::loadTransportChannel(std::string channelName)
{

    boost::property_tree::ptree pt;
    boost::property_tree::xml_parser::read_xml(MAPPING_FILE_LOC, pt);

    ip_address = pt.get<std::string>(
            "umts_channel_mapping.transport." + channelName + ".address");
    port = pt.get<int>(
            "umts_channel_mapping.transport." + channelName + ".port");

    return 0;
}

int ChannelMappingConfig::loadLogicalChannel(std::string channelName)
{

    boost::property_tree::ptree pt;
    boost::property_tree::xml_parser::read_xml(MAPPING_FILE_LOC, pt);

    ip_address = pt.get<std::string>(
            "umts_channel_mapping.logical." + channelName + ".address");
    port = pt.get<int>(
            "umts_channel_mapping.logical." + channelName + ".port");

    return 0;
}

int ChannelMappingConfig::getChannelEndpoint(physical::PhysicalChannel channel,
        ChannelNetworkEndpoint &channelNetworkEndpoint)
{
    channelNetworkEndpoint.ipAddress =
            physicalNetworkEndpoints[channel].ipAddress;
    channelNetworkEndpoint.port = physicalNetworkEndpoints[channel].port;

    return 0;
}

int ChannelMappingConfig::getChannelEndpoint(
        transport::TransportChannel channel,
        ChannelNetworkEndpoint &channelNetworkEndpoint)
{
    channelNetworkEndpoint.ipAddress =
            transportNetworkEndpoints[channel].ipAddress;
    channelNetworkEndpoint.port = transportNetworkEndpoints[channel].port;

    return 0;
}

int ChannelMappingConfig::getChannelEndpoint(logical::LogicalChannel channel,
        ChannelNetworkEndpoint &channelNetworkEndpoint)
{
    channelNetworkEndpoint.ipAddress =
            logicalNetworkEndpoints[channel].ipAddress;
    channelNetworkEndpoint.port = logicalNetworkEndpoints[channel].port;

    return 0;
}

std::string ChannelMappingConfig::getIPAddress()
{

    return ip_address;
}

int ChannelMappingConfig::getPort()
{

    return port;
}

} /* Namespace UMTS */
