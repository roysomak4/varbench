#pragma once

std::vector<std::string> split(const std::string_view input_str, const char delim);

std::string join(const std::vector<std::string> &sequence, const std::string &delim);

std::string join(const std::set<std::string> &sequence, const std::string &delim);

std::vector<std::string> slice(const std::vector<std::string> &v, const int start, const int end);

std::string& ltrim(std::string& str, const std::string &delim);
 
std::string& rtrim(std::string& str, const std::string &delim);
 
std::string& trim(std::string& str, const std::string &delim);
