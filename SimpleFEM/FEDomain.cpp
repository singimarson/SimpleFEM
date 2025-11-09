#include "FEDomain.h"

#include <stdexcept>

// Constructor
FEDomain::FEDomain(const int iDimension) : m_iDimension(iDimension) {}

// Constructor for if users want to initialize with the domain outline
FEDomain::FEDomain(const int iDimension, const std::vector<C3DPoint>& vDomainOutline) : m_iDimension(iDimension)
{
	if (!SetDomainOutline(vDomainOutline))
	{
		throw std::runtime_error("FEDomain::FEDomain: Invalid domain outline.");
	}
}

// Sets the domain outline 
bool FEDomain::SetDomainOutline(const std::vector<C3DPoint>& vDomainOutline)
{
	if (VerifyDomain(vDomainOutline))
	{
		m_vDomainOutline = vDomainOutline;
		return true;
	}
	else
	{
		return false;
	}
}

// Function to verify the domain outline.
bool FEDomain::VerifyDomain(const std::vector<C3DPoint>& vDomainOutline)
{
	switch (m_iDimension)
	{
		case 1:
		{
			// If we have a 1 dimensional domain, the input vector should only be two points
			// 
			// |-------------....------------------|
			// ^                                   ^
			// 0                                   1

			if (vDomainOutline.size() != 2 ||
				vDomainOutline[0].GetY() != 0.0 || vDomainOutline[0].GetZ() != 0.0 ||
				vDomainOutline[1].GetY() != 0.0 || vDomainOutline[1].GetZ() != 0.0)
			{
				return false;
			}
		}
	}

	return true;
}
