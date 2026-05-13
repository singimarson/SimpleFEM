#include "FEDomain.h"
#include "MeshCreation.h"

#include <stdexcept>

namespace MeshCreation
{
void MeshCreation::CreateUniformMesh(FEMesh<1>* m_pMesh, const int iElementNumber)
{
	if (!m_pMesh)
	{
		throw std::runtime_error("FEMeshCreator::CreateUniformMesh: Mesh pointer is null.");
	}

	if (iElementNumber < 1)
	{
		throw std::runtime_error("FEMeshCreator::CreateUniformMesh: Must have an element number greater than 0.");
	}

	m_pMesh->SetMeshType(FEMesh<1>::MeshType::eUniform);

	std::unordered_map<int, Point<1>> mapNodes;
	std::unordered_map<int, std::vector<int>> mapElements;

	FEDomain<1>* m_pDomain = m_pMesh->GetDomain();

	// If we have dimension 1, then the mesh will always be a line mesh
	// Reconfigure this to work with Point
	std::vector<Point<1>> vDomainOutline = m_pDomain->GetDomainOutline();
	if (vDomainOutline.size() != 2)
	{
		throw std::runtime_error("FEMeshCreator::CreateUniformMesh: Size of domain outline is incorrect");
	}

	Point<1> ptX0 = vDomainOutline.front();
	Point<1> ptXn1 = vDomainOutline.back();
	const double dLength = ptXn1.Distance(ptX0);
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
		Point<1> ptNodeLocation(ptX0[0] + iElementIndex * dElementLength);
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

	m_pMesh->SetNodes(mapNodes);
	m_pMesh->SetElements(mapElements);

	// Set boundary nodes, easy with this function because it's the first and last nodes
	m_pMesh->SetBoundaryNodes({ 0, iElementNumber });
}
} // namespace MeshCreation

