#include "vhost/vhost-factory.hh"
#include "vhost/vhost-static-file.hh"

namespace http
{
    shared_vhost VHostFactory::Create(const VHostConfig& v)
    {
        
        auto s_vh = std::make_shared<VHostStaticFile>(std::move(VHostStaticFile(v)));
        return s_vh;
    }
}
