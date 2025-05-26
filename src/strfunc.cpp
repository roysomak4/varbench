#include <string>
#include <vector>
#include <string_view>
#include <set>
#include "strfunc.h"

// std::vector<std::string> split(const std::string &input_str, const char &delim)
// {
//     std::vector<std::string> result;
//     size_t current, previous = 0;
//     current = input_str.find_first_of(delim);
//     while (current != std::string::npos)
//     {
//         result.push_back(
//             input_str.substr(previous, current - previous));
//         previous = current + 1;
//         current = input_str.find_first_of(delim, previous);
//     }
//     result.push_back(
//         input_str.substr(previous, current - previous));
//     return result;
// }

std::vector<std::string> split(std::string_view input_str, const char delim) {
    std::vector<std::string> result;
    if (input_str.empty()) {
        return result; // Return empty vector if input string is empty
    }

    size_t start = 0;
    size_t end = input_str.find(delim);
    while (end != std::string_view::npos) {
        result.emplace_back(input_str.substr(start, end - start));
        start = end + 1;
        end = input_str.find(delim, start);
    }
    result.emplace_back(input_str.substr(start, end - start));
    return result;
}

std::string join(const std::vector<std::string> &sequence, const std::string &delim)
{
    if (sequence.empty()) {
        return ""; // Return empty string if sequence is empty
    }
    
    // Estimate the size of the result and reserve space in memory
    size_t total_length = 0;
    for (const auto& ele : sequence) {
        total_length += ele.size() + delim.size();
    }
    total_length -= delim.size(); // Remove the last delimiter size

    std::string result;
    result.reserve(total_length); // Reserve space for the result string

    // Append elements with delimiter in between only
    auto it = sequence.begin();
    result += *it++; // Add the first element
    for (; it != sequence.end(); ++it)
    {
        result += delim;
        result += *it;
    }
    
    return result;
}

std::string join(const std::set<std::string> &sequence, const std::string &delim)
{
    if (sequence.empty()) {
        return ""; // Return empty string if sequence is empty
    }
    // estimate the size of the result and reserve space in memory
    size_t total_length = 0;
    for (const auto& ele : sequence) {
        total_length += ele.size() + delim.size();
    }
    total_length -= delim.size(); // Remove the last delimiter size

    // Reserve space in memory for the result string
    std::string result;
    result.reserve(total_length); // Reserve space for the result string

    // Append elements with delimiter in between
    auto it = sequence.begin();
    result += *it++; // Add the first element
    for (; it != sequence.end(); ++it)
    {
        result += delim;
        result += *it;
    }
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
std::string& ltrim(std::string& str, const std::string &delim = " \t\n\r")
{
    if (!str.empty()) {
        size_t start = str.find_first_not_of(delim);
        if (start != std::string::npos) {
            str.erase(0, start);
        } else {
            str.clear(); // If no non-delimiter characters found, clear the string
        }
    }
    return str;
}
 
std::string& rtrim(std::string& str, const std::string &delim = " \t\n\r")
{
    if(!str.empty()) {
        size_t end = str.find_last_not_of(delim);
        if (end != std::string::npos) {
            str.erase(end + 1);
        } else {
            str.clear(); // If no non-delimiter characters found, clear the string
        }
    }
    return str;
}
 
std::string& trim(std::string& str, const std::string &delim = " \t\n\r")
{
    return ltrim(rtrim(str, delim), delim);
}
