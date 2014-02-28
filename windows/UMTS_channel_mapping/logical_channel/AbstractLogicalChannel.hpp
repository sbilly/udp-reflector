#ifndef ABSTRACT_LOGICAL_CHANNEL_HPP_
#define ABSTRACT_LOGICAL_CHANNEL_HPP_

#include <boost/asio.hpp>
#include "LogicalDataFrame.hpp"

namespace UMTS
{

class AbstractLogicalChannel
{

public:

    void run(std::string channelName);
    virtual ~AbstractLogicalChannel(){};

protected:

    virtual void processDataFrame(LogicalDataFramePtr logicalDataFramePtr,
            boost::asio::ip::udp::socket *socket) = 0;
    std::string channelName;

private:

};

} /* Namespace UMTS */

#endif /* ABSTRACT_LOGICAL_CHANNEL_HPP_ */
