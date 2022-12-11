#include "uri.h"
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
    // param_scheme unittest
    std::string raw_str_1 = "file:///home/ajin/Desktop/ajpj/cpppls/test/main.pl";
    std::string raw_str_2 = "file";
    std::string raw_str_3 = "1file";
    URI *uri_1 = new URI(raw_str_1);

//  URI *uri_2 = new URI(raw_str_2);
//  URI *uri_3 = new URI(raw_str_3);
    std::cout <<"scheme: "<< uri_1->get_scheme() << std::endl;
    std::cout <<"path: "<< uri_1->get_path() << std::endl;

    delete uri_1;

//  delete uri_2;
//  delete uri_3;

    return 0;
}
