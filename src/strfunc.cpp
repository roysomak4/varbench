#include <string>
#include <vector>
#include <set>
#include "strfunc.h"

std::vector<std::string> split(const std::string &input_str, const char &delim)
{
    std::vector<std::string> result;
    size_t current, previous = 0;
    current = input_str.find_first_of(delim);
    while (current != std::string::npos)
    {
        result.push_back(
            input_str.substr(previous, current - previous));
        previous = current + 1;
        current = input_str.find_first_of(delim, previous);
    }
    result.push_back(
        input_str.substr(previous, current - previous));
    return result;
}

std::string &join(const std::vector<std::string> &sequence, const std::string &delim, std::string &result)
{
    for (auto ele : sequence)
    {
        result += ele;
        result += delim;
    }
    result.erase(result.size() - 1);
    return result;
}

std::string &join(const std::set<std::string> &sequence, const std::string &delim, std::string &result)
{
    for (auto ele : sequence)
    {
        result += ele;
        result += delim;
    }
    result.erase(result.size() - 1);
    return result;
}

std::vector<std::string> slice(const std::vector<std::string> &v, const int start, const int end)
{
  auto s = v.begin() + start;
  auto e = v.begin() + end + 1;
  
  std::vector<std::string> retvec(s, e);
  return retvec; 
}

// trim functions for std::string
// source: http://www.martinbroadhurst.com/how-to-trim-a-stdstring.html
std::string& ltrim(std::string& str, const std::string &delim)
{
    str.erase(0, str.find_first_not_of(delim));
    return str;
}
 
std::string& rtrim(std::string& str, const std::string &delim)
{
    str.erase(str.find_last_not_of(delim) + 1);
    return str;
}
 
std::string& trim(std::string& str, const std::string &delim)
{
    return ltrim(rtrim(str, delim), delim);
}
