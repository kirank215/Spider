#include "addrinfo.hh"

namespace misc
{
    AddrInfo getaddrinfo(const char* node, const char* service,
                         const AddrInfoHint& hints)
    {
        struct addrinfo* res;

        auto rc = ::getaddrinfo(node, service, &hints, &res);
        if (rc)
            throw std::system_error(rc, addrinfo_error_category(),
                                    "getaddrinfo");

        return AddrInfo(res);
    }
} // namespace misc

std::ostream& operator<<(std::ostream& os, const addrinfo& addr)
{
    return os << "Family: " << addr.ai_family << ", Flags: " << addr.ai_flags
              << ", SockType: " << addr.ai_socktype << ", Protocol; "
              << addr.ai_protocol << ", Addrlen: " << addr.ai_addrlen
              << ", Addr: " << addr.ai_addr;
}
