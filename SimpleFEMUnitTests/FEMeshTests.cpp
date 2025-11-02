#include "pch.h"

#include "../SimpleFEM/FEMesh.h"
#include "../SimpleFEM/FEMeshCreator.h"

TEST(FEMeshTests, MeshConstruction) 
{
	const int iDimension = 1;
	std::unique_ptr<FEMesh> pMesh1D = std::make_unique<FEMesh>(iDimension);
		
	EXPECT_EQ(pMesh1D->GetDimension(), iDimension);
	EXPECT_EQ(pMesh1D->GetElementShape(), FEMesh::ElementShape::eLine);
	
	std::unique_ptr<FEMesh> pMesh3D = std::make_unique<FEMesh>(3);

	EXPECT_EQ(pMesh3D->GetDimension(), 3);
}

TEST(FEMeshTests, MeshCreation)
{
	const int iDimension = 1;
	std::unique_ptr<FEMesh> pMesh = std::make_unique<FEMesh>(iDimension);

	FEMeshCreator meshCreator(pMesh.get());

	const int iMeshNumber = 2;
	meshCreator.CreateUniformMesh(iMeshNumber);

}