#pragma once

#include "FEVector.h"
#include "FESquareMatrix.h"

namespace FESolver
{
enum class DirectSolverMethod
{
	eLUDecomposition
};

enum class IterativeSolverMethod
{
	eConjugateGradient
};

FEVector SolveSystemDirect(const FESquareMatrix& A, const FEVector& b, const DirectSolverMethod& eMethod);

} // namespace FESolver

