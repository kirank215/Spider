#include "error/not-implemented.hh"
#include "config/config.hh"

int main(int argc, char *argv[])
{
    using namespace http;
    int i = 1;
    for(; i < argc; i++)
    {
        if(argv[i][0] != '-')
            break;
    }
    struct ServerConfig serveurconfig = http::parse_configuration(argv[i]);
    return 1;
}
