#ifndef PATHBUILDER_DEFS_H
#define PATHBUILDER_DEFS_H

#include <memory>

namespace pbuilder {

    const int INF = 1000 * 1000 * 1000;

    template <typename T>
    using ShPtr = std::shared_ptr<T>;

    const int PATH_BUILDER_MAX_BLOCK_SIZE = 10;
}

#endif //PATHBUILDER_DEFS_H
