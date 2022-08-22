#include <cmath>
#include <cstddef>
#include <stdio.h>
#include <stdexcept>
#include <vector>
#include <array>

template<class T, size_t rows, size_t columns>
class Matrix {
    using matrix = std::array<std::array<T, columns>, rows>;

    private:
        matrix data;

    public:
        Matrix(bool identity=false)
        {
            for (auto &row: data) {
                row.fill(0);
            }

            if (identity) {
                for (int i = 0; i < rows; i++) {
                    data[i][i] = 1;
                }
            }
        }

        Matrix(matrix data)
        {
            this->data = data;
        }

        template<size_t m_rows, size_t m_columns>
        Matrix operator * (Matrix<T, m_rows, m_columns> const &m)
        {
            if (columns != m_rows) {
                throw std::invalid_argument("Size of rows from m does not match the matrix number of columns.");
            }

            matrix result = Matrix<T, rows, m_columns> ();
            
            for (size_t i = 0; i < m_columns; i++) {
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

        template<size_t size>
        std::array<T, size> operator * (std::array<T, size> const &v)
        {
            if (columns != v.size()) {
                throw std::invalid_argument("Size of v does not match the matrix number of columns.");
            }

            std::array<T, size> result {};
            
            for (size_t y = 0; y < rows; y++) {
                T value;
                for (size_t x = 0; x < columns; x++) {
                    value += data[y][x] * v.at(x);
                }
                result[y] = value;
            }

            return result;
        }
};

template<class T>
std::vector<T> substract (const std::vector<T> v0, const std::vector<T> v1)
{
    std::vector<T> result {};
    for (int i = 0; i < v0.size(); i++) {
        result.push_back(v0.at(i) - v1.at(i));
    }
    return result;
}

template<class T>
std::vector<T> divide (const std::vector<T> v0, const std::vector<T> v1)
{
    std::vector<T> result {};
    for (int i = 0; i < v0.size(); i++) {
        result.push_back(v0.at(i) / v1.at(i));
    }
    return result;
}

template<class T>
std::vector<T> cross (const std::vector<T> v0, const std::vector<T> v1)
{
    T x = (v0.at(1) * v1.at(2)) - (v0.at(2) * v1.at(1));
    T y = (v0.at(0) * v1.at(2)) - (v0.at(2) * v1.at(0));
    T z = (v0.at(0) * v1.at(1)) - (v0.at(1) * v1.at(0));

    return {x, y, z};
}

std::vector<float> normalize (const std::vector<float> v)
{
    float magnitude {};
    for (auto &i: v)
    {
        magnitude += std::pow(i, 2);
    }
    
    std::vector<float> result {};
    for (auto &i : v) {
        result.push_back(i / magnitude);
    }

    return result;
}