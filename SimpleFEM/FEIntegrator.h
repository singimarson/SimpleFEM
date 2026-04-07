#pragma once

#include "FEFunction.h"
#include "FEObject.h"

#include <unordered_map>

// Class declaration of the main integrator functionality
template<std::size_t dim>
class FEIntegrator : public FEObject
{
public:
	FEIntegrator() = default;
	~FEIntegrator() = default;

	// Object type
	virtual bool SupportsObjectType(const long objType) override
	{
		return (FEObject::SupportsObjectType(objType) || objType == FE_OBJ_TYPE_INTEGRATOR);
	}

	// Integrator enum
	enum class IntegratorMethod
	{
		eMidpoint,
		eTrapezoidal,
		eSimpson13,
		eSimpson38,
		eGauss
	};

	FEIntegrator(FEIntegrator::IntegratorMethod& eIntegratorMethod);

	// Getters and setters
	void SetIntegratorMethod(const IntegratorMethod& method) { m_eIntegratorMethod = method; }
	IntegratorMethod GetIntegratorMethod() const { return m_eIntegratorMethod; }

	double Integrate(FEFunction<dim>& feFunction, const double& dLowerBound, const double& dUpperBound, const int& iIntervalNumber);
	double L2InnerProduct1D(FEFunction<dim>& feFunction1, FEFunction<dim>& feFunction2, const double& dLowerBound, const double& dUpperBound, const int& iIntervalNumber);

private:
	// Todo: These need to be generalized to take in FEDomain objects, and not the bounds. But it will work for now...
	double Midpoint(FEFunction<dim>& feFunction, const double& dLowerBound, const double& dUpperBound, const int& iIntervalNumber);
	double Trapezoid(FEFunction<dim>& feFunction, const double& dLowerBound, const double& dUpperBound, const int& iIntervalNumber);
	double Simpson13(FEFunction<dim>& feFunction, const double& dLowerBound, const double& dUpperBound, const int& iIntervalNumber);
	double Simpson38(FEFunction<dim>& feFunction, const double& dLowerBound, const double& dUpperBound, const int& iIntervalNumber);

	static std::unordered_map<int, std::vector<double>> m_mapGaussPoints;
	static std::unordered_map<int, std::vector<double>> m_mapGaussWeights;
	double GaussQuadrature(FEFunction<dim>& feFunction, const double& dLowerBound, const double& dUpperBound, const int& iNodeNumber);


	IntegratorMethod m_eIntegratorMethod = IntegratorMethod::eMidpoint;
};

