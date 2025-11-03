#pragma once

#include "C3DPoint.h"

#include <vector>

// Forward declaration of the FEDomain class
class FEDomain
{
public:
	FEDomain(const int iDimension);
	FEDomain(const int iDimension, const std::vector<C3DPoint>& vDomainOutline);
	~FEDomain() = default;

	// Getters and setters
	int GetDimension() const { return m_iDimension; }
	bool SetDimension(const int iDimension)
	{
		m_iDimension = iDimension;
		return true;
	}

	std::vector<C3DPoint> GetDomainOutline() const { return m_vDomainOutline; }
	bool SetDomainOutline(const std::vector<C3DPoint>& vDomainOutline);

private:
	bool VerifyDomain(const std::vector<C3DPoint>& vDomainOutline);

	int m_iDimension = 1;
	std::vector<C3DPoint> m_vDomainOutline;
};

