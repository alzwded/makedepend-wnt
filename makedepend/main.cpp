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

#define USAGE do{ \
            std::cout << "usage: " << argv[0] << " /IincludeDir1 /IincludeDir2 ... /Ooutfile.d file.cpp" << std::endl; \
			return 1; \
}while(0)

int main(int argc, char* argv[])
{
	if(argc <= 1) {
		USAGE;
	}
	for(int i =1; i < argc; ++i)
	{
        if(strncmp(argv[i], "/I", 2) == 0) {
            includes_.push_back(&argv[i][2]);
        } else if(strncmp(argv[i], "/?", 2) == 0) {
			USAGE;
        } else if(strncmp(argv[i], "/O", 2) == 0) {
            output_ = std::string(&argv[i][2]);
        } else {
            input_ = std::string(argv[i]);
            BuildDepend fctor(includes_);
            fctor(input_, output_);
        }
	}

#ifdef TEST_MODE
	std::ifstream depFile("obj/main.d");
	std::stringstream ss;
	std::string s;
	bool firstLine = true;
	bool delim = true;
	while(!depFile.eof()) {
		std::getline(depFile, s, '\n');
		if(firstLine) {
			if(!(s.find("src\\main.cpp") == 0)) {
				return -1;
			}
			firstLine = false;
		}
		if(!s.empty() && s[s.length() - 1] == '\\' && !delim) return -2;
		else if(!s.empty() && s[s.length() - 1] != '\\') delim = false;
		ss << s;
	}
	s = ss.str();
	size_t b_h = s.find("inc\\b.h");
	size_t a_h = s.find("inc\\blank\\a.h");
	size_t c_h = s.find("inc\\blank\\c.h");
	size_t d_h = s.find("inc\\d.h");
	size_t pass1_h = s.find("inc\\pass1.h");
	size_t pass2_h = s.find("inc\\pass2.h");
	size_t fail_h = s.find("fail.h");
	if(fail_h != std::string::npos) return -3;
	if(a_h == std::string::npos) return -4;
	if(b_h == std::string::npos) return -5;
	if(c_h == std::string::npos) return -6;
	if(d_h == std::string::npos) return -7;
	if(pass1_h == std::string::npos) return -8;
	if(pass2_h == std::string::npos) return -9;
#endif

	return 0;
}
