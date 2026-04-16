#pragma once

#include "FEFunctionLocal.h"
#include "FEMesh.h"
#include "FEObject.h"

#include <cstddef>
#include <stdexcept>
#include <unordered_map>

template <std::size_t dim>
class FESpace : public FEObject
{
public:
	// Constructor
	template<std::size_t D = dim, typename std::enable_if<D == 1 || D == 2 || D == 3, std::size_t>::type = 0>
	FESpace(FEMesh<dim>* pMesh) : m_pMesh(pMesh)
	{ }

	// Element order enum
	enum class ElementOrder
	{
		eLinear = 0
	};

	// Constructor with element order
	template<std::size_t D = dim, typename std::enable_if<D == 1 || D == 2 || D == 3, std::size_t>::type = 0>
	FESpace(FEMesh<dim>* pMesh, const ElementOrder& elementOrder) : m_pMesh(pMesh), m_eElementOrder(elementOrder)
	{ }

	// Object Type
	virtual bool SupportsObjectType(const long objType) override
	{
		return (FEObject::SupportsObjectType(objType) || objType == FE_OBJ_TYPE_SPACE);
	}

	// Function to create the space itself, i.e., Create all the functions
	void CreateElements()
	{
		switch (m_eElementOrder)
		{
		case ElementOrder::eLinear:
			CreateLinearElements();
			break;

		default:
			throw std::runtime_error("FESpace::CreateElements: Element order is invalid or not implemented yet.");
		}
	}


private:
	// Create linear elements
	void CreateLinearElements()
	{
		switch (dim)
		{
		case 1:
			std::unordered_map<int, Point<dim>> nodes = m_pMesh->GetNodes();

			for (const std::pair<int, std::vector<int>>& elementPair : m_pMesh->GetElements())
			{
				if (elementPair.second.size() != 2)
				{
					throw std::runtime_error("FESpace::CreateLinearElements: Number nodes in this element is incorrect.");
				}
				
				Point<dim> pt1 = nodes[elementPair.second[0]];
				Point<dim> pt2 = nodes[elementPair.second[1]];
				FEFunctionLocal<dim> func1([&pt1, &pt2](const Point<dim>& x) { return (x - pt1) / (pt2 - pt1); });
				FEFunctionLocal<dim> func2([&pt1, &pt2](const Point<dim>& x) { return (pt1 - x) / (pt1 - pt2); });

				m_mapLocalElementFunctions.insert({ elementPair.first, {func1, func2} });
			}
		}
	}

	FEMesh<dim>* m_pMesh = nullptr;
	ElementOrder m_eElementOrder = ElementOrder::eLinear;
	std::unordered_map<int, std::vector<FEFunctionLocal<dim>>> m_mapLocalElementFunctions;
};

