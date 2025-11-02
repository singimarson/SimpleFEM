#include "FEMesh.h"

// Constructor
FEMesh::FEMesh(int iDimension) : m_iDimension(iDimension)
{
	// If the dimension is 1, the element shape will always be a line
	if (iDimension == 1)
	{
		m_eElementShape = ElementShape::eLine;
	}
}
