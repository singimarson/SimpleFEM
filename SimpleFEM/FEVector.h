#pragma once

#include "FEObject.h"

#include <vector>

class FEVector : public FEObject
{
public:
	// COnstructors and destructor
	FEVector() = default;
	FEVector(const int& iVectorLength);
	FEVector(const std::vector<double>& vector);

	~FEVector() = default;

	// Object Type
	virtual bool SupportsObjectType(const long objType) override
	{
		return (FEObject::SupportsObjectType(objType) || objType == FE_OBJ_VECTOR);
	}

	// Element interaction
	double Get(const int& iElementNumber) { return m_vVectorElements[iElementNumber]; }
	void Set(const int& iElementNumber, const double& value) { m_vVectorElements[iElementNumber] = value; }

	// Operator overload
	double operator[](const int& iElementNumber) { return Get(iElementNumber); }
	FEVector operator+(FEVector& otherVector);
	FEVector operator-(FEVector& otherVector);
	FEVector operator*(double& scalar);

	// Vector operations
	double DotProduct(FEVector& otherVector);

	// Getters and setters
	int GetSize() const { return m_iVectorLength; }

private:
	int m_iVectorLength = 0;

	std::vector<double> m_vVectorElements;
};

