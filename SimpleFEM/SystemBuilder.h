#pragma once

#include "FEFunction.h"
#include "FESquareMatrix.h"
#include "FEMesh.h"

#include <unordered_map>

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

	// struct for PDE terms
	struct EquationTerms
	{
		bool m_bD0 = false;
		bool m_bD1 = false;
	};

	std::pair<FESquareMatrix, FEVector> CreateSystem(FEMesh<1>* pMesh,
													 const EquationTerms& terms,
													 const ElementOrder eOrder,
													 FEFunction<1>& rhsFunc,
													 const std::vector<double>& vBoundaryConditions);

private:
	std::pair<FESquareMatrix, FEVector> CreateLinearElements(FEMesh<1>* pMesh,
															 const EquationTerms& terms,
															 FEFunction<1>& rhsFunc,
															 const std::vector<double>& vBoundaryConditions);
	double LinearElementFunction(const int& iElementNumber,
								 const EquationTerms& terms,
								 const Point<1>& x,
								 const Point<1>& pt1,
								 const Point<1>& pt2);

	std::unordered_map<int, double> MatchBCToNode(const std::vector<int>& vBoundaryNodes, const std::vector<double>& vBoundaryConditions);
};