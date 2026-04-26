#include "SystemBuilder.h"

#include "FEFunction.h"
#include "FEIntegrator.h"

#include <memory>

// Main function driver to create linear system matrix
FESquareMatrix SystemBuilder::CreateSystemMatrix(FEMesh<1>* m_pMesh, const SystemBuilder::DerivativeOrder& m_dOrder, const SystemBuilder::ElementOrder m_eOrder)
{
	switch (m_dOrder)
	{
	case DerivativeOrder::eNone:
	{
		switch (m_eOrder)
		{
		case ElementOrder::eLinear:
			return CreateLinearD0Elements(m_pMesh);
			break;

		default:
			throw std::runtime_error("SystemBuilder::CreateSystemMatrix: Element order is invalid or not implemented yet.");
		}

		break;
	}
	case DerivativeOrder::eFirstOrder:
	{
		switch (m_eOrder)
		{
		case ElementOrder::eLinear:
			//CreateLinearD1Elements();
			break;

		default:
			throw std::runtime_error("SystemBuilder::CreateSystemMatrix: Element order is invalid or not implemented yet.");
		}

		break;
	}
	default:
		throw std::runtime_error("SystemBuilder::CreateSystemMatrix: Derivative order is invalid or not implemented yet.");
	}
	return FESquareMatrix();
}

// Create Linear zeroth derivative element matrix
FESquareMatrix SystemBuilder::CreateLinearD0Elements(FEMesh<1>* m_pMesh)
{
	// Just a little info on what we're doing
	// The matrix that we calculate here would be for the term in a PDE (u, v).
	// 
	// |\                /|
	// |  \Φ_2      Φ_1/  |
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
	//
	// todo: Doing an integral for this is probaby unnecessary. Once we get to the point where time
	// optimization is a thing, we can go back and do it a simpler way.
	const int dim = 1;

	// Integrator
	FEIntegrator<dim>::IntegratorMethod intMethod = FEIntegrator<dim>::IntegratorMethod::eGauss;
	std::unique_ptr<FEIntegrator<dim>> pIntegrator = std::make_unique<FEIntegrator<dim>>(intMethod);
	int iNumGaussPoints = 4;

	// Todo: Write some explanation for what's going on here
	std::unordered_map<int, Point<dim>> nodes = m_pMesh->GetNodes();

	FESquareMatrix mSystemMatrix(static_cast<int>(nodes.size()));

	for (const std::pair<int, std::vector<int>>& elementPair : m_pMesh->GetElements())
	{
		if (elementPair.second.size() != 2)
		{
			throw std::runtime_error("FESpace::CreateLinearD0Elements: Number nodes in this element is incorrect.");
		}

		const int pt1Index = elementPair.second[0];
		const int pt2Index = elementPair.second[1];

		const Point<dim> pt1 = nodes[pt1Index];
		const Point<dim> pt2 = nodes[pt2Index];

		auto func1 = [&pt1, &pt2](const Point<dim>& x) -> double
			{
				return x[0] >= pt1[0] || x[0] <= pt2[0] ? (x[0] - pt1[0]) / (pt2[0] - pt1[0]) : 0.0;
			};
		auto func2 = [&pt1, &pt2](const Point<dim>& x) -> double
			{
				return x[0] >= pt1[0] || x[0] <= pt2[0] ? (pt2[0] - x[0]) / (pt2[0] - pt1[0]) : 0.0;
			};

		FEFunction<dim> func1Obj(func1), func2Obj(func2);

		// Integrating the local element function products and throwing them into the global matrix
		double dIntegralResult = 0.0;

		// (\phi_i, \phi_i)
		dIntegralResult = pIntegrator->L2InnerProduct1D(func1Obj, func1Obj, pt1[0], pt2[0], iNumGaussPoints);
		mSystemMatrix.Set(pt1Index, pt1Index, mSystemMatrix.Get(pt1Index, pt1Index) + dIntegralResult);
		
		// (\phi_i, \phi_j)
		dIntegralResult = pIntegrator->L2InnerProduct1D(func1Obj, func2Obj, pt1[0], pt2[0], iNumGaussPoints);
		mSystemMatrix.Set(pt1Index, pt2Index, mSystemMatrix.Get(pt1Index, pt2Index) + dIntegralResult);

		// (\phi_j, \phi_i)
		mSystemMatrix.Set(pt2Index, pt1Index, mSystemMatrix.Get(pt2Index, pt1Index) + dIntegralResult);

		// (\phi_j, \phi_j)
		dIntegralResult = pIntegrator->L2InnerProduct1D(func2Obj, func2Obj, pt1[0], pt2[0], iNumGaussPoints);
		mSystemMatrix.Set(pt2Index, pt2Index, mSystemMatrix.Get(pt2Index, pt2Index) + dIntegralResult);
	}

	return mSystemMatrix;
}


