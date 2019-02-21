#include "vhost/vhost-factory.hh"
#include "vhost/vhost-static-file.hh"

namespace http
{
    shared_vhost VHostFactory::Create(VHostConfig v)
    {
        static shared_vhost s_vh = std::make_shared<VHost>(VHostStaticFile(v));
        return s_vh;
    }
}
