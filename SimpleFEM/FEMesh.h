#pragma once

#include "Point.h"
#include "FEDomain.h"
#include "FEObject.h"
#include "FETypes.h"

#include <unordered_map>
#include <vector>

// Forward declaration of the FEMesh class
template <std::size_t dim>
class FEMesh : public FEObject
{
public:
	// Constructors and destructor
	template<std::size_t D = dim, typename std::enable_if<D == 1 || D == 2 || D == 3, std::size_t>::type = 0>
	FEMesh(FEDomain<dim>* pDomain) : FEObject(), m_pDomain(pDomain)
	{
		// If the dimension is 1, the element shape will always be a line
		if (dim == 1)
		{
			m_eElementShape = ElementShape::eLine;
		}
	}

	virtual ~FEMesh() = default;

	// Object Type
	virtual bool SupportsObjectType(const long objType) override
	{
		return ( FEObject::SupportsObjectType(objType) || objType == FE_OBJ_TYPE_MESH);
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

	FEDomain<dim>* GetDomain() const { return m_pDomain; }
	bool SetDomain(FEDomain<dim>* pDomain)
	{
		m_pDomain = pDomain;
		return true;
	}

	FEMesh::ElementShape GetElementShape() const { return m_eElementShape; };

	std::unordered_map<int, Point<dim>> GetNodes() const { return m_vNodes; }
	bool SetNodes(const std::unordered_map<int, Point<dim>>& nodes)
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
	FEDomain<dim>* m_pDomain = nullptr;

	FEMesh::ElementShape m_eElementShape = ElementShape::eLine; ///< Shape of the elements, Line for now
	FEMesh::MeshType m_eMeshType = MeshType::eUniform;			///< Type of mesh, Uniform for now

	std::unordered_map<int, Point<dim>> m_vNodes;		   ///< member variable to store node numbers and coordinates
	std::unordered_map<int, std::vector<int>> m_vElements; ///< Member variable to store element numbers and connectivity	
};

