#ifndef LOGICAL_DATA_FRAME_HPP_
#define LOGICAL_DATA_FRAME_HPP_

#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>

namespace UMTS
{

typedef boost::array<unsigned char, 1536> Payload;

class LogicalDataFrame
{

public:

    Payload payloadData;
    unsigned int length;

    virtual ~LogicalDataFrame() {}
};

// The LogicalDataFramePtr will be automatically deallocated through
// the use of boost shared_ptr.
typedef boost::shared_ptr<LogicalDataFrame> LogicalDataFramePtr;

} /* Namespace UMTS */

#endif /* LOGICAL_DATA_FRAME_HPP_ */
