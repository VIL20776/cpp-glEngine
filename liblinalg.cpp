#include <cstddef>
#include <stdio.h>
#include <stdexcept>
#include <vector>

template<class T>
class Matrix {
    using matrix = std::vector<std::vector<T>>;

    private:
        const size_t rows;
        const size_t columns;
        matrix data;

    public:
        Matrix(size_t rows, size_t columns, bool identity=false)
        {
            this->rows = rows;
            this->columns = columns;

            for (int i = 0; i < rows; i++) {
                std::vector<T> row (columns, 0);
                data.push_back(row);
            }

            if (identity) {
                for (int i = 0; i < rows; i++) {
                    data[i][i] = 1;
                }
            }
        }

        Matrix(size_t rows, size_t columns, matrix data)
        {
            this->rows = rows;
            this->columns = columns;
            this->data = data;
        }

        Matrix operator * (Matrix<T> const &m)
        {
            if (columns != m.rows) {
                throw std::invalid_argument("Size of rows from m does not match the matrix number of columns.");
            }

            matrix result = Matrix<T> (rows, m.columns);
            
            for (size_t i = 0; i < m.columns; i++) {
                for (size_t y = 0; y < rows; y++) {
                    T value;
                    for (size_t x = 0; y < columns; x++) {
                        value += data[y][x] * m.data[x][i];
                    }
                    result[y][i] = value;
                }
            }

            return result;
        }

        std::vector<T> operator * (std::vector<T> const &v)
        {
            if (columns != v.size()) {
                throw std::invalid_argument("Size of v does not match the matrix number of columns.");
            }

            std::vector<T> result;
            
            for (auto &row: data) {
                T value;
                for (size_t i = 0; i < columns; i++) {
                    value += row.at(i) * v.at(i);
                }
                result.push_back(value);
            }

            return result;
        }


        size_t getColumns () {
            return columns;
        }
};

template<class T>
class Vector{
    private:
    std::vector<T> v;

    public:
    std::vector<T> operator - (std::vector<T> v)
    {
        std::vector<T> result{};
        for (int i = 0; i < v.size(); i++) {
            result.push_back(this->v.at(i) - v.at(i));
        }
        return result;
    }
};
