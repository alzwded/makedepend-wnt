#include "BuildDepend.h"
#include "DependencyGenerator.h"
#include "Path.h"
#include <fstream>
#include <string>

void BuildDepend::operator()(const std::string& _infile, const std::string& _outfile)
{
    std::string outfile = _outfile;
    if(outfile.empty()) {
        Path outpath(_infile);
        outfile = outpath.RHead() + ".d";
    }

    std::fstream fout(outfile.c_str(), std::ios::out);
    if(!fout.is_open()) { return; }
    fout << gDep_(_infile);
    fout.close();
}
