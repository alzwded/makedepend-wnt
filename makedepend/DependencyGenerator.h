#ifndef DEPENDENCY_GENERATOR_H
#define DEPENDENCY_GENERATOR_H

#include "Path.h"
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <set>

class DependencyGenerator {
    std::vector<Path> includes_;
    std::map<std::string, std::set<std::string> > cache_;
private:
    bool findFile(const std::string& _name, std::string& foundFile, std::fstream& f);
    std::set<std::string> gatherDependencies(std::fstream& f);
public:
    std::string operator()(const std::string& f);
    DependencyGenerator(const std::vector<const char*>& _includes);
};

#endif
