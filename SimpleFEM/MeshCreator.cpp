#include "FEDomain.h"
#include "MeshCreator.h"

#include <stdexcept>

// Constructor
template<std::size_t dim>
MeshCreator<dim>::MeshCreator(FEMesh<dim>* pMesh) : m_pMesh(pMesh)
{
	if (!pMesh)
	{
		throw std::runtime_error("FEMeshCreator::FEMeshCreator: pMesh is empty.");
	}
}

// Function to create a uniform mesh
template<std::size_t dim>
void MeshCreator<dim>::CreateUniformMesh(const int iElementNumber)
{
	if (iElementNumber < 1)
	{
		throw std::runtime_error("FEMeshCreator::CreateUniformMesh: Must have an element number greater than 0.");
	}

	m_pMesh->SetMeshType(FEMesh::MeshType::eUniform);

	std::unordered_map<int, Point> mapNodes;
	std::unordered_map<int, std::vector<int>> mapElements;

	FEDomain* pDomain = m_pMesh->GetDomain();

	switch (pDomain->GetDimension())
	{
		case 1:
		{
			// If we have dimension 1, then the mesh will always be a line mesh
			// Reconfigure this to work with Point
			std::vector<Point<dim>> vDomainOutline = pDomain->GetDomainOutline();
			if (vDomainOutline.size() != 2)
			{
				throw std::runtime_error("FEMeshCreator::CreateUniformMesh: Size of domain outline is incorrect");
			}

			Point<dim> ptX0 = vDomainOutline.front();
			Point<dim> ptXn1 = vDomainOutline.back();
			Point<dim> ptLineDiff = ptXn1 - ptX0;
			const double dLength = ptLineDiff;
			const double dElementLength = dLength / static_cast<double>(iElementNumber);

			/*
			Uniform line mesh will always look like this:
			|----o----o----...----o----|
			^    ^    ^           ^    ^
			x_0  x_1  x_2         x_n  x_(n+1)
			Where there are 'iElementNumber' elements and 'iElementNumber + 1' nodes

			x_i = x_0 + i * dElementLength for i = 0 to iElementNumber
			*/

			for (int iElementIndex = 0; iElementIndex < iElementNumber + 1; ++iElementIndex)
			{
				Point ptNodeLocation(ptX0.GetX() + iElementIndex * dElementLength, 0.0, 0.0);
				mapNodes[iElementIndex] = ptNodeLocation;

				if (iElementIndex == 0)
				{
					continue;
				}

				std::vector<int> vecElementConnections;

				vecElementConnections.push_back(iElementIndex - 1);
				vecElementConnections.push_back(iElementIndex);

				mapElements[iElementIndex] = vecElementConnections;
			}
		}
	}

	m_pMesh->SetNodes(mapNodes);
	m_pMesh->SetElements(mapElements);
}
