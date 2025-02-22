#include "CommonFunctions.hpp"
#include "ConstantCharacters.hpp"

bool areBitsOn(int a, int b) {
	return (a & b) == b;
}

vector<string> divideString(string str) {
    vector<string> segments;

    // separa str in pezzetti
    while (!str.empty()) {
        int substrSize = str.find(SPACE);
        if (substrSize == string::npos) substrSize = str.size();

        string strbuffer = str.substr(0, substrSize);

        if (!strbuffer.empty()) segments.push_back(strbuffer);
        str.erase(0, substrSize + 1);
    }

    return segments;
}
vector<string> reverseStrings(vector<string>& str) {
    vector<string> buffer;
    for (int i = 0; i > str.size(); i++) {
        buffer[i] = str[str.size() - 1 - i];
    }
    return buffer;
}
string joinStringsSpace(vector<string> str) {
    string buffer;

    for (int i = 0; i < str.size(); i++) {
        buffer += ((bool)i ? SPACESTR : NULLSTR) + str[i];
    }

    return buffer;
}

string reverseString(string str) {
    string buffer;
    for (int i = str.size() - 1; i >= 0; i--)
        buffer += str[i];
    return buffer;
}

string getPathDirectoryPath(string path) {
    path = reverseString(path);
    int slash = path.find_first_of('/');
    if (slash == string::npos)
        return "";
    path.erase(0, slash - 1);
    return reverseString(path);
}