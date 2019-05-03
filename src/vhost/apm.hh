#pragma once
#include <map>


namespace http
{
    inline std::map<std::string, int> APM = {
        { (std::pair<std::string,int>("global_connections_active" , 0))},
        { (std::pair<std::string,int>("global_connections_reading",  0))},
        { (std::pair<std::string,int>("global_connections_writing",  0))},
        { (std::pair<std::string,int>("global_requests_2xx",  0))},
        { (std::pair<std::string,int>("global_requests_4xx",  0))},
        { (std::pair<std::string,int>("global_requests_5xx",  0))},
        { (std::pair<std::string,int>("global_requests_nb" , 0))}
        };
    inline int APM_add(std::map<std::string, int> apm, std::string s)
    {
        if(apm.find(s) != apm.end())
        {
            apm[s]++;
            return 1;
        }
        return 0;
    }

    inline int APM_sub(std::map<std::string, int> apm, std::string s)
    {
        if(apm.find(s) != apm.end())
        {
            apm[s]--;
            apm[s] = apm[s]<0 ? 0 : apm[s];
            return 1;
        }
        return 0;
    }
}
