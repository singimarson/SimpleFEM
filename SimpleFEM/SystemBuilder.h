#pragma once

#include "FESquareMatrix.h"
#include "FEMesh.h"

class SystemBuilder
{
public:
	// Constructor/Destructor
	SystemBuilder() = default;
	~SystemBuilder() = default;

	// Element order enum
	enum class ElementOrder
	{
		eLinear = 0
	};

	// Derivative order enum
	enum class DerivativeOrder
	{
		eNone = 0,
		eFirstOrder
	};

	FESquareMatrix CreateSystemMatrix(FEMesh<1>* m_pMesh, const DerivativeOrder& m_dOrder, const ElementOrder m_eOrder);

private:
	FESquareMatrix CreateLinearD0Elements(FEMesh<1>* m_pMesh);
};