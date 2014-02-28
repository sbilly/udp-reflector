#ifndef TRANSPORT_CHANNEL_XYZ_HPP_
#define TRANSPORT_CHANNEL_XYZ_HPP_

#include "TransportDataFrame.hpp"
#include "AbstractTransportChannel.hpp"

namespace UMTS
{

/*
 * Transport Channel Derived Class
 *
 * A real implementation would include a derived class for each of the
 * possible types of transport channels, with respect to the transport
 * channel specification.
 *
 */
class TransportChannelXYZ: public AbstractTransportChannel
{

public:

    TransportChannelXYZ(){};

    ~TransportChannelXYZ(){};

protected:

    void processDataFrame(TransportDataFramePtr transportDataFramePtr,
            boost::asio::ip::udp::socket *socket);

};

} /* Namespace UMTS */

#endif /* TRANSPORT_CHANNEL_XYZ_HPP_ */
