#include "BoundaryConditionHandler.h"

#include <stdexcept>

namespace BoundaryConditionHandler
{

// Main function for handling boundary conditions
// This function is only useful for 1D implementations, higher dimensions is a future Sean problem
void HandleBoundaryConditions(FESquareMatrix& mSystemMatrix, FEVector& vSystemRHS, const std::unordered_map<int, double>& mapBoundaryConditions)
{
	for (const std::pair<int, double>& pNodeBCPair : mapBoundaryConditions)
	{
		const int iNodeIndex = pNodeBCPair.first;
		const double dBoundaryCondition = pNodeBCPair.second;
		if (mSystemMatrix.GetColumn(iNodeIndex).Norm() > 0.0)
		{
			throw std::runtime_error("BoundaryConditionHandler::HandleBoundaryConditions: Matrix should not be filled out on a boundary node.");
		}

		mSystemMatrix.Set(iNodeIndex, iNodeIndex, 1.0);
		vSystemRHS.Set(iNodeIndex, dBoundaryCondition);
	}
}

} // namespace BoundaryConditionHandler