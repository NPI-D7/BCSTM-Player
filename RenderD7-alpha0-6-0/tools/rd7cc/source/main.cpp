//rd7cc
#include <iostream>
#include <fstream>
int main(int argc, char* argv[])
{
    std::ofstream result ("result.hpp");

    result << "//Result" << std::endl;

    result.close();
}
