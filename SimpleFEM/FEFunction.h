#pragma once

#include "Point.h"
#include "FEObject.h"

#include <functional>

// Forward declaration of the FEFunction class
template <std::size_t dim>
class FEFunction : public FEObject
{
public:
	// Constructors
	template<std::size_t D = dim, typename std::enable_if<D == 1 || D == 2 || D == 3, std::size_t>::type = 0>
	FEFunction(const std::function<double(Point<dim>)>& fFunction) : m_fFunction(fFunction)
	{
	}

	// Object type
	virtual bool SupportsObjectType(const long objType) override
	{
		return (FEObject::SupportsObjectType(objType) || objType == FE_OBJ_TYPE_FUNCTION);
	}

	// Overrides
	// This is the main function call operator that evaluates the function at a given point
	double operator()(const Point<dim>& point) { return m_fFunction(point); }

	// Addition operator override
	FEFunction operator+(FEFunction& fOtherFunction)
	{
		std::function<double(Point<dim>)> fAddedFunction = [&fThisfunction = *this, &fOtherFunction](const Point<dim>& point)
			{ return fThisfunction(point) + fOtherFunction(point); };

		return FEFunction<dim>(fAddedFunction);
	}

	// Subtraction override
	FEFunction operator-(FEFunction& fOtherFunction)
	{
		std::function<double(Point<dim>)> fSubtractedFunction = [&fThisfunction = *this, &fOtherFunction](const Point<dim>& point)
			{ return fThisfunction(point) - fOtherFunction(point); };

		return FEFunction<dim>(fSubtractedFunction);
	}

	// FEFunction multiplication override
	FEFunction operator*(FEFunction& fOtherFunction)
	{
		std::function<double(Point<dim>)> fMultipliedFunction = [&fThisfunction = *this, &fOtherFunction](const Point<dim>& point)
			{ return fThisfunction(point) * fOtherFunction(point); };

		return FEFunction<dim>(fMultipliedFunction);
	}

	// Scalar multiplication override
	FEFunction operator*(double& scalar)
	{
		std::function<double(Point<dim>)> fMultipliedFunction = [&fThisfunction = *this, &scalar](const Point<dim>& point)
			{ return scalar * fThisfunction(point); };

		return FEFunction<dim>(fMultipliedFunction);
	}

	// Left scalar multiplication overload
	friend FEFunction operator*(double& scalar, FEFunction& feFunction)
	{
		return feFunction * scalar;
	}

	// Division override
	FEFunction operator/(FEFunction& fOtherFunction)
	{
		std::function<double(Point<dim>)> fDividedFunction = [&fThisfunction = *this, &fOtherFunction](const Point<dim>& point)
			{ return fThisfunction(point) / fOtherFunction(point); };

		return FEFunction<dim>(fDividedFunction);
	}

	// Scalar division override
	FEFunction operator/(double& scalar)
	{
		if (scalar == 0.0)
		{
			throw std::runtime_error("FEDomain::FEDomain: Invalid domain outline.");
		}

		std::function<double(Point<dim>)> fDividedFunction = [&fThisfunction = *this, &scalar](const Point<dim>& point)
			{ return fThisfunction(point) / scalar; };

		return FEFunction<dim>(fDividedFunction);
	}

	// Getters and Setters
	std::function<double(Point<dim>)> GetMainFunction() const { return m_fFunction; }
	void SetMainFunction(const std::function<double(Point<dim>)>& function) { m_fFunction = function; }

private:
	std::function<double(Point<dim>)> m_fFunction;
};

