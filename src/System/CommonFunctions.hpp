#pragma once

#include <vector>
#include <string>

using std::vector, std::string;

inline extern bool areBitsOn(int a, int b);

extern vector<string> divideString(string str);
extern vector<string> reverseStrings(vector<string>& str);
extern string joinStringsSpace(vector<string> str);

extern string reverseString(string str);

extern string getPathDirectoryPath(string path);