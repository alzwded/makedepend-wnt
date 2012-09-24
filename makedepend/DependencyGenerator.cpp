#include "DependencyGenerator.h"
#include <utility>
#include <fstream>

DependencyGenerator::DependencyGenerator(
        const std::vector<const char*>& _includes)
{
    for(std::vector<const char*>::const_iterator i = _includes.begin();
        i != _includes.end();
        ++i)
    {
        Path p(*i);
        includes_.push_back(Path(*i));
    }
}

std::set<std::string> DependencyGenerator::gatherDependencies(std::fstream& f)
{
	bool insideComment = false;
    std::string s;
    std::set<std::string> ret;
	size_t p, end, beginComment, endComment, lineComment;

    while(!f.eof()) {
        std::getline(dynamic_cast<std::istream&>(f), s, '\n');
recalculateLine:
		beginComment = s.find("/*");
		endComment = s.find("*/");
		p = s.find("#include");
		lineComment = s.find("//");
		if(p == std::string::npos) {
			if(insideComment
				&& endComment != std::string::npos
				&& (lineComment == std::string::npos || lineComment > endComment))
			{
				insideComment = false;
				s = s.substr(endComment + 1);
			}
			if(!insideComment
				&& beginComment != std::string::npos
				&& (lineComment == std::string::npos || lineComment > beginComment))
			{
				insideComment = true;
				goto recalculateLine;
			}
			continue;
		}
		if(lineComment != std::string::npos
			&& p > lineComment)
		{
			continue;
		}
		if(endComment == std::string::npos && insideComment)
		{
			continue;
		} else if(insideComment) {
			s = s.substr(endComment + 1);
			insideComment = false;
			goto recalculateLine;
		}
		if(beginComment == std::string::npos && endComment == std::string::npos) { goto pullInclude; }
		if(endComment == std::string::npos
			&& beginComment != std::string::npos
			&& p > beginComment)
		{
			insideComment = true;
			continue;
		}
		if(beginComment != std::string::npos 
			&& endComment != std::string::npos
			&& p > beginComment
			&& p < endComment)
		{
			s = s.substr(endComment + 1);
			goto recalculateLine;
		}
		else if(beginComment != std::string::npos
			&& endComment != std::string::npos
			&& p > beginComment) {
			s = s.substr(endComment + 1);
			goto recalculateLine;
		}
pullInclude:
        {
			insideComment = false;
            p = s.find("\"");
            if(p != std::string::npos) {
                end = s.rfind("\"");
                if(end == std::string::npos) {
                    continue;
                }
            } else {
                p = s.find("<");
                end = s.rfind(">");
                if(p == std::string::npos || end == std::string::npos) {
                    continue;
                }
            }
            std::string depend = s.substr(p + 1, end - p - 1);
            std::map<std::string, std::set<std::string> >
                ::iterator cachedDepend = 
                    cache_.find(depend);
            if(cachedDepend != cache_.end()) {
                ret.insert(
                        cachedDepend->second.begin(),
                        cachedDepend->second.end());
            } else {
                std::fstream included;
                std::string foundFile;
                if(findFile(depend, foundFile, included)) {
                    std::set<std::string> includedIncludes;
                    // prevent cyclic dependency infinite loop
                    cache_.insert(
                            std::pair<std::string, std::set<std::string> >(
                                depend,
                                includedIncludes));
                    includedIncludes =
                            gatherDependencies(included);
                    cache_[depend] = includedIncludes;
                    includedIncludes.insert(foundFile);
                    ret.insert(
                            includedIncludes.begin(),
                            includedIncludes.end());
                    included.close();
                }
            }
        }
    }

    return ret;
}

bool DependencyGenerator::findFile(const std::string& _name, std::string& foundFile, std::fstream& f)
{
    f.open(_name);
    if(f.is_open()) { foundFile = _name; return true; }
    for(std::vector<Path>::iterator i = includes_.begin();
            i != includes_.end();
            ++i)
    {
        Path pth(i->Get() + Path::PATH_SEPARATOR_CHAR + _name);
        f.open(pth.Get());
        if(f.is_open()) { foundFile = pth.Get(); return true; }
    }
    return false;
}

std::string DependencyGenerator::operator()(const std::string& _fname)
{
    std::ostringstream s;
    std::fstream f;
    std::string foundFile;
	Path filePath(_fname);
    if(!findFile(filePath.Get(), foundFile, f)) return "";

    std::set<std::string> deps = gatherDependencies(f);
    f.close();

    s << foundFile << ": ";
    std::set<std::string>::iterator i = deps.begin();
    s << *i << " ";
    for(++i;
            i != deps.end();
            ++i)
    {
        s << "\\\n\t" << *i << " ";
    }
    s << "\n";

    return s.str();
}
