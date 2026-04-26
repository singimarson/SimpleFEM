#pragma once

#include "FEObject.h"
#include "FEVector.h"

#include <vector>

class FEMatrix : public FEObject
{
public:
	// Constructors and destructors
	FEMatrix() = default;
	FEMatrix(const int& iRowNum, const int& iColNum);
	FEMatrix(const std::vector<std::vector<double>>& matrix);

	~FEMatrix() = default;

	// Object Type
	virtual bool SupportsObjectType(const long objType) override
	{
		return (FEObject::SupportsObjectType(objType) || objType == FE_OBJ_TYPE_MATRIX);
	}

	// Element accessing functions
	double Get(const int& iRow, const int& iColumn) const { return m_vMatrixElements[iRow][iColumn]; }
	void Set(const int& iRow, const int& iColumn, const double& value) { m_vMatrixElements[iRow][iColumn] = value; }

	int GetRowSize() const { return m_iRowNumber; }
	int GetColumnSize() const { return m_iColumnNumber; }

	FEVector GetRow(const int& iRow) const { return FEVector(m_vMatrixElements[iRow]); }
	void SetRow(const int& iRow, FEVector& vector);

	FEVector GetColumn(const int& iColumn) const;
	void SetColumn(const int& iColumn, FEVector& row);

	// Operator overloads
	FEMatrix operator+(FEMatrix& otherMatrix);
	FEMatrix operator-(FEMatrix& otherMatrix);
	FEMatrix operator*(double& scalar);
	FEVector operator*(FEVector& feVector);
	FEMatrix operator*(FEMatrix& otherMatrix);


protected:
	int m_iRowNumber = 0;
	int m_iColumnNumber = 0;

	// todo: refactor this to just be one vector of doubles of size row * column
	std::vector<std::vector<double>> m_vMatrixElements;
};

