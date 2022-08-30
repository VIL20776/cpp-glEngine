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
                for (size_t x = 0; x < columns; x++) {
                    result[y] += data[y][x] * v.at(x);
                }
            }

            return result;
        }

        // https://www.geeksforgeeks.org/finding-inverse-of-a-matrix-using-gauss-jordan-method/
        Matrix inverse ()
        {    
            T temp;

            // Create the augmented matrix
            std::array<std::array<T, 2*columns>, rows> augmented {};
            for (size_t y = 0; y < rows; y++) {
                for (size_t x = 0; x < columns; x++) {
                    augmented[y][x] = data[y][x];
                }
            }      

            // Add the identity matrix
            // of order at the end of original matrix.
            for (int i = 0; i < rows; i++) {
        
                for (int j = 0; j < 2 * rows; j++) {
        
                    // Add '1' at the diagonal places of
                    // the matrix to create a identity matrix
                    if (j == (i + rows))
                        augmented[i][j] = 1;
                }
            }
        
            // Interchange the row of matrix,
            // interchanging of row will start from the last row
            for (int i = rows - 1; i > 0; i--) {
        
                // Swapping each and every element of the two rows
                if (augmented[i - 1][0] < augmented[i][0])
                for (int j = 0; j < 2 * rows; j++) {
                
                       // Swapping of the row, if above
                       // condition satisfied.
                temp = augmented[i][j];
                augmented[i][j] = augmented[i - 1][j];
                augmented[i - 1][j] = temp;
                }
            }
        
            // Replace a row by sum of itself and a
            // constant multiple of another row of the matrix
            for (int i = 0; i < rows; i++) {
        
                for (int j = 0; j < rows; j++) {
        
                    if (j != i) {
        
                        temp = augmented[j][i] / augmented[i][i];
                        for (int k = 0; k < 2 * rows; k++) {
        
                            augmented[j][k] -= augmented[i][k] * temp;
                        }
                    }
                }
            }
        
            // Multiply each row by a nonzero integer.
            // Divide row element by the diagonal element
            for (int i = 0; i < rows; i++) {
        
                temp = augmented[i][i];
                for (int j = 0; j < 2 * rows; j++) {
        
                    augmented[i][j] = augmented[i][j] / temp;
                }
            }

            matrix inverse {};
            for (size_t y = 0; y < rows; y++) {
                for (size_t x = columns; x < 2 * columns; x++) {
                    inverse[y][x - columns] = augmented[y][x];
                }
            }   
        
            return {inverse};
        }
};

std::vector<float> substract (const std::vector<float> v0, const std::vector<float> v1);

std::vector<float> divide (const std::vector<float> v0, const std::vector<float> v1);

std::vector<float> cross (const std::vector<float> v0, const std::vector<float> v1);

std::vector<float> normalize (const std::vector<float> v);

float dot (const std::vector<float> v0, const std::vector<float> v1);

std::vector<float> negateV (const std::vector<float> v);

#endif
