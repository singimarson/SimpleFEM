#include "FEIntegrator.h"

#include <stdexcept>

template<std::size_t dim>
std::unordered_map<int, std::vector<double>> FEIntegrator<dim>::m_mapGaussPoints =
{ {1, {0.0}},
  {2, {-0.57735, 0.57735}},
  {3, {0.0, -0.774597, 0.774597}},
  {4, {-0.339981, 0.339981, -0.861136, 0.861136}},
  {5, {0.0, -0.538469, 0.538469, -0.90618, 0.90618} } };

template<std::size_t dim>
std::unordered_map<int, std::vector<double>> FEIntegrator<dim>::m_mapGaussWeights =
{ {1, {2.0}},
  {2, {1.0, 1.0}},
  {3, {0.888889, 0.555556, 0.555556}},
  {4, {0.652145, 0.652145, 0.347855, 0.347855}},
  {5, {0.568889, 0.478629, 0.478629, 0.236927, 0.236927} } };

// Constructor
template<std::size_t dim>
FEIntegrator<dim>::FEIntegrator(FEIntegrator<dim>::IntegratorMethod& eIntegratorMethod) : m_eIntegratorMethod(eIntegratorMethod)
{
}

// Integrate 1D
template<std::size_t dim>
double FEIntegrator<dim>::Integrate(FEFunction<dim>& feFunction, const double& dLowerBound, const double& dUpperBound, const int& iIntervalNumber)
{
	if (dim != 1)
	{
		throw std::runtime_error("FEIntegrator::Integrate: Quadrature is not implemented for dim > 1.");
	}

	switch (m_eIntegratorMethod)
	{
	case IntegratorMethod::eMidpoint:
		return Midpoint(feFunction, dLowerBound, dUpperBound, iIntervalNumber);
	case IntegratorMethod::eTrapezoidal:
		return Trapezoid(feFunction, dLowerBound, dUpperBound, iIntervalNumber);
	case IntegratorMethod::eSimpson13:
		return Simpson13(feFunction, dLowerBound, dUpperBound, iIntervalNumber);
	case IntegratorMethod::eSimpson38:
		return Simpson38(feFunction, dLowerBound, dUpperBound, iIntervalNumber);
	case IntegratorMethod::eGauss:
		return GaussQuadrature(feFunction, dLowerBound, dUpperBound, iIntervalNumber);
	default:
		throw std::runtime_error("FEIntegrator::Integrate: Invalid type of integrator.");
	}
}

// L^2 Inner product
template<std::size_t dim>
double FEIntegrator<dim>::L2InnerProduct1D(FEFunction<dim>& feFunction1, FEFunction<dim>& feFunction2, const double& dLowerBound, const double& dUpperBound, const int& iIntervalNumber)
{
	FEFunction feFunctionMult = feFunction1 * feFunction2;
	return Integrate(feFunctionMult, dLowerBound, dUpperBound, iIntervalNumber);
}

// Midpoint integrator on uniform grid
template<std::size_t dim>
double FEIntegrator<dim>::Midpoint(FEFunction<dim>& feFunction, const double& dLowerBound, const double& dUpperBound, const int& iIntervalNumber)
{
	const double dDeltaX = (dUpperBound - dLowerBound) / static_cast<double>(iIntervalNumber);

	double x_prev = dLowerBound;
	double x_next = x_prev + dDeltaX;

	double solution = 0.0;

	for (int iFuncIter = 0; iFuncIter < iIntervalNumber; ++iFuncIter)
	{
		solution += feFunction(Point<dim>(0.5 * (x_prev + x_next)));
		
		x_prev = x_next;
		x_next += dDeltaX;
	}

	return dDeltaX * solution;
}

// Trapezoidal integrator on uniform grid
template<std::size_t dim>
double FEIntegrator<dim>::Trapezoid(FEFunction<dim>& feFunction, const double& dLowerBound, const double& dUpperBound, const int& iIntervalNumber)
{
	const double dDeltaX = (dUpperBound - dLowerBound) / static_cast<double>(iIntervalNumber);
	double solution = 0.5 * (feFunction(Point<dim>(dLowerBound)) + feFunction(Point<dim>(dUpperBound)));

	for (int iFuncIter = 1; iFuncIter < iIntervalNumber; ++iFuncIter)
	{
		solution += feFunction(Point<dim>(dLowerBound + iFuncIter * dDeltaX));
	}

	return dDeltaX * solution;
}

// Simpson's 1/3 rule for integration
template<std::size_t dim>
double FEIntegrator<dim>::Simpson13(FEFunction<dim>& feFunction, const double& dLowerBound, const double& dUpperBound, const int& iIntervalNumber)
{
	const double dDeltaX = (dUpperBound - dLowerBound) / static_cast<double>(iIntervalNumber);
	double solution = feFunction(Point<dim>(dLowerBound)) + feFunction(Point<dim>(dUpperBound));

	for (int iFuncIter = 1; iFuncIter < iIntervalNumber; ++iFuncIter)
	{
		double coeff = iFuncIter % 2 ? 4.0 : 2.0;
		solution += coeff * feFunction(Point<dim>(dLowerBound + iFuncIter * dDeltaX));
	}

	return dDeltaX * solution / 3.0;
}

// Simpson's 3/8 rule for integration
template<std::size_t dim>
double FEIntegrator<dim>::Simpson38(FEFunction<dim>& feFunction, const double& dLowerBound, const double& dUpperBound, const int& iIntervalNumber)
{
	if (iIntervalNumber % 3 != 0)
	{
		throw std::runtime_error("FEIntegrator::Simpson38: Invalid interval number for Simpson's 3/8 rule.");
	}

	const double dDeltaX = (dUpperBound - dLowerBound) / static_cast<double>(iIntervalNumber);
	double solution = feFunction(Point<dim>(dLowerBound)) + feFunction(Point<dim>(dUpperBound));

	for (int iFuncIter = 1; iFuncIter < iIntervalNumber; ++iFuncIter)
	{
		double coeff = iFuncIter % 3 ? 3.0 : 2.0;
		solution += coeff * feFunction(Point<dim>(dLowerBound + iFuncIter * dDeltaX));
	}

	return 3.0 * dDeltaX * solution / 8.0;
}

// Gauss 1D quadrature (1-5 points so far)
template<std::size_t dim>
double FEIntegrator<dim>::GaussQuadrature(FEFunction<dim>& feFunction, const double& dLowerBound, const double& dUpperBound, const int& iNodeNumber)
{
	if (iNodeNumber < 0 || iNodeNumber > 5)
	{
		throw std::runtime_error("FEIntegrator::GaussQuadrature: Invalid number of nodes for Gaussian quadrature.");
	}

	// We need a linear transformation so we can integrate over [-1,1] instead of [a,b]
	const double dTransform = 0.5 * (dUpperBound - dLowerBound);
	const double dMidpoint = 0.5 * (dUpperBound + dLowerBound);
	double solution = 0.0;

	std::vector<double> vecGaussPoints = m_mapGaussPoints[iNodeNumber];
	std::vector<double> vecGaussWeights = m_mapGaussWeights[iNodeNumber];

	for (int iQuadIter = 0; iQuadIter < iNodeNumber; ++iQuadIter)
	{
		solution += vecGaussWeights[iQuadIter] *
			feFunction(Point<dim>(dTransform * vecGaussPoints[iQuadIter] + dMidpoint));
	}

	return dTransform * solution;
}
