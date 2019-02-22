/**
 * \file vhost/vhost-factory.hh
 * \brief VHostFactory
 */

#include "vhost/vhost.hh"
#include "vhost/vhost-static-file.hh"

namespace http
{
    /**
     * \class VHostFactory
     * \brief Factory design pattern to create VHost.
     */
    class VHostFactory
    {
    public:
        /**
         * \brief Create a VHost object from a given VHostConfig.
         */
        static shared_vhost Create(const VHostConfig&);
    };
} // namespace http
