#include <iostream>
#include "LogicalChannelXYZ.hpp"

int main(int argc, char *argv[])
{
    UMTS::LogicalChannelXYZ logicalChannelXYZ;

    if (argv[1] == NULL)
    {
        std::cerr << "Channel must be specified as program argument."
                << std::endl;
        return -1;
    }

    logicalChannelXYZ.run(argv[1]);

    return 0;
}
