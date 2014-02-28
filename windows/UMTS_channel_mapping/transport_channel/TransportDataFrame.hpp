#ifndef TRANSPORT_DATA_FRAME_HPP_
#define TRANSPORT_DATA_FRAME_HPP_

#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>

namespace UMTS
{

typedef boost::array<unsigned char, 1536> Payload;

class TransportDataFrame
{

public:

    Payload payloadData;
    unsigned int length;

    virtual ~TransportDataFrame() {}
};

// The TransportDataFramePtr will be automatically deallocated through
// the use of boost shared_ptr.
typedef boost::shared_ptr<TransportDataFrame> TransportDataFramePtr;

} /* Namespace UMTS */

#endif /* TRANSPORT_DATA_FRAME_HPP_ */
