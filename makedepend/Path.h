#ifndef PATH_H
#define PATH_H

#include <string>
#include <list>
#include <sstream>

#define OTHER_PATH_SEP '/'

class Path {
    std::list<std::string> nodes_;
    bool beginSlash_;

	std::string cacheFull_;
	std::string cacheRHead_;
	std::string cacheExtension_;
	std::string cacheHead_;
	std::string cacheFileName_;

	void ClearCache()
	{
		cacheFull_ = "";
		cacheRHead_ = "";
		cacheHead_ = "";
		cacheExtension_ = "";
		cacheFileName_ = "";
	}

    void Init(const std::string& _s)
    {
		ClearCache();
        std::string s(_s);
        for(size_t i = 0; i < s.length(); ++i) {
            if(s[i] == OTHER_PATH_SEP) s[i] = PATH_SEPARATOR_CHAR;
        }
        beginSlash_ = (!s.empty() && s[0] == PATH_SEPARATOR_CHAR);
        size_t p;
        std::string current(s);
		if(!beginSlash_) {
			if((p = current.find(PATH_SEPARATOR_CHAR)) != std::string::npos) {
				nodes_.push_back(current.substr(0, p));
			} else {
				nodes_.push_back(current);
				return;
			}
		}
        while((p = current.find(PATH_SEPARATOR_CHAR)) != std::string::npos)
        {
            size_t i = p + 1;
            while(i < current.length() && current[i] == PATH_SEPARATOR_CHAR) ++i;
            if(i >= current.length()) break;
            size_t j = i + 1;
            while(j < current.length() && current[j] != PATH_SEPARATOR_CHAR) ++j;
            nodes_.push_back(current.substr(i, j - i));
			if(j < current.length()) current = current.substr(j);
			else break;
        }
		//if(!current.empty()) nodes_.push_back(current);
    }
public:
    static const char PATH_SEPARATOR_CHAR = '\\';
    Path(const std::string& _s)
    {
        Init(_s);
    }
    Path(const char* _s)
    {
        std::string s(_s);
        Init(s);
    }
    void Push(const std::string& _node)
    {
		ClearCache();
        nodes_.push_back(_node);
    }
    void Pop()
    {
        if(nodes_.empty()) return;
        nodes_.pop_back();
		ClearCache();
    }
    std::string Get()
    {
		if(!cacheFull_.empty()) return cacheFull_;

        std::stringstream ret;
        if(beginSlash_) ret << PATH_SEPARATOR_CHAR;
        if(nodes_.empty()) return ret.str();
        std::list<std::string>::const_iterator i = nodes_.begin();
        ret << *i;
        for(++i;
            i != nodes_.end();
            ++i)
        {
            ret << PATH_SEPARATOR_CHAR << *i;
        }

		cacheFull_ = ret.str();
        return ret.str();
    }
    std::string Head()
    {
		if(!cacheHead_.empty()) return cacheHead_;
        std::stringstream ret;
        if(beginSlash_) ret << PATH_SEPARATOR_CHAR;
        if(nodes_.empty()) return ret.str();
        if(nodes_.size() <= 1) return ret.str();
        std::list<std::string>::const_iterator i = nodes_.begin();
        ret << *i;
        std::string tail = nodes_.back();
        nodes_.pop_back();
        for(++i;
            i != nodes_.end();
            ++i)
        {
            ret << PATH_SEPARATOR_CHAR << *i;
        }
        nodes_.push_back(tail);
		cacheHead_ = ret.str();
        return ret.str();
    }
    std::string Tail()
    {
        if(nodes_.empty()) return "";
        return nodes_.back();
    }
    std::string RHead()
    {
		if(!cacheRHead_.empty()) return cacheRHead_;
        std::string ret = Get();
        size_t p = ret.rfind(".");
        size_t psep = ret.rfind(PATH_SEPARATOR_CHAR);
        if(p != std::string::npos && p > psep) {
			cacheRHead_ = ret.substr(0, p);
        } else {
			cacheRHead_ = ret;
        }
		return cacheRHead_;
    }
    std::string FileName()
    {
		if(!cacheFileName_.empty()) return cacheFileName_;
        if(nodes_.empty()) return "";
        std::string ret = nodes_.back();
        size_t p = ret.rfind(".");
        if(p != std::string::npos) {
			cacheFileName_ = ret.substr(0, p);
        } else {
            cacheFileName_ = ret;
        }
		return cacheFileName_;
    }
    std::string Extension()
    {
		if(!cacheExtension_.empty()) return cacheExtension_;
        if(nodes_.empty()) return "";
        std::string ret = nodes_.back();
        size_t p = ret.rfind(".");
        if(p != std::string::npos && p < ret.length() - 1) {
			cacheExtension_ = ret.substr(p + 1);
            return cacheExtension_;
        } else {
            return "";
        }
    }
    std::string GetNode(size_t _i)
    {
        if(_i < 0 || _i >= nodes_.size()) return "";
        std::list<std::string>::iterator i = nodes_.begin();
        advance(i, _i);
        return *i;
    }
    bool& RootPath()
    {
        return beginSlash_;
    }
    void RemoveNode(size_t _i)
    {
		ClearCache();
        if(_i < 0 || _i >= nodes_.size()) return;
        std::list<std::string>::iterator i = nodes_.begin();
        advance(i, _i);
        nodes_.erase(i);
    }
    void Insert(const std::string& _s, size_t _i)
    {
		ClearCache();
        if(_i < 0 || _i >= nodes_.size()) return;
        std::list<std::string>::iterator i = nodes_.begin();
        advance(i, _i);
        nodes_.insert(i, _s);
    }
};

#endif
