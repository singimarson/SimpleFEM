#pragma once

#include "FEObject.h"

#include <vector>

class FEMatrix : public FEObject
{
public:
	// Constructors and destructors
	FEMatrix() = default;
	FEMatrix(const int& iRowNum, const int& iColNum);

	~FEMatrix() = default;

	// Object Type
	virtual bool SupportsObjectType(const long objType) override
	{
		return (FEObject::SupportsObjectType(objType) || objType == FE_OBJ_MATRIX);
	}

	// Element accessing functions
	double Get(const int& iRow, const int& iColumn) { return m_vMatrixElements[iRow][iColumn]; }
	void Set(const int& iRow, const int& iColumn, const double& value) { m_vMatrixElements[iRow][iColumn] = value; }

	// Operator overloads
	FEMatrix operator+(FEMatrix& otherMatrix);
	FEMatrix operator-(FEMatrix& otherMatrix);
	FEMatrix operator*(double& scalar);
	FEMatrix operator*(FEMatrix& otherMatrix);


private:
	int m_iRowNumber = 0;
	int m_iColumnNumber = 0;
	std::vector<std::vector<double>> m_vMatrixElements;
};

