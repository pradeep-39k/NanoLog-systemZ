#pragma once

#include <string>
#include <vector>

#include <json/value.h>

namespace wallsoft{

class Parameter{

public:

    int i;
    int64_t l;
    bool b;
    std::string s;
    std::vector<int> v;

    void initialize(Json::Value value);
    Json::Value getJsonValue();

};


}