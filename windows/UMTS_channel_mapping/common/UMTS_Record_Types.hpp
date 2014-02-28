#ifndef UMTS_RECORD_TYPES_HPP_
#define UMTS_RECORD_TYPES_HPP_

namespace UMTS
{

namespace physical
{
enum PhysicalChannel
{
    P_CCPCH, S_CCPCH, D_DPCCH, HS_SCCH, LAST
};
} /* Namespace physical */

namespace transport
{
enum TransportChannel
{
    BCH, PCH, FACH, DCH, HS_DSCH, LAST
};
} /* Namespace transport */

namespace logical
{
enum LogicalChannel
{
    BCCH, PCCH, CCCH, CTCH, DCCH, DTCH, MCCH, MTCH, LAST
};
} /* Namespace logical */

const int NUM_PHYSICAL_CHANNELS = physical::LAST;
const int NUM_TRANSPORT_CHANNELS = transport::LAST;
const int NUM_LOGICAL_CHANNELS = logical::LAST;

class ChannelNetworkEndpoint
{
public:
    std::string ipAddress;
    int port;
};

class LogicalDataFrameRecord
{
public:
    // Start Logical DataFrame Header
    logical::LogicalChannel logicalChannel;
    int headerLogicalDataX;
    int headerLogicalDataY;
    int headerLogicalDataZ;

    // Start Logical DataFrame Body
    int data[128];
};

class TransportDataFrameRecord
{
public:
    // Start Transport DataFrame Header
    transport::TransportChannel transportChannel;
    int headerTransportDataX;
    int headerTransportDataY;
    int headerTransportDataZ;

    // Start Transport DataFrame Body
    LogicalDataFrameRecord logicalDataFrame;
};

class PhysicalDataFrameRecord
{
public:
    // Start PhysicalDataFrame Header
    physical::PhysicalChannel physicalChannel;
    int headerPhysicalDataX;
    int headerPhysicalDataY;
    int headerPhysicalDataZ;

    // Start PhysicalDataFrame Body
    TransportDataFrameRecord transportDataFrame;
};

} /* Namespace UMTS */

#endif /* UMTS_RECORD_TYPES_HPP_ */
