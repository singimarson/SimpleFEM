#include "FERectangularMesh.h"

// Constructor
FERectangularMesh::FERectangularMesh(int iDim, double dLengthX, double dLengthY, double dLengthZ) : FEMesh(iDim), 
	m_dLengthX(dLengthX), m_dLengthY(dLengthY), m_dLengthZ(dLengthZ)
{
}
