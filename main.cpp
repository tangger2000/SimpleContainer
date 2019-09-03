#include "dokidocker.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;
using namespace dokidocker;

const string help = {
        "This is a simple docker by Tanger!\n"
           "options:\n"
           "        -d  rootfs_dir     Your busybox file system dir. \n"
           "        -n  host_name      The hostname of the simple docker.\n"
};

int main(int argc, char** argv)
{
    int ch;
    docker_config config;
    while ((ch = getopt(argc,argv,"hn:d:"))!=-1)
    {
        switch (ch)
        {
            case 'n':
                config.host_name = optarg;
                break;
            case 'd':
                config.rootfs_dir = optarg;
                break;
            case 'h':
                cout << help << endl;
                return 0;
        }
    }

    cout << "DokiDocker Start!" <<endl;
    cout << "Attention: Please run this program with root privileges!" << endl;

    docker docker(config);
    docker.start();
    cout << "DokiDocker Exit!" <<endl;
    return 0;
}
