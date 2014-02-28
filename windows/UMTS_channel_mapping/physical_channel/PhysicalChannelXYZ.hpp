#ifndef PHYSICAL_CHANNEL_XYZ_HPP_
#define PHYSICAL_CHANNEL_XYZ_HPP_

#include "PhysicalDataFrame.hpp"
#include "AbstractPhysicalChannel.hpp"

namespace UMTS
{

/*
 * Physical Channel Derived Class
 *
 * A real implementation would include a derived class for each of the
 * possible types of physical channels, with respect to the physical
 * channel specification.
 *
 */
class PhysicalChannelXYZ: public AbstractPhysicalChannel
{

public:

    PhysicalChannelXYZ(){};

    ~PhysicalChannelXYZ(){};

protected:

    void processDataFrame(PhysicalDataFramePtr physicalDataFramePtr,
            boost::asio::ip::udp::socket *socket);

};

} /* Namespace UMTS */

#endif /* PHYSICAL_CHANNEL_XYZ_HPP_ */
