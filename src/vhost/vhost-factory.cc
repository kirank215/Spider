#include "vhost/vhost-factory.hh"

namespace http
{
    shared_vhost VHostFactory::Create(const VHostConfig& v)
    {
        return std::shared_ptr<VHostStaticFile>(new VHostStaticFile(v));
    }
}
