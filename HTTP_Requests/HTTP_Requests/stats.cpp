#include "stats.h"
using namespace std;

Stats::Stats() {
    for (const auto& i : knownMethods) {
        methodStats[i];
    }
    methodStats[unknownM];
    for (const auto& i : knownUris) {
        uriStats[i];
    }
    uriStats[unknownU];
}

void Stats::AddMethod(string_view method) {
    string_view methodParsed = ParseMethod(method);
    methodStats[methodParsed]++;
}

void Stats::AddUri(string_view uri) {
    string_view uriParsed = ParseUri(uri);
    uriStats[uriParsed]++;
}

const map<string_view, int>& Stats::GetMethodStats() const {
    return methodStats;
}
const map<string_view, int>& Stats::GetUriStats() const {
    return uriStats;
}

string_view Stats::ParseMethod(string_view input) const {
    auto res = knownMethods.find(input);
    if (res != knownMethods.end()) {
        return (string_view)*res;
    }
    else {
        return unknownM;
    }
}

string_view Stats::ParseUri(string_view input) const {
    auto res = knownUris.find(input);
    if (res != knownUris.end()) {
        return (string_view)*res;
    }
    else {
        return unknownU;
    }
}

void TrimSV(string_view& input) {
    while (input[0] == ' ') {
        input.remove_prefix(1);
    }
}

HttpRequest ParseRequest(string_view line) {
    TrimSV(line);
    vector<string_view> result;
    result.reserve(3);
    while (true) {
        size_t space = line.find(' ');
        result.push_back(line.substr(0, space));
        if (space == line.npos) {
            break;
        }
        else {
            line.remove_prefix(space + 1);
        }
    }
    return HttpRequest { result[0], result[1], result[2] };
}