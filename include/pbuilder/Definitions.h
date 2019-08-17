#ifndef PATHBUILDER_DEFS_H
#define PATHBUILDER_DEFS_H

#include <memory>
#include <vector>
#include <string>

namespace pbuilder {

    const int INF = 1000 * 1000 * 1000;

    template<typename T>
    using ShPtr = std::shared_ptr<T>;

    const int PATH_BUILDER_MAX_BLOCK_SIZE = 10;
    const int DAYS_IN_WEEK = 7;
    const std::vector<std::string> DAYS_OF_WEEK_STR =
            {"monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday"};
}

#endif //PATHBUILDER_DEFS_H
