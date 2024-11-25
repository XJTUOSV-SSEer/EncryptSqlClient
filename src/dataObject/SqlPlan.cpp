//
// Created by Zhiti on 2024/11/16.
//

#include "SqlPlan.h"

#include <utility>

SqlPlan::SqlPlan(string type, vector<string> params) {
    this->type = std::move(type);
    this->params = std::move(params);
}

string SqlPlan::getType() {
    return type;
}
vector<string> SqlPlan::getParams() {
    return params;
}




