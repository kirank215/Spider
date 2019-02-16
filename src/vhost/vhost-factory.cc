#include "vhost/vhost-factory.hh"

namespace http
{
    shared_vhost VHostFactory::Create(VHostConfig v)
    {
        static shared_vhost s_vh = std::make_shared<VHost>(VHost(v));
        return s_vh;
    }
}
