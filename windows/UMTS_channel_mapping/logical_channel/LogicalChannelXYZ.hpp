#ifndef LOGICAL_CHANNEL_XYZ_HPP_
#define LOGICAL_CHANNEL_XYZ_HPP_

#include "LogicalDataFrame.hpp"
#include "AbstractLogicalChannel.hpp"

namespace UMTS
{

/*
 * Logical Channel Derived Class
 *
 * A real implementation would include a derived class for each of the
 * possible types of logical channels, with respect to the logical
 * channel specification.
 *
 */
class LogicalChannelXYZ: public AbstractLogicalChannel
{

public:

    LogicalChannelXYZ(){};

    ~LogicalChannelXYZ(){};

protected:

    void processDataFrame(LogicalDataFramePtr logicalDataFramePtr,
            boost::asio::ip::udp::socket *socket);

};

} /* Namespace UMTS */

#endif /* LOGICAL_CHANNEL_XYZ_HPP_ */
