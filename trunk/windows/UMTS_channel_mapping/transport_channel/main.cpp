#include <iostream>
#include "TransportChannelXYZ.hpp"

int main(int argc, char *argv[])
{
    UMTS::TransportChannelXYZ transportChannelXYZ;

    if (argv[1] == NULL)
    {
        std::cerr << "Channel must be specified as program argument."
                << std::endl;
        return -1;
    }

    transportChannelXYZ.run(argv[1]);

    return 0;
}
