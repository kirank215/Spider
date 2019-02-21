#include "vhost/vhost-factory.hh"
#include "vhost/vhost-static-file.hh"

namespace http
{
    shared_vhost VHostFactory::Create(const VHostConfig& v)
    {
        return std::shared_ptr<VHostStaticFile>(new VHostStaticFile(v));
    }
}
