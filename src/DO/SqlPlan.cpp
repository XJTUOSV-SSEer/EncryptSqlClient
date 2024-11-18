//
// Created by Zhiti on 2024/11/16.
//

#include "SqlPlan.h"
string SqlPlan::getType() {
    return type;
}
vector<string> SqlPlan::getWhereParams() {
    return whereParams;
}


