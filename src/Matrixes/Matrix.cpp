#include <Matrix.hpp>
#include <CoordinateVector.hpp>
#include <MatrixDynamicStorage.hpp>
#include <MatrixStaticStorage.hpp>

#pragma region CONSTRUCTION

Matrix::Matrix(MatrixBaseStorage *p_storage)
    : storage(p_storage) {}

Matrix::~Matrix()
{
    delete storage;
}

// TODO: FIX STRORAGE ALLOCATION
Matrix::Matrix(const Matrix &m)
{
    storage = new MatrixDynamicStorage(m.getRows(), m.getCols());

    for (int i = 0; i < getRows(); ++i)
    {
        for (int j = 0; j < getCols(); ++j)
            storage->get(i, j) = m.storage->get(i, j);
    }
}

#pragma endregion CONSTRUCTION

#pragma region OPERATORS

Matrix &Matrix::operator=(const Matrix &m)
{
    if (this == &m)
        return *this;

    if (storage == nullptr || getRows() != m.getRows() || getCols() != m.getCols())
        storage = new MatrixDynamicStorage(m.getRows(), m.getCols());

    for (int i = 0; i < getRows(); ++i)
    {
        for (int j = 0; j < getCols(); ++j)
            storage->get(i, j) = m.storage->get(i, j);
    }

    return *this;
}

Matrix &Matrix::operator+=(const Matrix &m)
{
    if (getRows() != m.getRows() || getCols() != m.getCols())
        throw std::logic_error("Could not execute += operator");

    for (int i = 0; i < getRows(); ++i)
    {
        for (int j = 0; j < getCols(); ++j)
            storage->get(i, j) += m.storage->get(i, j);
    }

    return *this;
}

Matrix &Matrix::operator-=(const Matrix &m)
{
    if (getRows() != m.getRows() || getCols() != m.getCols())
        throw std::logic_error("Could not execute -= operator");

    for (int i = 0; i < getRows(); ++i)
    {
        for (int j = 0; j < getCols(); ++j)
            storage->get(i, j) -= m.storage->get(i, j);
    }

    return *this;
}

Matrix &Matrix::operator*=(const Matrix &m)
{
    if (getCols() != m.getRows())
        throw std::logic_error("Could not execute vector multiply");

    auto storage = new MatrixDynamicStorage(getRows(), m.getCols());
    auto temp = Matrix(storage);

    for (int i = 0; i < temp.getRows(); ++i)
    {
        for (int j = 0; j < temp.getCols(); ++j)
        {
            for (int k = 0; k < getCols(); ++k)
                temp.storage->get(i, j) += (storage->get(i, k) * m.storage->get(k, j));
        }
    }

    return (*this = temp);
}

Matrix &Matrix::operator*=(const double v)
{
    for (int i = 0; i < getRows(); ++i)
    {
        for (int j = 0; j < getCols(); ++j)
            storage->get(i, j) *= v;
    }

    return *this;
}

Matrix &Matrix::operator/=(const double v)
{
    for (int i = 0; i < getRows(); ++i)
    {
        for (int j = 0; j < getCols(); ++j)
            storage->get(i, j) /= v;
    }

    return *this;
}

Matrix::operator CoordinateVector() const
{
    return CoordinateVector(storage->get(0, 0), storage->get(0, 1), storage->get(0, 2));
}

#pragma endregion OPERATORS

#pragma region FUNCTIONS

const double Matrix::getCols() const
{
    return storage->cols;
}

const double Matrix::getRows() const
{
    return storage->rows;
}

double &Matrix::getValue(const int i, const int j) const
{
    return storage->get(i, j);
}

#pragma endregion FUNCTIONS

#pragma region DUAL_OPERATORS

Matrix
operator+(const Matrix &m1, const Matrix &m2)
{
    Matrix temp(m1);
    return (temp += m2);
}

Matrix operator-(const Matrix &m1, const Matrix &m2)
{
    Matrix temp(m1);
    return (temp -= m2);
}

Matrix operator*(const Matrix &m1, const Matrix &m2)
{
    Matrix temp(m1);
    return (temp *= m2);
}

Matrix operator*(const Matrix &m, double v)
{
    Matrix temp(m);
    return (temp *= v);
}

Matrix operator*(double v, const Matrix &m)
{
    return (m * v);
}

Matrix operator/(const Matrix &m, double v)
{
    Matrix temp(m);
    return (temp /= v);
}

#pragma endregion DUAL_OPERATORS
