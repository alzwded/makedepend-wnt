#ifndef BUILD_DEPEND_H
#define BUILD_DEPEND_H

#include <vector>
#include "DependencyGenerator.h"

class BuildDepend {
    DependencyGenerator gDep_;
public:
    BuildDepend(const std::vector<const char*>& _includes)
        : gDep_(_includes)
        {}
    void operator()(const std::string& _infile, const std::string& _outfile);
};

#endif
