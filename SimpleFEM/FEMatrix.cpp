#include "FEMatrix.h"

#include <stdexcept>

// Constructor for full matrix
FEMatrix::FEMatrix(const int& iRowNum, const int& iColNum) : m_iRowNumber(iRowNum), m_iColumnNumber(iColNum),
	m_vMatrixElements(std::vector<std::vector<double>>(iRowNum, std::vector<double>(iColNum)))
{}

// Constructor for vector of vectors
FEMatrix::FEMatrix(const std::vector<std::vector<double>>& matrix) 
{
	// Need to verify that elements of the matrix are valid
	std::size_t row1Size = matrix.front().size();
	for (int iRowIter = 0; iRowIter < matrix.size(); ++iRowIter)
	{
		if (matrix[iRowIter].size() != row1Size)
		{
			throw std::runtime_error("FEMatrix::FEMatrix: The row sizes for a matrix need to be consistent.");
		}
	}

	m_iRowNumber = static_cast<int>(matrix.size());
	m_iColumnNumber = static_cast<int>(row1Size);
	m_vMatrixElements = matrix;
}

// Gets specific column of matrix
FEVector FEMatrix::GetColumn(const int& iColumn)
{
	if (iColumn >= m_iColumnNumber)
	{
		throw std::runtime_error("FEMatrix::GetColumn: Given rhsColumn is larger than the number of columns in this matrix.");
	}

	std::vector<double> column;
	for (const std::vector<double>& row : m_vMatrixElements)
	{
		column.push_back(row[iColumn]);
	}

	return FEVector(column);
}

// Addition operator overload
FEMatrix FEMatrix::operator+(FEMatrix& otherMatrix)
{
	FEMatrix MatrixSum = FEMatrix(m_iRowNumber, m_iColumnNumber);
	
	for (int iRow = 0; iRow < m_iRowNumber; ++iRow)
	{
		for (int iColumn = 0; iColumn < m_iColumnNumber; ++iColumn)
		{
			if (Get(iRow, iColumn) == 0.0 && otherMatrix.Get(iRow, iColumn) == 0.0)
				continue;

			MatrixSum.Set(iRow, iColumn, Get(iRow, iColumn) + otherMatrix.Get(iRow, iColumn));
		}
	}

	return MatrixSum;
}

// Subtraction operator overload
FEMatrix FEMatrix::operator-(FEMatrix& otherMatrix)
{
	FEMatrix MatrixDiff = FEMatrix(m_iRowNumber, m_iColumnNumber);

	for (int iRow = 0; iRow < m_iRowNumber; ++iRow)
	{
		for (int iColumn = 0; iColumn < m_iColumnNumber; ++iColumn)
		{
			if (Get(iRow, iColumn) == 0.0 && otherMatrix.Get(iRow, iColumn) == 0.0)
				continue;

			MatrixDiff.Set(iRow, iColumn, Get(iRow, iColumn) - otherMatrix.Get(iRow, iColumn));
		}
	}

	return MatrixDiff;
}

// Scalar multiplication overload
FEMatrix FEMatrix::operator*(double& scalar)
{
	FEMatrix MatrixMult = FEMatrix(m_iRowNumber, m_iColumnNumber);

	for (int iRow = 0; iRow < m_iRowNumber; ++iRow)
	{
		for (int iColumn = 0; iColumn < m_iColumnNumber; ++iColumn)
		{
			if (Get(iRow, iColumn) == 0.0)
				continue;

			MatrixMult.Set(iRow, iColumn, scalar * Get(iRow, iColumn));
		}
	}

	return MatrixMult;
}

// Operator overload for matrix-vector multiplication
FEVector FEMatrix::operator*(FEVector& feVector)
{
	std::vector<double> product;
	for (const std::vector<double>& matrixRow : m_vMatrixElements)
	{
		FEVector rowVector(matrixRow);
		product.push_back(feVector.DotProduct(rowVector));
	}

	return FEVector(product);
}

// Operator overload for matrix-matrix multiplication
FEMatrix FEMatrix::operator*(FEMatrix& otherMatrix)
{
	if (m_iColumnNumber != otherMatrix.GetRowSize())
	{
		throw std::runtime_error("FEMatrix::operator*: Matrices are incompatible sizes.");
	}
	
	std::vector<std::vector<double>> matrix;
	for (int iRowIter = 0; iRowIter < m_iRowNumber; ++iRowIter)
	{
		std::vector<double> multRow;
		FEVector lhsRow = GetRow(iRowIter);
		for (int iColumnIter = 0; iColumnIter < otherMatrix.GetColumnSize(); ++iColumnIter)
		{
			FEVector rhsColumn = otherMatrix.GetColumn(iColumnIter);
			multRow.push_back(lhsRow.DotProduct(rhsColumn));
		}

		matrix.push_back(multRow);
	}
	
	return FEMatrix(matrix);
}


