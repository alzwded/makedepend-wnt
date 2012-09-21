#include <fstream>
#include <cstring>
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>

#include "BuildDepend.h"

std::vector<const char*> includes_;
std::list<const char*> inputs_;
std::string output_;
std::string input_;

int main(int argc, char* argv[])
{
	for(int i =1; i < argc; ++i)
	{
        if(strncmp(argv[i], "/I", 2) == 0) {
            includes_.push_back(&argv[i][2]);
        } else if(strncmp(argv[i], "/?", 2) == 0) {
            std::cout << "usage: " << argv[0] << " /IincludeDir1 /IincludeDir2 ... /Ooutfile.d file.cpp" << std::endl;
        } else if(strncmp(argv[i], "/O", 2) == 0) {
            output_ = std::string(&argv[i][2]);
        } else {
            input_ = std::string(argv[i]);
            BuildDepend fctor(includes_);
            fctor(input_, output_);
        }
	}
}
