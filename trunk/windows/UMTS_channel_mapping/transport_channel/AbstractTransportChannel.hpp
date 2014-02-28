#ifndef ABSTRACT_TRANSPORT_CHANNEL_HPP_
#define ABSTRACT_TRANSPORT_CHANNEL_HPP_

#include <boost/asio.hpp>
#include "TransportDataFrame.hpp"

namespace UMTS
{

class AbstractTransportChannel
{

public:

    void run(std::string channelName);
    virtual ~AbstractTransportChannel(){};

protected:

    virtual void processDataFrame(TransportDataFramePtr transportDataFramePtr,
            boost::asio::ip::udp::socket *socket) = 0;
    std::string channelName;

private:

};

} /* Namespace UMTS */

#endif /* ABSTRACT_TRANSPORT_CHANNEL_HPP_ */
