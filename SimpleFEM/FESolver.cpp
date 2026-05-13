#include "FESolver.h"

#include <stdexcept>
#include <tuple>

namespace FESolver
{

// Anonymous namespace for helper functions
namespace
{

// Calculate the LU decomposition of A
// Function might be moved to the FESquare matrix class later if it needs to live elsewhere
std::pair<FESquareMatrix, FESquareMatrix> CalculateLU(const FESquareMatrix& A)
{
	int iMatrixSize = A.GetRowSize();
	FESquareMatrix L(iMatrixSize, 1);
	FESquareMatrix U = A;

	FEVector row0 = U.GetRow(0);
	
	for (int iRowIter = 0; iRowIter < iMatrixSize; ++iRowIter)
	{
		FEVector row = U.GetRow(iRowIter);
		double dDiagCoeff = row[iRowIter];

		FEVector lRow = L.GetRow(iRowIter);

		if (dDiagCoeff == 0)
		{
			throw std::runtime_error("FESpace::CreateLinearElements: Number nodes in this element is incorrect.");
		}

		for (int iRowEditIter = iRowIter + 1; iRowEditIter < iMatrixSize; ++iRowEditIter)
		{
			FEVector editVector = U.GetRow(iRowEditIter);
			double dEditVectorCoeff = editVector[iRowIter];

			// The math here should set the subdiagonal to zero
			editVector = editVector - dEditVectorCoeff / dDiagCoeff * row;
			U.SetRow(iRowEditIter, editVector);

			L.Set(iRowEditIter, iRowIter, dEditVectorCoeff / dDiagCoeff);
		}
	}

	return std::pair<FESquareMatrix, FESquareMatrix>(L, U);
}

// Solves linear system with LU decomposition
FEVector LUSolver(const FESquareMatrix& A, const FEVector& b)
{
	FESquareMatrix L, U;
	std::tie(L, U) = CalculateLU(A);

	const int iMatrixSize = A.GetRowSize();
	FEVector x_l(iMatrixSize), x(iMatrixSize);

	// Solve the L matrix
	for (int iRowIter = 0; iRowIter < iMatrixSize; ++iRowIter)
	{
		double dTemp = b[iRowIter];
		for (int iColIter = 0; iColIter < iRowIter; ++iColIter)
		{
			dTemp -= L.Get(iRowIter, iColIter) * x_l[iColIter];
		}
		x_l.Set(iRowIter, dTemp);
	}

	// For the U matrix, we start at the bottom of the matrix and work our way up
	for (int iRowIter = iMatrixSize - 1; iRowIter >= 0; --iRowIter)
	{
		double dTemp = x_l[iRowIter];
		for (int iColIter = iRowIter + 1; iColIter < iMatrixSize; ++iColIter)
		{
			dTemp -= U.Get(iRowIter, iColIter) * x[iColIter];
		}
		dTemp /= U.Get(iRowIter, iRowIter);
		x.Set(iRowIter, dTemp);
	}

	return x;
}
} // namespace


FEVector SolveSystemDirect(const FESquareMatrix& A, const FEVector& b, const DirectSolverMethod& eMethod)
{
	switch (eMethod)
	{
	case DirectSolverMethod::eLUDecomposition:
	{
		return LUSolver(A, b);
	}
	}
	return FEVector();
}


} //namespace FESolver
