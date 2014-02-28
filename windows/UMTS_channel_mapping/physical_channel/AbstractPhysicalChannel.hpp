#ifndef ABSTRACT_PHYSICAL_CHANNEL_HPP_
#define ABSTRACT_PHYSICAL_CHANNEL_HPP_

#include <boost/asio.hpp>
#include "PhysicalDataFrame.hpp"

namespace UMTS
{

class AbstractPhysicalChannel
{

public:

    void run(std::string channelName);
    virtual ~AbstractPhysicalChannel(){};

protected:

    virtual void processDataFrame(PhysicalDataFramePtr physicalDataFramePtr,
            boost::asio::ip::udp::socket *socket) = 0;
    std::string channelName;

private:

};

} /* Namespace UMTS */

#endif /* ABSTRACT_PHYSICAL_CHANNEL_HPP_ */
