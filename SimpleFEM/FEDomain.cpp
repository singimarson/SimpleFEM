#include "FEDomain.h"

#include <stdexcept>

// Constructor for if users want to initialize with the domain outline
template<std::size_t dim>
FEDomain<dim>::FEDomain(const std::vector<Point<dim>>& vDomainOutline) : FEObject()
{
	if (!SetDomainOutline(vDomainOutline))
	{
		throw std::runtime_error("FEDomain::FEDomain: Invalid domain outline.");
	}
}

// Sets the domain outline 
template<std::size_t dim>
bool FEDomain<dim>::SetDomainOutline(const std::vector<Point<dim>>& vDomainOutline)
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
template<std::size_t dim>
bool FEDomain<dim>::VerifyDomain(const std::vector<Point<dim>>& vDomainOutline)
{
	switch (dim)
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
