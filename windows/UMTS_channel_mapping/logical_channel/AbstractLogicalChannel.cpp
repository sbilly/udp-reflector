#include <iostream>

#include <ChannelMappingConfig.hpp>
#include "AbstractLogicalChannel.hpp"

namespace UMTS
{

void AbstractLogicalChannel::run(std::string channelName)
{
    UMTS::ChannelMappingConfig *channelMappingConfig =
            UMTS::ChannelMappingConfig::getInstance();
    channelMappingConfig->loadLogicalChannel(channelName);
    this->channelName = channelName;

    boost::asio::io_service io_service;

    boost::asio::ip::udp::endpoint local_endpoint =
            boost::asio::ip::udp::endpoint(
                    boost::asio::ip::address::from_string(
                            channelMappingConfig->getIPAddress()),
                    channelMappingConfig->getPort());

    boost::asio::ip::udp::socket socket(io_service);
    socket.open(boost::asio::ip::udp::v4());
    socket.bind(local_endpoint);

    while (true)
    {
        LogicalDataFramePtr logicalDataFramePtr(new LogicalDataFrame);
        boost::asio::ip::udp::endpoint receiver_endpoint;

        // Blocking receive_from method processes data asynchronously when published
        logicalDataFramePtr->length = socket.receive_from(
                boost::asio::buffer(logicalDataFramePtr->payloadData),
                receiver_endpoint);

        processDataFrame(logicalDataFramePtr, &socket);
    }
}

} /* Namespace UMTS */
