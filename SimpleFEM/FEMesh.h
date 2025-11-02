#pragma once

#include "C3DPoint.h"
#include "FETypes.h"

#include <unordered_map>
#include <vector>

// Forward declaration of the FEMesh class
class FEMesh
{
public:
	FEMesh(int iDimension);
	~FEMesh() = default;

	virtual bool SupportsObjectType(const long objType) const
	{
		return (objType == FE_OBJ_TYPE_MESH);
	}

	enum class MeshType
	{
		eUniform = 0
	};

	enum class ElementShape
	{
		eLine = 0,
		eTriangle,
		eTetrahedron
	};

	// Getters and Setters
	FEMesh::MeshType GetMeshType() const { return m_eMeshType; }
	bool SetMeshType(const FEMesh::MeshType type)
	{
		m_eMeshType = type;
		return true;
	}

	FEMesh::ElementShape GetElementShape() const { return m_eElementShape; };

	int GetDimension() const { return m_iDimension; }
	bool SetDimension(const int dim)
	{
		m_iDimension = dim;
		return true;
	}

	std::unordered_map<int, C3DPoint> GetNodes() const { return m_vNodes; }
	bool SetNodes(const std::unordered_map<int, C3DPoint>& nodes)
	{
		m_vNodes = nodes;
		return true;
	}

	std::unordered_map<int, std::vector<int>> GetElements() const { return m_vElements; }
	bool SetElements(const std::unordered_map<int, std::vector<int>>& elements)
	{
		m_vElements = elements;
		return true;
	}

private:
	int m_iDimension = 1; // Dimension of the mesh (1D, 2D, 3D), could probably be templated but worry about that later

	FEMesh::ElementShape m_eElementShape = ElementShape::eLine; ///< Shape of the elements, Line for now
	FEMesh::MeshType m_eMeshType = MeshType::eUniform;			///< Type of mesh, Uniform for now

	std::unordered_map<int, C3DPoint> m_vNodes;		 ///< member variable to store node numbers and coordinates
	std::unordered_map<int, std::vector<int>> m_vElements; ///< Member variable to store element numbers and connectivity	
};

