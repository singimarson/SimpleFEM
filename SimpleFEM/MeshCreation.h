#pragma once

#include "FEMesh.h"

#include <memory>

// This is the set of functions responsible for creating meshes of various types and dimensions.
namespace MeshCreation
{
	void CreateUniformMesh(FEMesh<1>* pMesh, const int iElementNumber);
}

