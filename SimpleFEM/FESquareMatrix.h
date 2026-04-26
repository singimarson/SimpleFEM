#pragma once
#include "FEMatrix.h"
class FESquareMatrix : public FEMatrix
{
public:
	// Constructors
	FESquareMatrix() = default;
	FESquareMatrix(const int& iSize);
	FESquareMatrix(const int& iSize, const int& iDiag);
	FESquareMatrix(const std::vector<std::vector<double>>& matrix);
};

