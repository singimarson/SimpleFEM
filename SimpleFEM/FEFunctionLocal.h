#pragma once
#include "FEFunction.h"
#include "Point.h"


template <std::size_t dim>
class FEFunctionLocal : public FEFunction<dim>
{
public:
	// Constructors
	template<std::size_t D = dim, typename std::enable_if<D == 1 || D == 2 || D == 3, std::size_t>::type = 0>
	FEFunctionLocal() : FEFunction<dim>()
	{ }

	template<std::size_t D = dim, typename std::enable_if<D == 1 || D == 2 || D == 3, std::size_t>::type = 0>
	FEFunctionLocal(const std::function<double(Point<dim>)>& fFunction, const int& iElementNumber) : FEFunction<dim>(fFunction), 
		m_iElementNumber(iElementNumber) {}

	// Object type
	virtual bool SupportsObjectType(const long objType) override
	{
		return (FEObject::SupportsObjectType(objType) || objType == FE_OBJ_TYPE_LOCAL_FUNCTION);
	}

	// Getters and Setters
	int GetAssociatedElementNumber() const { return m_iElementNumber; }

private:
	int m_iElementNumber = 0;
};

