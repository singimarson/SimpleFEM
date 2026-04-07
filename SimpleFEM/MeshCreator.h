#pragma once

#include "FEMesh.h"

#include <memory>

// Forward declaration of the FEMeshCreator class
// This class is intended to create and manage finite element meshes
template<std::size_t dim>
class MeshCreator
{
public:
	MeshCreator(FEMesh<dim>* pMesh);
	~MeshCreator() = default;

	void CreateUniformMesh(const int iElementNumber);

private:
	FEMesh<dim>* m_pMesh = nullptr; // Pointer to the FEMesh object being created
};

