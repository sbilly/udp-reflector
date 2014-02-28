#include <iostream>

#include <UMTS_Record_Types.hpp>
#include <ChannelMappingConfig.hpp>
#include "PhysicalChannelXYZ.hpp"
#include <stdio.h>

namespace UMTS
{

void PhysicalChannelXYZ::processDataFrame(
        PhysicalDataFramePtr physicalDataFramePtr,
        boost::asio::ip::udp::socket *socket)
{
    PhysicalDataFrameRecord *physicalDataFrameRecord =
            reinterpret_cast<PhysicalDataFrameRecord*>(&physicalDataFramePtr.get()->payloadData);
    ChannelMappingConfig *channelMappingConfig =
            UMTS::ChannelMappingConfig::getInstance();
    ChannelNetworkEndpoint channelNetworkEndpoint;

    channelMappingConfig->getChannelEndpoint(
            physicalDataFrameRecord->transportDataFrame.transportChannel,
            channelNetworkEndpoint);

    boost::asio::ip::udp::endpoint senderEndpoint =
            boost::asio::ip::udp::endpoint(
                    boost::asio::ip::address::from_string(channelNetworkEndpoint.ipAddress),
                    channelNetworkEndpoint.port);

    // Forward the transport data frame to the appropriate transport channel.

    int bytesSent = socket->send_to(boost::asio::buffer(
       reinterpret_cast<void*>(&physicalDataFrameRecord->transportDataFrame),
       sizeof(TransportDataFrameRecord)), senderEndpoint);

    std::cout << "\nPhysical channel (" << channelName;
    std::cout << ") forwarding transport dataframe (" << bytesSent <<
            " bytes) to transport channel." << std::endl;
    std::cout << "Destination IP Address = " << channelNetworkEndpoint.ipAddress;
    std::cout << ", Destination Port = " << channelNetworkEndpoint.port << std::endl;
}

} /* Namespace UMTS */
