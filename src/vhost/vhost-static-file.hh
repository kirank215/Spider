/**
 * \file vhost/vhost-static-file.hh
 * \brief VHostStaticFile declaration.
 */

#pragma once

#include <map>
#include "config/config.hh"
#include "request/request.hh"
#include "vhost/connection.hh"
#include "vhost/vhost.hh"
#include "vhost/dispatcher.hh"

namespace http
{
    /**
     * \class VHostStaticFile
     * \brief VHost serving static files.
     */
    using remaining_iterator = int;

    inline std::map<std::string, std::string> content_types =
        {
           {  "bmp" , "image/bmp" },
           {  "csv" , "text/csv" },
           {  "doc" , "application/msword"},
           {  "dot" , "application/msword"},
           {  "exe" , "application/octet-stream" },
           {  "gif" , "image/gif" },
           {  "gz"  , "application/x-gzip" },
           {  "htm" , "text/html" },
           {  "html",  "text/html" },
           {  "jar" , "application/java-archive" },
           {  "json",  "application/json" },
           {  "jpeg",  "image/jpeg" },
           {  "jpg" , "image/jpeg" },
           {  "mp3" , "audio/mpeg" },
           {  "mp4" , "video/mp4" },
           {  "pdf" , "application/pdf" },
           {  "png" , "image/png" },
           {  "ppt" , "application/vnd.ms-powerpoint" },
           {  "sh", "application/x-sh"},
           {  "txt" , "text/plain" },
           {  "zip" , "application/x-zip-compressed"}
        };
    class VHostStaticFile : public VHost
    {
        public:
            friend class VHostFactory;
            virtual ~VHostStaticFile() = default;

        private:
            /**
             * \brief Constructor called by the factory.
             *
             * \param config VHostConfig virtual host configuration.
             */
            explicit VHostStaticFile(const VHostConfig&);

        public:
            /**
             * \brief Send response.
             *
             * \param req Request.
             * \param conn Connection.
             * \param begin remaining_iterator of received data.
             * \param end remaining_iterator of received data.
             *
             * Note that these iterators will only be useful starting from SRPS.
             */
            void respond(const Request&, Connection, remaining_iterator,
                    remaining_iterator) final;
    };

    void build_html(std::string&, std::string);


} // namespace http
