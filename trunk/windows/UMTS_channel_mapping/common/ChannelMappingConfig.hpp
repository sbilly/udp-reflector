#ifndef CHANNEL_MAPPING_CONFIG_HPP_
#define CHANNEL_MAPPING_CONFIG_HPP_

#include "UMTS_Record_Types.hpp"

namespace UMTS
{

class ChannelMappingConfig
{

public:

    static ChannelMappingConfig* getInstance();

    /* Load network settings based on channel type and name */
    int loadPhysicalChannel(std::string channelName);
    int loadTransportChannel(std::string channelName);
    int loadLogicalChannel(std::string channelName);

    /* Get a channel network endpoint configuration */
    int getChannelEndpoint(physical::PhysicalChannel channel,
            ChannelNetworkEndpoint &channelNetworkEndpoint);
    int getChannelEndpoint(transport::TransportChannel channel,
            ChannelNetworkEndpoint &channelNetworkEndpoint);
    int getChannelEndpoint(logical::LogicalChannel channel,
            ChannelNetworkEndpoint &channelNetworkEndpoint);

    std::string getIPAddress();
    int getPort();

protected:

    ChannelMappingConfig();

private:

    void initPhysicalChannelMapping();
    void initTransportChannelMapping();
    void initLogicalChannelMapping();
};

} /* Namespace UMTS */

#endif /* CHANNEL_MAPPING_CONFIG_HPP_ */
