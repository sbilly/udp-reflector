#include <iostream>

#include <UMTS_Record_Types.hpp>
#include <ChannelMappingConfig.hpp>
#include "LogicalChannelXYZ.hpp"

namespace UMTS
{

void LogicalChannelXYZ::processDataFrame(
        LogicalDataFramePtr logicalDataFramePtr,
        boost::asio::ip::udp::socket *socket)
{
    std::cout << "\nLogical channel (" << channelName << ") processing logical dataframe";
    std::cout << " (" << (sizeof(LogicalDataFrameRecord) - 16) << " bytes)." << std::endl;
}

} /* Namespace UMTS */
