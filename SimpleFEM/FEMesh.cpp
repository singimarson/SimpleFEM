#include "FEMesh.h"

// Constructor
FEMesh::FEMesh(FEDomain* pDomain) : m_pDomain(pDomain)
{
	// If the dimension is 1, the element shape will always be a line
	if (pDomain->GetDimension() == 1)
	{
		m_eElementShape = ElementShape::eLine;
	}
}
