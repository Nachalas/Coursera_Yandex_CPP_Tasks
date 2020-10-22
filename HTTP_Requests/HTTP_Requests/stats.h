#pragma once

#include "http_request.h"

#include <string_view>
#include <map>
#include <vector>
#include <set>
using namespace std;

const set<string_view> knownMethods = { "GET", "POST", "PUT", "DELETE" };
const set<string_view> knownUris = { "/", "/order", "/product", "/basket", "/help" };
const string unknownM = "UNKNOWN";
const string unknownU = "unknown";

class Stats {
public:
    Stats();
    void AddMethod(string_view method);
    void AddUri(string_view uri);
    const map<string_view, int>& GetMethodStats() const;
    const map<string_view, int>& GetUriStats() const;

private:
    map<string_view, int> methodStats;
    map<string_view, int> uriStats;

    string_view ParseMethod(string_view input) const;

    string_view ParseUri(string_view input) const;
};

HttpRequest ParseRequest(string_view line);