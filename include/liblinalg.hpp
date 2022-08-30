#ifndef LIBLINALG_H
#define LIBLINALG_H

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

            Matrix result = Matrix<T, rows, m_columns> ();
            
            for (size_t i = 0; i < m_columns; i++) {
                for (size_t y = 0; y < rows; y++) {
                    T value = 0;
                    for (size_t x = 0; x < columns; x++) {
                        value += data[y][x] * m.data[x][i];
                    }
                    result.data[y][i] = value;
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
                T value = 0;
                for (size_t x = 0; x < columns; x++) {
                    value += data[y][x] * v.at(x);
                }
                result[y] = value;
            }

            return result;
        }

        // https://www.codesansar.com/numerical-methods/matrix-inverse-using-gauss-jordan-cpp-output.htm
        Matrix inverse ()
        {
            matrix a {data};
            int i, j, k, ratio, n = rows;
            /* Augmenting Identity Matrix of Order n */
            for(i=1;i<=n;i++)
            {
                for(j=1;j<=n;j++)
                {
                    if(i==j)
                    {
                        a[i][j+n] = 1;
                    }
                    else
                    {
                        a[i][j+n] = 0;
                    }
                }
            }

            /* Applying Gauss Jordan Elimination */
            for(i=1;i<=n;i++)
            {
                if(a[i][i] == 0.0)
                {
                    exit(0);
                }
                for(j=1;j<=n;j++)
                {
                    if(i!=j)
                    {
                        ratio = a[j][i]/a[i][i];
                        for(k=1;k<=2*n;k++)
                        {
                            a[j][k] = a[j][k] - ratio*a[i][k];
                        }
                    }
                }
            }
            /* Row Operation to Make Principal Diagonal to 1 */
            for(i=1;i<=n;i++)
            {
                for(j=n+1;j<=2*n;j++)
                {
                    a[i][j] = a[i][j]/a[i][i];
                }
            }
            return Matrix {a};
        }
};

std::vector<float> substract (const std::vector<float> v0, const std::vector<float> v1);

std::vector<float> divide (const std::vector<float> v0, const std::vector<float> v1);

std::vector<float> cross (const std::vector<float> v0, const std::vector<float> v1);

std::vector<float> normalize (const std::vector<float> v);

float dot (const std::vector<float> v0, const std::vector<float> v1);

std::vector<float> negateV (const std::vector<float> v);

#endif
