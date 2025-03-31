#pragma once

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <random>
#include <cassert>
#include <type_traits>

#include "Vector.hpp"

template <typename T, size_t N, size_t M>
requires std::is_arithmetic_v<T>
class Matrix
{
public:
	constexpr Matrix();
	constexpr explicit Matrix(const T& data);
	template <typename Distribution, typename Engine>
	explicit Matrix(Distribution& dist, Engine& engine); // Cannot be constexpr since random gen are runtime-dependent
	constexpr Matrix(std::initializer_list<Vector<T, M>> l);
	constexpr Matrix(const Matrix& other);
	constexpr ~Matrix();

	constexpr Matrix& operator= (const Matrix& other);
	constexpr Matrix& operator= (const T& val);
	constexpr Vector<T, M>& operator[] (size_t rowIndex) noexcept;
	constexpr T& operator() (size_t rowIndex, size_t colIndex) noexcept;
	constexpr void modify(const std::function<T(T&)>& f);
	constexpr void fill(const T& val);

	constexpr const Vector<T, M>& operator[] (size_t rowIndex) const noexcept;
	constexpr const T& operator() (size_t rowIndex, size_t colIndex) const noexcept;
	constexpr size_t rowSize() const noexcept;
	constexpr size_t colSize() const noexcept;
	[[nodiscard]] constexpr Matrix operator-() const;
	[[nodiscard]] constexpr Matrix transpose() const;
	void print() const;

private:
	Vector<Vector<T, M>, N> m_matrix;
};

//---------------Implementation------------------

template <typename T, size_t N, size_t M>
requires std::is_arithmetic_v<T>
constexpr Matrix<T, N, M>::Matrix() : m_matrix{}
{}

template <typename T, size_t N, size_t M>
requires std::is_arithmetic_v<T>
constexpr Matrix<T, N, M>::Matrix(const T& data)  : m_matrix{}
{
	for (size_t i = 0; i < N; ++i)
		for (size_t j = 0; j < M; ++j)
			m_matrix[i][j] = data;
}

template <typename T, size_t N, size_t M>
requires std::is_arithmetic_v<T>
template <typename Distribution, typename Engine>
Matrix<T, N, M>::Matrix(Distribution& dist, Engine& engine)
{
	for (size_t i = 0; i < N; ++i)
		for (size_t j = 0; j < M; ++j)
			m_matrix[i][j] = dist(engine);
}

template <typename T, size_t N, size_t M>
requires std::is_arithmetic_v<T>
constexpr Matrix<T, N, M>::Matrix(std::initializer_list<Vector<T, M>> l) : m_matrix{}
{
	auto it = l.begin();
	for (size_t i = 0; i < N; ++i, ++it)
		m_matrix[i] = std::move(*it);
}

template <typename T, size_t N, size_t M>
requires std::is_arithmetic_v<T>
constexpr Matrix<T, N, M>::Matrix(const Matrix& other) : m_matrix{}
{
	for (size_t i = 0; i < N; ++i)
		for (size_t j = 0; j < M; ++j)
			m_matrix[i][j] = other.m_matrix[i][j];
}

template <typename T, size_t N, size_t M>
requires std::is_arithmetic_v<T>
constexpr Matrix<T, N, M>::~Matrix() {}

template <typename T, size_t N, size_t M>
requires std::is_arithmetic_v<T>
constexpr Vector <T, M> & Matrix<T, N, M>::operator[] (size_t rowIndex) noexcept
{
	assert(rowIndex < M && "Index out-of-bounds");
	return m_matrix[rowIndex];
}

template <typename T, size_t N, size_t M>
requires std::is_arithmetic_v<T>
constexpr const Vector<T, M>& Matrix<T, N, M>::operator[] (size_t rowIndex) const noexcept
{
	assert(rowIndex < M && "Index out-of-bounds");
	return m_matrix[rowIndex];
}

template <typename T, size_t N, size_t M>
requires std::is_arithmetic_v<T>
constexpr T& Matrix<T, N, M>::operator() (size_t rowIndex, size_t colIndex) noexcept
{
	assert(rowIndex < M && "Row index out-of-bounds");
	assert(colIndex < N && "Column index out-of-bounds");
	return m_matrix[rowIndex][colIndex];
}

template <typename T, size_t N, size_t M>
requires std::is_arithmetic_v<T>
constexpr const T& Matrix<T, N, M>::operator() (size_t rowIndex, size_t colIndex) const noexcept
{
	assert(rowIndex < M && "Row index out-of-bounds");
	assert(colIndex < N && "Column index out-of-bounds");
	return m_matrix[rowIndex][colIndex];
}

template <typename T, size_t N, size_t M>
requires std::is_arithmetic_v<T>
constexpr size_t Matrix<T, N, M>::rowSize() const noexcept
{
	return M;
}

template <typename T, size_t N, size_t M>
requires std::is_arithmetic_v<T>
constexpr size_t Matrix<T, N, M>::colSize() const noexcept
{
	return N;
}

template <typename T, size_t N, size_t M>
requires std::is_arithmetic_v<T>
constexpr Matrix<T, N, M>& Matrix<T, N, M>::operator= (const Matrix& other)
{
	if (this == &other)
		return *this;

	for (size_t i = 0; i < N; ++i)
		m_matrix[i] = other.m_matrix[i];
	return *this;
}

template <typename T, size_t N, size_t M>
requires std::is_arithmetic_v<T>
constexpr Matrix<T, N, M>& Matrix<T, N, M>::operator= (const T& val)
{
	fill(val);
	return *this;
}

template <typename T, size_t N, size_t M>
requires std::is_arithmetic_v<T>
constexpr Matrix<T, N, M> Matrix<T, N, M>::operator-() const
{
	Matrix temp;
	for (size_t i = 0; i < N; ++i)
		temp[i] = -(this->operator[](i));
	return temp;
}

template <typename T, size_t N, size_t M>
requires std::is_arithmetic_v<T>
constexpr Matrix<T, N, M> Matrix<T, N, M>::transpose() const
{
	Matrix temp;
	for (size_t i = 0; i < N; ++i)
		for(size_t j = 0; j < M; ++j)
			temp[i][j] = m_matrix[j][i];
	return temp;
}

template <typename T, size_t N, size_t M>
requires std::is_arithmetic_v<T>
constexpr void Matrix<T, N, M>::modify(const std::function<T(T&)>& f)
{
	for (size_t i = 0; i < N; ++i)
		this->operator[](i).modify(f);
}

template <typename T, size_t N, size_t M>
requires std::is_arithmetic_v<T>
constexpr void Matrix<T, N, M>::fill(const T& val)
{
	for (auto& vec : m_matrix)
		vec = val;
}

template <typename T, size_t N, size_t M>
requires std::is_arithmetic_v<T>
void Matrix<T, N, M>::print() const
{
	for (size_t i = 0; i < N; ++i)
	{
		for (size_t j = 0; j < M; ++j)
			std::cout << m_matrix[i][j] << " ";
		std::cout << std::endl;
	}
}

// Non-member functions

template <typename T, size_t N, size_t M>
constexpr Matrix<T, N, M> operator+(const Matrix<T,N,M>& A, const Matrix<T,N,M>& B)
{
	Matrix<T,N,M> temp;
	for (size_t i = 0; i < N; ++i)
		temp[i] = A[i] + B[i];
	return temp;
}

template <typename T, size_t N, size_t M>
constexpr Matrix<T, N, M> operator-(const Matrix<T,N,M>& A, const Matrix<T,N,M>& B)
{
	Matrix<T,N,M> temp;
	for (size_t i = 0; i < N; ++i)
		temp[i] = A[i] - B[i];
	return temp;
}

template <typename T, typename F, size_t N, size_t M>
constexpr Matrix<F, N, M> operator*(const Matrix<T, N, M>& matrix, const F& scalar)
{
	Matrix<F, N, M> temp;
	for (size_t i = 0; i < N; ++i)
		for (size_t j = 0; j < N; ++j)
			temp[i][j] = scalar * matrix[i][j];
	return temp;
}

template <typename T, typename F, size_t N, size_t M>
constexpr Matrix<F, N, M> operator*(const F& scalar, const Matrix<T, N, M>& matrix)
{
	Matrix<F, N, M> temp;
	for (size_t i = 0; i < N; ++i)
		for (size_t j = 0; j < N; ++j)
			temp[i][j] = scalar * matrix[i][j];
	return temp;
}

template <typename T, size_t N>
requires std::is_arithmetic_v<T>
constexpr Matrix<T,N,N> matrix_mult(const Matrix<T,N,N>& A, const Matrix<T,N,N>& B)
{
    Matrix<T, N, N> C;
    // Compute the transpose of B to make accessing columns easier
    auto B_T = B.transpose(); // B_T is a Matrix<T, P, M>
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            // Dot product of row i of A and row j of B_T (which is column j of B)
            C[i][j] = A[i].dot_product(B_T[j]);
        }
    }
    return C;
}