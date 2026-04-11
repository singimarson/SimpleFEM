#pragma once

#include "Point.h"
#include "FEObject.h"
#include "FETypes.h"

#include <stdexcept>
#include <type_traits>
#include <vector>

// Forward declaration of the FEDomain class
template <std::size_t dim>
class FEDomain : public FEObject
{
public:
	// Constructors and destructor
	template<std::size_t D = dim, typename std::enable_if<D == 1 || D == 2 || D == 3, std::size_t>::type = 0>
	FEDomain() {}

	template<std::size_t D = dim, typename std::enable_if<D == 1 || D == 2 || D == 3, std::size_t>::type = 0>
	FEDomain(const std::vector<Point<dim>>& vDomainOutline) : FEObject()
	{
		if (!SetDomainOutline(vDomainOutline))
		{
			throw std::runtime_error("FEDomain::FEDomain: Invalid domain outline.");
		}
	};

	virtual ~FEDomain() {};

	// Object type
	virtual bool SupportsObjectType(const long objType) override
	{
		return (FEObject::SupportsObjectType(objType) || objType == FE_OBJ_TYPE_DOMAIN);
	}

	// Getters and setters
	std::vector<Point<dim>> GetDomainOutline() const { return m_vDomainOutline; }
	bool SetDomainOutline(const std::vector<Point<dim>>& vDomainOutline)
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

private:
	bool VerifyDomain(const std::vector<Point<dim>>& vDomainOutline)
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

			if (vDomainOutline.size() != 2)
			{
				return false;
			}
		}
		}

		return true;
	}

	std::vector<Point<dim>> m_vDomainOutline;
};

