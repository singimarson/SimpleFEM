#include "FESquareMatrix.h"

// Constructor for square matrix with zeros
FESquareMatrix::FESquareMatrix(const int& iSize) : FEMatrix(iSize, iSize)
{
}

// Constructor for unit matrix
FESquareMatrix::FESquareMatrix(const int& iSize, const int& iDiag) : FESquareMatrix(iSize)
{
	for (int iDiagIter = 0; iDiagIter < iSize; ++iDiagIter)
	{
		m_vMatrixElements[(m_iRowNumber + 1) * iDiagIter] = iDiag;
	}
}

// Constructor with vectors
FESquareMatrix::FESquareMatrix(const std::vector<std::vector<double>>& matrix) : FEMatrix(matrix)
{
}
