#include "FEMatrix.h"

// Constructor for full matrix
FEMatrix::FEMatrix(const int& iRowNum, const int& iColNum) : m_iRowNumber(iRowNum), m_iColumnNumber(iColNum),
	m_vMatrixElements(std::vector<std::vector<double>>(iRowNum, std::vector<double>(iColNum)))
{}

// Addition operator overload
FEMatrix FEMatrix::operator+(FEMatrix& otherMatrix)
{
	FEMatrix MatrixSum = FEMatrix(m_iRowNumber, m_iColumnNumber);
	
	for (int iRow = 0; iRow < m_iRowNumber; ++iRow)
	{
		for (int iColumn = 0; iColumn < m_iColumnNumber; ++iColumn)
		{
			if (this->Get(iRow, iColumn) == 0.0 && otherMatrix.Get(iRow, iColumn) == 0.0)
				continue;

			MatrixSum.Set(iRow, iColumn, this->Get(iRow, iColumn) + otherMatrix.Get(iRow, iColumn));
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
			if (this->Get(iRow, iColumn) == 0.0 && otherMatrix.Get(iRow, iColumn) == 0.0)
				continue;

			MatrixDiff.Set(iRow, iColumn, this->Get(iRow, iColumn) - otherMatrix.Get(iRow, iColumn));
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
			if (this->Get(iRow, iColumn) == 0.0)
				continue;

			MatrixMult.Set(iRow, iColumn, scalar * this->Get(iRow, iColumn));
		}
	}

	return MatrixMult;
}

