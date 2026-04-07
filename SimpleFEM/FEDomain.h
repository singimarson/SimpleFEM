#pragma once

#include "Point.h"
#include "FEObject.h"
#include "FETypes.h"

#include <vector>

// Forward declaration of the FEDomain class
template <std::size_t dim>
class FEDomain : public FEObject
{
public:
	FEDomain() = default;
	FEDomain(const std::vector<Point<dim>>& vDomainOutline);
	virtual ~FEDomain() {};

	// Object type
	virtual bool SupportsObjectType(const long objType) override
	{
		return (FEObject::SupportsObjectType(objType) || objType == FE_OBJ_TYPE_DOMAIN);
	}

	// Getters and setters
	std::vector<Point<dim>> GetDomainOutline() const { return m_vDomainOutline; }
	bool SetDomainOutline(const std::vector<Point<dim>>& vDomainOutline);

private:
	bool VerifyDomain(const std::vector<Point<dim>>& vDomainOutline);

	std::vector<Point<dim>> m_vDomainOutline;
};

