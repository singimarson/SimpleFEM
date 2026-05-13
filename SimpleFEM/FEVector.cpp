#include "FEVector.h"

#include <cmath>
#include <stdexcept>

// General construction
FEVector::FEVector(const int& iVectorLength) : m_iVectorLength(iVectorLength), m_vVectorElements(std::vector<double>(iVectorLength))
{}

// Construction for vectors
FEVector::FEVector(const std::vector<double>& vector) : m_iVectorLength(vector.size()), m_vVectorElements(vector)
{}

// Addition operator overload
FEVector FEVector::operator+(const FEVector& otherVector) const
{
	std::vector<double> vecSum(m_iVectorLength);
	for (int iVecIter = 0; iVecIter < m_iVectorLength; ++iVecIter)
	{
		vecSum[iVecIter] = m_vVectorElements[iVecIter] + otherVector.Get(iVecIter);
	}

	return FEVector(vecSum);
}

// Subtraction operator overload
FEVector FEVector::operator-(const FEVector& otherVector) const
{
	std::vector<double> vecDiff(m_iVectorLength);
	for (int iVecIter = 0; iVecIter < m_iVectorLength; ++iVecIter)
	{
		vecDiff[iVecIter] = m_vVectorElements[iVecIter] - otherVector.Get(iVecIter);
	}

	return FEVector(vecDiff);
}

// Scalar multiplication operator overload
FEVector FEVector::operator*(const double& scalar) const
{
	std::vector<double> vecMult(m_iVectorLength);
	for (int iVecIter = 0; iVecIter < m_iVectorLength; ++iVecIter)
	{
		vecMult[iVecIter] = scalar * m_vVectorElements[iVecIter];
	}

	return FEVector(vecMult);
}

// Left multiplication
FEVector operator*(const double& scalar, const FEVector& vector)
{
	return vector * scalar;
}

// Dot product for vectors
double FEVector::DotProduct(FEVector& otherVector)
{
	if (m_iVectorLength != otherVector.GetSize())
	{
		throw std::runtime_error("FEVector::DotProduct: These two vectors are different sizes.");
	}

	double dDotProduct = 0;
	for (int iVecIter = 0; iVecIter < m_iVectorLength; ++iVecIter)
	{
		dDotProduct += m_vVectorElements[iVecIter] * otherVector.Get(iVecIter);
	}

	return dDotProduct;
}

// l2 norm for vector
double FEVector::Norm()
{
	double dNormSquared = 0;
	for (int iVecIter = 0; iVecIter < m_iVectorLength; ++iVecIter)
	{
		dNormSquared += m_vVectorElements[iVecIter] * m_vVectorElements[iVecIter];
	}
	return std::sqrt(dNormSquared);
}

// Normalize this vector
void FEVector::Normalize()
{
	double dNorm = Norm();
	for (int iVecIter = 0; iVecIter < m_iVectorLength; ++iVecIter)
	{
		m_vVectorElements[iVecIter] /= dNorm;
	}
}

// Return L2 error between two vectors.
double FEVector::L2Error(FEVector& otherVector)
{
	if (otherVector.GetSize() != m_iVectorLength)
	{
		throw std::runtime_error("FEVector::L2Error: These two vectors are different sizes.");
	}

	double dError = 0.0;
	for (int iVecIter = 0; iVecIter < m_iVectorLength; ++iVecIter)
	{
		dError += std::pow(Get(iVecIter) - otherVector.Get(iVecIter), 2);
	}

	return std::sqrt(dError);
}