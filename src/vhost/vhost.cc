#include "vhost/vhost.hh"

namespace http
{
    VHost::VHost(const VHostConfig& v)
    {
        conf_ = v;
    }
}
