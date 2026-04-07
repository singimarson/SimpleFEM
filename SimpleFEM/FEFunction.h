#pragma once

#include "Point.h"
#include "FEObject.h"

#include <functional>

// Forward declaration of the FEFunction class
template <std::size_t dim>
class FEFunction : public FEObject
{
public:
	FEFunction(const std::function<double(Point<dim>)>& fFunction);

	// Object type
	virtual bool SupportsObjectType(const long objType) override
	{
		return (FEObject::SupportsObjectType(objType) || objType == FE_OBJ_TYPE_FUNCTION);
	}

	// Overrides
	double operator()(const Point<dim>& point) { return m_fFunction(point); }
	FEFunction operator+(FEFunction& fOtherFunction);
	FEFunction operator-(FEFunction& fOtherFunction);

	FEFunction operator*(FEFunction& fOtherFunction);
	FEFunction operator*(double& scalar);
	friend FEFunction operator*(double& scalar, FEFunction& feFunction);

	FEFunction operator/(FEFunction& fOtherFunction);
	FEFunction operator/(double& scalar);

	// Getters and Setters
	std::function<double(Point<dim>)> GetMainFunction() const { return m_fFunction; }

private:
	std::function<double(Point<dim>)> m_fFunction;
};

