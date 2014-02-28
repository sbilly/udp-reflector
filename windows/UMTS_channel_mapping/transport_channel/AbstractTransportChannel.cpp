#include <iostream>

#include <ChannelMappingConfig.hpp>
#include "AbstractTransportChannel.hpp"

namespace UMTS
{

void AbstractTransportChannel::run(std::string channelName)
{
    UMTS::ChannelMappingConfig *channelMappingConfig =
            UMTS::ChannelMappingConfig::getInstance();
    channelMappingConfig->loadTransportChannel(channelName);
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
        TransportDataFramePtr transportDataFramePtr(new TransportDataFrame);
        boost::asio::ip::udp::endpoint receiver_endpoint;

        // Blocking receive_from method processes data asynchronously when published
        transportDataFramePtr->length = socket.receive_from(
                boost::asio::buffer(transportDataFramePtr->payloadData),
                receiver_endpoint);

        processDataFrame(transportDataFramePtr, &socket);
    }
}

} /* Namespace UMTS */
