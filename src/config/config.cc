#include "config/config.hh"

#include "error/not-implemented.hh"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Winline"
#ifdef __clang__
#pragma GCC diagnostic ignored "-Wlogical-op-parentheses"
#endif
#include <json.hpp>
#include <fstream>
#pragma GCC diagnostic pop

using json = nlohmann::json;

using namespace http;

static json create_json(const std::string path)
{
    std::ifstream file(path);
    json j = json::parse(file);
    return j;
}

struct ServerConfig http::parse_configuration(const std::string& path)
{
    struct ServerConfig SC;
    json j = create_json(path);
    std::vector<VHostConfig> list_vhost;
    for(auto elt : j)
    {
        struct VHostConfig v;
        if(elt["ip"] && elt["port"] && elt["server_name"] && elt["root"])
        {
            v.ip = elt["ip"];
            v.server_name = elt["server_name"];
            v.port = elt["port"];
            v.root = elt["root"];
            if(elt["default_file"])
                v.default_file = elt["default_file"];
            list_vhost.push_back(v);
        }
    }
    SC.list_vhost = list_vhost;
    return SC;
}
