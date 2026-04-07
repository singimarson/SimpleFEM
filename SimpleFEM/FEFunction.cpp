#include "FEFunction.h"

#include <stdexcept>

// Constructor
template <std::size_t dim>
FEFunction<dim>::FEFunction(const std::function<double(Point<dim>)>& fFunction) : m_fFunction(fFunction)
{
}

// Addition overload
template <std::size_t dim>
FEFunction<dim> FEFunction<dim>::operator+(FEFunction<dim>& fOtherFunction)
{
	std::function<double(Point<dim>)> fAddedFunction = [&fThisfunction = *this, &fOtherFunction](const Point<dim>& point)
		{ return fThisfunction(point) + fOtherFunction(point); };

	return FEFunction<dim>(fAddedFunction);
}

// Subtraction overload
template <std::size_t dim>
FEFunction<dim> FEFunction<dim>::operator-(FEFunction<dim>& fOtherFunction)
{
	std::function<double(Point<dim>)> fSubtractedFunction = [&fThisfunction = *this, &fOtherFunction](const Point<dim>& point)
		{ return fThisfunction(point) - fOtherFunction(point); };

	return FEFunction<dim>(fSubtractedFunction);
}

// Multiplication overload
template <std::size_t dim>
FEFunction<dim> FEFunction<dim>::operator*(FEFunction<dim>& fOtherFunction)
{
	std::function<double(Point<dim>)> fMultipliedFunction = [&fThisfunction = *this, &fOtherFunction](const Point<dim>& point)
		{ return fThisfunction(point) * fOtherFunction(point); };

	return FEFunction<dim>(fMultipliedFunction);
}

// Scalar multiplication overload
template <std::size_t dim>
FEFunction<dim> FEFunction<dim>::operator*(double& scalar)
{
	std::function<double(Point<dim>)> fMultipliedFunction = [&fThisfunction = *this, &scalar](const Point<dim>& point)
		{ return scalar * fThisfunction(point); };

	return FEFunction<dim>(fMultipliedFunction);
}

// Left scalar multiplication 
template <std::size_t dim>
FEFunction<dim> operator*(double& scalar, FEFunction<dim>& feFunction)
{
	return feFunction * scalar;
}

// Division overload
template <std::size_t dim>
FEFunction<dim> FEFunction<dim>::operator/(FEFunction<dim>& fOtherFunction)
{
	std::function<double(Point<dim>)> fDividedFunction = [&fThisfunction = *this, &fOtherFunction](const Point<dim>& point)
		{ return fThisfunction(point) / fOtherFunction(point); };

	return FEFunction<dim>(fDividedFunction);
}

// Scalar division overload
template <std::size_t dim>
FEFunction<dim> FEFunction<dim>::operator/(double& scalar)
{
	if (scalar == 0.0)
	{
		throw std::runtime_error("FEDomain::FEDomain: Invalid domain outline.");
	}

	std::function<double(Point<dim>)> fDividedFunction = [&fThisfunction = *this, &scalar](const Point<dim>& point)
		{ return fThisfunction(point) / scalar; };

	return FEFunction<dim>(fDividedFunction);
}
