#include <iostream>
#include "PhysicalChannelXYZ.hpp"
#include <UMTS_Record_Types.hpp>
#include <stdio.h>

int main(int argc, char *argv[])
{
    UMTS::PhysicalChannelXYZ physicalChannelXYZ;

    if (argv[1] == NULL)
    {
        std::cerr << "Channel must be specified as program argument."
                << std::endl;
        return -1;
    }

    physicalChannelXYZ.run(argv[1]);

    return 0;
}
