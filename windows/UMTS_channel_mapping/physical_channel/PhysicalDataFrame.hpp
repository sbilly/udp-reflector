#ifndef PHYSICAL_DATA_FRAME_HPP_
#define PHYSICAL_DATA_FRAME_HPP_

#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>

namespace UMTS
{

typedef boost::array<unsigned char, 1536> Payload;

class PhysicalDataFrame
{

public:

    Payload payloadData;
    unsigned int length;

    virtual ~PhysicalDataFrame() {}
};

// The PhysicalDataFramePtr will be automatically deallocated through
// the use of boost shared_ptr.
typedef boost::shared_ptr<PhysicalDataFrame> PhysicalDataFramePtr;

} /* Namespace UMTS */

#endif /* PHYSICAL_DATA_FRAME_HPP_ */
