#include "SystemBuilder.h"

#include "FEIntegrator.h"

#include <memory>
#include <unordered_map>

// Main function driver to create linear system matrix
std::pair<FESquareMatrix, FEVector> SystemBuilder::CreateSystem(FEMesh<1>* pMesh, 
																const EquationTerms& terms,
																const SystemBuilder::ElementOrder m_eOrder,
																FEFunction<1>& rhsFunc,
																const std::vector<double>& vBoundaryConditions)
{
	switch (m_eOrder)
	{
	case ElementOrder::eLinear:
		return CreateLinearElements(pMesh, terms, rhsFunc, vBoundaryConditions);
		break;

	default:
		throw std::runtime_error("SystemBuilder::CreateSystem: Element order is invalid or not implemented yet.");
	}
}

// Create Linear zeroth derivative element matrix
std::pair<FESquareMatrix, FEVector> SystemBuilder::CreateLinearElements(FEMesh<1>* pMesh,
																		const EquationTerms& terms,
																		FEFunction<1>& rhsFunc,
																		const std::vector<double>& vBoundaryConditions)
{
	// First set up boundary conditions
	const std::vector<int> vBoundaryNodes = pMesh->GetBoundaryNodes();
	std::unordered_map<int, double> mapBoundaryConditions = MatchBCToNode(vBoundaryNodes, vBoundaryConditions);

	// todo: Doing an integral for this is probaby unnecessary. Once we get to the point where time
	// optimization is a thing, we can go back and do it a simpler way.
	const int dim = 1;

	// Integrator
	FEIntegrator<dim>::IntegratorMethod intMethod = FEIntegrator<dim>::IntegratorMethod::eGauss;
	std::unique_ptr<FEIntegrator<dim>> pIntegrator = std::make_unique<FEIntegrator<dim>>(intMethod);
	const int iQuadraturePoints = 4;

	// Todo: Write some explanation for what's going on here
	std::unordered_map<int, Point<dim>> nodes = pMesh->GetNodes();

	FESquareMatrix mSystemMatrix(static_cast<int>(nodes.size()));
	FEVector vSystemRHS(static_cast<int>(nodes.size()));

	// Iterating through elements
	for (const std::pair<int, std::vector<int>>& elementPair : pMesh->GetElements())
	{
		if (elementPair.second.size() != 2)
		{
			throw std::runtime_error("SystemBuilder::CreateLinearElements: Number nodes in this element is incorrect.");
		}

		// phi_i index
		const int iNodeIndexI = elementPair.second[0];
		const bool bIsNodeIBC = std::find(vBoundaryNodes.begin(), vBoundaryNodes.end(), iNodeIndexI) != vBoundaryNodes.end();

		// phi_j index
		const int iNodeIndexJ = elementPair.second[1];
		const bool bIsNodeJBC = std::find(vBoundaryNodes.begin(), vBoundaryNodes.end(), iNodeIndexJ) != vBoundaryNodes.end();

		const Point<dim> pt1 = nodes[iNodeIndexI];
		const Point<dim> pt2 = nodes[iNodeIndexJ];

		auto func1 = [&pt1, &pt2, &terms, this](const Point<dim>& x) -> double
			{
				return this->LinearElementFunction(1, terms, x, pt1, pt2);
			};
		auto func2 = [&pt1, &pt2, &terms, this](const Point<dim>& x) -> double
			{
				return this->LinearElementFunction(2, terms, x, pt1, pt2);
			};

		// Because the rhs exists, we need to perform the inner products with these guys if the derivative order differs
		// Can probably be organized to be more optimal but I'm too lazy rn.
		EquationTerms rhsTerms;
		rhsTerms.m_bD0 = true;
		auto funcRHS1 = [&pt1, &pt2, &rhsTerms, this](const Point<dim>& x) -> double
			{
				return this->LinearElementFunction(1, rhsTerms, x, pt1, pt2);
			};		
		auto funcRHS2 = [&pt1, &pt2, &rhsTerms, this](const Point<dim>& x) -> double
			{
				return this->LinearElementFunction(2, rhsTerms, x, pt1, pt2);
			};

		FEFunction<dim> LocalFunction1(func1), LocalFunction2(func2), LocalRHS1(funcRHS1), LocalRHS2(funcRHS2);

		// Integrating the local element function products and throwing them into the global matrix
		double dIntegralResult = 0.0;

		// Integrating section
		// Only do integrating if the node we're looking at is NOT a boundary node
		// First integrate the phi_i * phi_i function or handle the boundary condition
		if (!bIsNodeIBC)
		{
			// (\phi_i, \phi_i)
			dIntegralResult = pIntegrator->L2InnerProduct1D(LocalFunction1, LocalFunction1, pt1[0], pt2[0], iQuadraturePoints);
			mSystemMatrix.Set(iNodeIndexI, iNodeIndexI, mSystemMatrix.Get(iNodeIndexI, iNodeIndexI) + dIntegralResult);

			// (\phi_i, f) 
			dIntegralResult = pIntegrator->L2InnerProduct1D(LocalRHS1, rhsFunc, pt1[0], pt2[0], iQuadraturePoints);
			vSystemRHS.Set(iNodeIndexI, vSystemRHS[iNodeIndexI] + dIntegralResult);
		}
		else
		{
			// Handle the boundary conditions
			mSystemMatrix.Set(iNodeIndexI, iNodeIndexI, 1.0);
			vSystemRHS.Set(iNodeIndexI, mapBoundaryConditions[iNodeIndexI]);
		}

		// Integrate the phi_j * phi_j function or handle the boundary condition
		if (!bIsNodeJBC)
		{
			// (\phi_j, \phi_j)
			dIntegralResult = pIntegrator->L2InnerProduct1D(LocalFunction2, LocalFunction2, pt1[0], pt2[0], iQuadraturePoints);
			mSystemMatrix.Set(iNodeIndexJ, iNodeIndexJ, mSystemMatrix.Get(iNodeIndexJ, iNodeIndexJ) + dIntegralResult);

			// (\phi_j, f)
			dIntegralResult = pIntegrator->L2InnerProduct1D(LocalRHS2, rhsFunc, pt1[0], pt2[0], iQuadraturePoints);
			vSystemRHS.Set(iNodeIndexJ, vSystemRHS[iNodeIndexJ] + dIntegralResult);
		}
		else
		{
			// Handle the boundary conditions
			mSystemMatrix.Set(iNodeIndexJ, iNodeIndexJ, 1.0);
			vSystemRHS.Set(iNodeIndexJ, mapBoundaryConditions[iNodeIndexJ]);
		}
		
		if (!bIsNodeIBC && !bIsNodeJBC)
		{
			// (\phi_i, \phi_j)
			dIntegralResult = pIntegrator->L2InnerProduct1D(LocalFunction1, LocalFunction2, pt1[0], pt2[0], iQuadraturePoints);
			mSystemMatrix.Set(iNodeIndexI, iNodeIndexJ, mSystemMatrix.Get(iNodeIndexI, iNodeIndexJ) + dIntegralResult);

			// (\phi_j, \phi_i)
			mSystemMatrix.Set(iNodeIndexJ, iNodeIndexI, mSystemMatrix.Get(iNodeIndexJ, iNodeIndexI) + dIntegralResult);
		}


	}

	return std::pair<FESquareMatrix, FEVector>(mSystemMatrix, vSystemRHS);
}

// Function that stores the element functions
double SystemBuilder::LinearElementFunction(const int& iElementNumber,
											const EquationTerms& terms,
											const Point<1>& x, 
											const Point<1>& pt1, 
											const Point<1>& pt2)
{
	double result = 0.0;

	if (terms.m_bD0)
	{
		// Just a little info on what we're doing
		// The matrix that we calculate here would be for the term in a PDE (u, v).
		// 
		// |\                /|
		// |  \Φ_1      Φ_2/  |
		// |    \        /    |
		// |      \    /      |  local matrix -> [ (Φ_1,Φ_1)  (Φ_1,Φ_2) ]
		// |        \/        |                  [ (Φ_2,Φ_1)  (Φ_2,Φ_2) ]
		// |        /\        |
		// |      /    \      |
		// |    /        \    |
		// |  /            \  |
		// |/________________\|
		// ^       Ω_j        ^
		// x_i             x_{i+1}
		if (iElementNumber == 1)
		{
			result += (x[0] >= pt1[0] || x[0] <= pt2[0]) ? (pt2[0] - x[0]) / (pt2[0] - pt1[0]) : 0.0;
		}
		else if (iElementNumber == 2)
		{
			result += (x[0] >= pt1[0] || x[0] <= pt2[0]) ? (x[0] - pt1[0]) / (pt2[0] - pt1[0]) : 0.0;
		}
		else
		{
			throw std::runtime_error("SystemBuilder::LinearElementFunction: Incorrect element index.");
		}
	}
	
	if (terms.m_bD1)
	{
		// Just a little info on what we're doing
		// The matrix that we calculate here would be for the term in a PDE (u, v).
		// 
		// |__________________| 1 / h
		// |       Φ'_2       |
		// |                  |
		// |                  |  local matrix -> [ (Φ'_1,Φ'_1)  (Φ'_1,Φ'_2) ]
		// |                  |                  [ (Φ'_2,Φ'_1)  (Φ'_2,Φ'_2) ]
		// |_______Ω_j________|0
		// |                  |
		// |                  |
		// |                  |
		// |__________________| -1 / h
		// ^       Φ'_1       ^
		// x_i             x_{i+1}
		if (iElementNumber == 1)
		{
			result += (x[0] >= pt1[0] || x[0] <= pt2[0]) ? -1.0 / (pt2[0] - pt1[0]) : 0.0;
		}
		else if (iElementNumber == 2)
		{
			result += (x[0] >= pt1[0] || x[0] <= pt2[0]) ? 1.0 / (pt2[0] - pt1[0]) : 0.0;
		}
		else
		{
			throw std::runtime_error("SystemBuilder::LinearElementFunction: Incorrect element index.");
		}
	}

	return result;
}

// Fixes up the boundary conditions to the actual boundary nodes
std::unordered_map<int, double> SystemBuilder::MatchBCToNode(const std::vector<int>& vBoundaryNodes, const std::vector<double>& vBoundaryConditions)
{
	if (vBoundaryNodes.size() != vBoundaryConditions.size())
	{
		throw std::runtime_error("SystemBuilder::MatchBCToNode: Size of boundary nodes and conditions must match.");
	}

	std::unordered_map<int, double> mapNodeBCs;

	// For now we're assuming the boundary conditions are ordered well, i.e., u(0) = a, u(1) = b
	// and vBoundaryConditions = {a, b}
	for (int iBCIndex = 0; iBCIndex < vBoundaryConditions.size(); ++iBCIndex)
	{
		mapNodeBCs.insert({ vBoundaryNodes[iBCIndex], vBoundaryConditions[iBCIndex] });
	}

	return mapNodeBCs;
}


