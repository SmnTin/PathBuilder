#include "pbuilder/Structures/Mat.h"

namespace pbuilder {
    MatOutOfBoundsException::MatOutOfBoundsException(size_t rows, size_t cols, size_t i, size_t j) {
        std::stringstream ss;
        ss << "Accessing mat with size " << rows << "x" << cols << " at (" << i << ", " << j
           << " that is out of its bounds.";
        ss >> _what;
    }

    const char *MatOutOfBoundsException::what() const noexcept {
        return _what.c_str();
    }
}