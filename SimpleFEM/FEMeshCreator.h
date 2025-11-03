#pragma once

#include "FEMesh.h"

#include <memory>

// Forward declaration of the FEMeshCreator class
// This class is intended to create and manage finite element meshes
class FEMeshCreator
{
public:
	FEMeshCreator(FEMesh* pMesh);
	~FEMeshCreator() = default;

	void CreateUniformMesh(const int iElementNumber);

private:
	std::unique_ptr<FEMesh> m_pMesh; // Pointer to the FEMesh object being created
};

