#ifndef PATHBUILDER_MAT_H
#define PATHBUILDER_MAT_H

#include <cstddef>
#include <exception>
#include <sstream>
#include <vector>

namespace pbuilder {

    class MatOutOfBoundsException: public std::exception {
    public:
        MatOutOfBoundsException(size_t rows, size_t cols, size_t i, size_t j);

        const char* what() const noexcept override;

    protected:
        std::string _what;
    };

    template<class T>
    class Mat {
    public:
        explicit Mat(size_t rows_ = 0, size_t cols_ = 0) {
            _rows = rows_;
            _cols = cols_;
            _data.assign(_rows, std::vector<T>(_cols));
        }

        explicit Mat(size_t rows_, size_t cols_, const T & defaultValue) : Mat(rows_, cols_) {
            fill(defaultValue);
        }

        size_t rows() {
            return _rows;
        }
        size_t cols() {
            return _cols;
        }

        void fill(const T & value) {
            for(size_t i = 0; i < _rows; ++i)
                for(size_t j = 0; j < _cols; ++j)
                    _data[i][j] = value;
        }

        const T& at(size_t row, size_t col) const {
            if(row >= _rows || col >= _cols)
                throw MatOutOfBoundsException(_rows, _cols, row, col);
            return _data[row][col];
        }

        T& at(size_t row, size_t col) {
            if(row >= _rows || col >= _cols)
                throw MatOutOfBoundsException(_rows, _cols, row, col);
            return _data[row][col];
        }

        static Mat createFromVector(size_t rows_, size_t cols_, const std::vector<T> & vec);

    private:
        size_t _rows, _cols;
        std::vector<std::vector<T>> _data;

    };

    template <class T>
    Mat<T> Mat<T>::createFromVector(size_t rows_, size_t cols_, const std::vector<T> & vec) {
        Mat<T> mat(rows_, cols_);

        for(size_t i = 0; i < rows_; ++i)
            for(size_t j = 0; j < cols_; ++j)
                mat.at(i, j) = vec[i*cols_ + j];

        return mat;
    }

    typedef Mat<int> MatInt;
    typedef Mat<unsigned int> MatUInt;
    typedef Mat<float> MatFloat;
    typedef Mat<double> MatDouble;
}

#endif //PATHBUILDER_MAT_H
