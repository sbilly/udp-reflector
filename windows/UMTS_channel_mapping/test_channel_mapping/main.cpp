#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <ChannelMappingConfig.hpp>
#include <UMTS_Record_Types.hpp>

int main(int argc, char *argv[])
{
    UMTS::ChannelMappingConfig *channelMappingConfig =
            UMTS::ChannelMappingConfig::getInstance();

    boost::asio::io_service io_service;
    boost::asio::ip::udp::socket socket(io_service);
    socket.open(boost::asio::ip::udp::v4());

    while (true)
    {
        for (int physical = 0; physical < UMTS::NUM_PHYSICAL_CHANNELS; physical++)
        {
            for (int transport = 0; transport < UMTS::NUM_TRANSPORT_CHANNELS; transport++)
            {
                for (int logical = 0; logical < UMTS::NUM_LOGICAL_CHANNELS; logical++)
                {
                    UMTS::PhysicalDataFrameRecord physicalDataFrameRecord;
                    UMTS::ChannelNetworkEndpoint channelNetworkEndpoint;

                    physicalDataFrameRecord.physicalChannel =
                            UMTS::physical::PhysicalChannel(physical);
                    physicalDataFrameRecord.headerPhysicalDataX = 0;
                    physicalDataFrameRecord.headerPhysicalDataY = 0;
                    physicalDataFrameRecord.headerPhysicalDataZ = 0;

                    physicalDataFrameRecord.transportDataFrame.transportChannel =
                            UMTS::transport::TransportChannel(transport);
                    physicalDataFrameRecord.transportDataFrame.headerTransportDataX = 0;
                    physicalDataFrameRecord.transportDataFrame.headerTransportDataY = 0;
                    physicalDataFrameRecord.transportDataFrame.headerTransportDataZ = 0;

                    physicalDataFrameRecord.transportDataFrame.logicalDataFrame.logicalChannel =
                            UMTS::logical::LogicalChannel(logical);
                    physicalDataFrameRecord.transportDataFrame.logicalDataFrame.headerLogicalDataX = 0;
                    physicalDataFrameRecord.transportDataFrame.logicalDataFrame.headerLogicalDataY = 0;
                    physicalDataFrameRecord.transportDataFrame.logicalDataFrame.headerLogicalDataZ = 0;

                    channelMappingConfig->getChannelEndpoint(
                            physicalDataFrameRecord.physicalChannel,
                            channelNetworkEndpoint);

                    boost::asio::ip::udp::endpoint senderEndpoint =
                            boost::asio::ip::udp::endpoint(
                                    boost::asio::ip::address::from_string(
                                            channelNetworkEndpoint.ipAddress),
                                    channelNetworkEndpoint.port);

                    std::cout << "\nSimulating raw signal data on physical channels." << std::endl;
                    std::cout << "Destination IP Address = " << channelNetworkEndpoint.ipAddress;
                    std::cout << ", Destination Port = " << channelNetworkEndpoint.port << std::endl;
    
                    socket.send_to(boost::asio::buffer(
                            reinterpret_cast<void*>(&physicalDataFrameRecord),
                            sizeof(UMTS::PhysicalDataFrameRecord)),
                            senderEndpoint);

                    boost::this_thread::sleep(
                            boost::posix_time::milliseconds(10));
                }
            }
        }
    }
}
