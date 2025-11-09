#include "pch.h"
#include "CppUnitTest.h"

#include "C3DPoint.h"
#include "CAssert.h"
#include "FEMesh.h"
#include "FEDomain.h"

#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SimpleFEMTesting
{
	// C3DPoint tests
	TEST_CLASS(C3DPointTests)
	{
	public:
		TEST_METHOD(C3DPointGetters)
		{
			// Base constructor
			C3DPoint ptOrigin;
			CAssert::AreEqual(ptOrigin.GetX(), 0.0);
			CAssert::AreEqual(ptOrigin.GetY(), 0.0);
			CAssert::AreEqual(ptOrigin.GetZ(), 0.0);

			// Value constructor
			C3DPoint ptPoint(1.0, 2.0, 3.0);
			CAssert::AreEqual(ptPoint.GetX(), 1.0);
			CAssert::AreEqual(ptPoint.GetY(), 2.0);
			CAssert::AreEqual(ptPoint.GetZ(), 3.0);
		}

		TEST_METHOD(C3DPointAddition)
		{
			C3DPoint ptA(2.3, 1.1, 8.2);
			C3DPoint ptB(4.4, 2.1, 0.1);
			C3DPoint ptSum(6.7, 3.2, 8.3);
			C3DPoint ptDiff(2.1, 1.0, -8.1);
			
			const double dTolerance = 1e-8;
			CAssert::AreEqual(ptA + ptB, ptSum, dTolerance);
			CAssert::AreEqual(ptB - ptA, ptDiff, dTolerance);
		}
	};

	// FEDomain tests
	TEST_CLASS(FEDomainTests)
	{
	public:
		
		TEST_METHOD(DomainGetters)
		{
			int iDimension = 1;
			std::unique_ptr<FEDomain> pDomain = std::make_unique<FEDomain>(iDimension);
			CAssert::AreEqual(iDimension, pDomain->GetDimension());

			C3DPoint ptA(0.0, 0.0, 0.0);
			C3DPoint ptB(1.0, 0.0, 0.0);
			const std::vector<C3DPoint> vDomainPoints = { ptA, ptB };
			pDomain->SetDomainOutline(vDomainPoints);
			const std::vector<C3DPoint> vTestDomainPoints = pDomain->GetDomainOutline();

			CAssert::AreEqual(vDomainPoints.size(), vTestDomainPoints.size());
			for (std::size_t ptIndex = 0; ptIndex < vTestDomainPoints.size(); ++ptIndex)
			{
				CAssert::AreEqual(vDomainPoints[ptIndex].GetX(), vTestDomainPoints[ptIndex].GetX());
				CAssert::AreEqual(vDomainPoints[ptIndex].GetY(), vTestDomainPoints[ptIndex].GetY());
				CAssert::AreEqual(vDomainPoints[ptIndex].GetZ(), vTestDomainPoints[ptIndex].GetZ());
			}
		}
	};
}
