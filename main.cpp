#include "webserv.hpp"
#include "Cluster.hpp"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Bad number of arguments" << std::endl;
        return 1;
    }
    Cluster cluster;

    cluster.run();
}