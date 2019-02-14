#include <iostream>
#include "../request.hh"
#include <string>
#include <fstream>
int main()
{
    std::ifstream file("lol.txt");
    std::string str = "";
    std::string line ="";
    if (file.is_open())
    {
        while ( getline (file,line) )
        {
            str += line;
            str += "\n";
        }
        file.close();
    }
    http::Request r(str);
    std::cout << "method: "<< r.m_ << "\n";
    std::cout << "uri: "<< r.request_uri_ << "\n";
    std::cout << "version: "<< r.version_<<"\n";
    std::cout << "Headers: \n";
    for (auto it=r.headers_.begin(); it!=r.headers_.end(); ++it)
        std::cout << it->first << " => " << it->second << '\n';
    std::cout  << "body:" << r.msg_body_;
}
