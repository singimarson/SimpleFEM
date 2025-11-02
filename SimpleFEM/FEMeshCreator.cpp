#include "FEMeshCreator.h"
#include "FELineMesh.h"

#include <stdexcept>

// Constructor
FEMeshCreator::FEMeshCreator(FEMesh* pMesh)
{
	if (!pMesh)
	{
		throw std::runtime_error("FEMeshCreator::FEMeshCreator: pMesh is empty.");
	}

	// Probably a way to do this in the constructor but I'm lazy
	m_pMesh = std::unique_ptr<FEMesh>(pMesh);
}

// Function to create a uniform mesh
void FEMeshCreator::CreateUniformMesh(const int iElementNumber)
{
	if (iElementNumber < 1)
	{
		throw std::runtime_error("FEMeshCreator::CreateUniformMesh: Must have an element number greater than 0.");
	}

	m_pMesh->SetMeshType(FEMesh::MeshType::eUniform);

	std::unordered_map<int, C3DPoint> mapNodes;
	std::unordered_map<int, std::vector<int>> mapElements;

	switch (m_pMesh->GetDimension())
	{
		case 1:
		{
			// If we have dimension 1, then the mesh will always be a line mesh
			if (!m_pMesh->SupportsObjectType(FE_OBJ_TYPE_LINE_MESH))
			{
				throw std::runtime_error("FEMeshCreator::CreateUniformMesh: The provided mesh is not a line mesh for 1D meshing.");
			}

			FELineMesh* pLineMesh = static_cast<FELineMesh*>(m_pMesh.get());
			const double dLength = pLineMesh->GetLength();
			const double dElementLength = dLength / static_cast<double>(iElementNumber);

			/*
			Uniform line mesh will always look like this:
			|----o----o----...----o----|
			^    ^    ^           ^    ^
			x_0  x_1  x_2         x_n  x_(n+1)
			Where there are 'iElementNumber' elements and 'iElementNumber + 1' nodes

			x_i = i * (dElementLength / iElementNumber) for i = 0 to iElementNumber
			*/

			for (int iElementIndex = 0; iElementIndex < iElementNumber + 1; ++iElementIndex)
			{
				C3DPoint ptNodeLocation(iElementIndex * (dElementLength / iElementNumber), 0.0, 0.0);
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
