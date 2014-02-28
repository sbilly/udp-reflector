#include <iostream>

#include <UMTS_Record_Types.hpp>
#include <ChannelMappingConfig.hpp>
#include "TransportChannelXYZ.hpp"

namespace UMTS
{

void TransportChannelXYZ::processDataFrame(
        TransportDataFramePtr transportDataFramePtr,
        boost::asio::ip::udp::socket *socket)
{
    TransportDataFrameRecord *transportDataFrameRecord =
            reinterpret_cast<TransportDataFrameRecord*>(&transportDataFramePtr.get()->payloadData);
    ChannelMappingConfig *channelMappingConfig =
            UMTS::ChannelMappingConfig::getInstance();
    ChannelNetworkEndpoint channelNetworkEndpoint;

    channelMappingConfig->getChannelEndpoint(
            transportDataFrameRecord->logicalDataFrame.logicalChannel,
            channelNetworkEndpoint);

    boost::asio::ip::udp::endpoint senderEndpoint =
            boost::asio::ip::udp::endpoint(
                    boost::asio::ip::address::from_string(channelNetworkEndpoint.ipAddress),
                    channelNetworkEndpoint.port);

    // Forward the transport data frame to the appropriate transport channel.

    int bytesSent = socket->send_to(boost::asio::buffer(
       reinterpret_cast<void*>(&transportDataFrameRecord->logicalDataFrame),
       sizeof(LogicalDataFrameRecord)), senderEndpoint);

    std::cout << "\nTransport channel (" << channelName;
    std::cout << ") forwarding logical dataframe (" << bytesSent <<
            " bytes) to logical channel." << std::endl;
    std::cout << "Destination IP Address = " << channelNetworkEndpoint.ipAddress;
    std::cout << ", Destination Port = " << channelNetworkEndpoint.port << std::endl;
}

} /* Namespace UMTS */
