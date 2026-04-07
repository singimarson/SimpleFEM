#include "pch.h"
#include "CppUnitTest.h"

#include "CAssert.h"

#include "../SimpleFEM/Point.h"
#include "../SimpleFEM/FEFunction.h"
#include "../SimpleFEM/FEMesh.h"
#include "../SimpleFEM/FEDomain.h"
#include "../SimpleFEM/FEIntegrator.h"
#include "../SimpleFEM/FETypes.h"
#include "../SimpleFEM/MeshCreator.h"

#include <functional>
#include <memory>
#include <unordered_map>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testing
{
	static double dTolerance = 1e-8;

	// Point tests
	TEST_CLASS(PointTests)
	{
	public:
		TEST_METHOD(PointGetters)
		{
			// Base constructor
			Point<3> ptOrigin;
			CAssert::AreEqual(ptOrigin[0], 0.0);
			CAssert::AreEqual(ptOrigin[1], 0.0);
			CAssert::AreEqual(ptOrigin[2], 0.0);

			// Value constructor
			Point<3> ptPoint(1.0, 2.0, 3.0);
			CAssert::AreEqual(ptPoint[0], 1.0);
			CAssert::AreEqual(ptPoint[1], 2.0);
			CAssert::AreEqual(ptPoint[2], 3.0);
		}

		//TEST_METHOD(PointAddition)
		//{
		//	Point<3> ptA(2.3, 1.1, 8.2);
		//	Point<3> ptB(4.4, 2.1, 0.1);
		//	Point<3> ptSum(6.7, 3.2, 8.3);
		//	Point<3> ptDiff(2.1, 1.0, -8.1);

		//	CAssert::AreEqual(ptA + ptB, ptSum, dTolerance);
		//	CAssert::AreEqual(ptB - ptA, ptDiff, dTolerance);
		//}
	};

	//// FEDomain tests
	//TEST_CLASS(FEDomainTests)
	//{
	//public:
	//	TEST_METHOD(DomainGetters)
	//	{
	//		const std::size_t dim = 1;
	//		FEDomain<dim>* pDomain = new FEDomain<dim>();

	//		Point<dim> ptA(0.0);
	//		Point<dim> ptB(1.0);
	//		std::vector<Point<dim>> vDomainPoints = { ptA, ptB };
	//		pDomain->SetDomainOutline(vDomainPoints);
	//		const std::vector<Point<dim>> vTestDomainPoints = pDomain->GetDomainOutline();

	//		CAssert::AreEqual(vDomainPoints.size(), vTestDomainPoints.size());
	//		for (std::size_t ptIndex = 0; ptIndex < vTestDomainPoints.size(); ++ptIndex)
	//		{
	//			CAssert::AreEqual(vDomainPoints[ptIndex].GetX(), vTestDomainPoints[ptIndex].GetX());
	//			CAssert::AreEqual(vDomainPoints[ptIndex].GetY(), vTestDomainPoints[ptIndex].GetY());
	//			CAssert::AreEqual(vDomainPoints[ptIndex].GetZ(), vTestDomainPoints[ptIndex].GetZ());
	//		}

	//		delete pDomain;
	//	}
	//};

	//// FEMesh tests
	//TEST_CLASS(MeshCreatorTests)
	//{
	//public:
	//	TEST_METHOD(MeshCreation)
	//	{
	//		// Create the finite element domain
	//		int iDimension = 1;
	//		std::vector<Point> vEndpoints = { Point(), Point(1.0, 0.0, 0.0) };
	//		std::unique_ptr<FEDomain> pDomain = std::make_unique<FEDomain>(iDimension, vEndpoints);

	//		// Initialize the mesh class, uniform by default
	//		std::unique_ptr<FEMesh> pMesh = std::make_unique<FEMesh>(pDomain.get());

	//		// MeshCreator class
	//		const int iElementNumber = 2;
	//		MeshCreator meshCreator(pMesh.get());
	//		meshCreator.CreateUniformMesh(iElementNumber);

	//		// We expect the mesh with 2 elements to look like this:
	//		//
	//		// |--------------|--------------|
	//		// 0             0.5             1
	//		//
	//		// Now we test what MeshCreator gives us

	//		std::unordered_map<int, Point> mMeshNodeTest;
	//		mMeshNodeTest[0] = Point();
	//		mMeshNodeTest[1] = Point(0.5, 0.0, 0.0);
	//		mMeshNodeTest[2] = Point(1.0, 0.0, 0.0);

	//		std::unordered_map<int, Point> mMeshNodeResult = pMesh->GetNodes();

	//		// These aren't equal, fix the MeshCreator
	//		CAssert::AreEqual(mMeshNodeTest.size(), mMeshNodeResult.size());
	//		for (int iMeshIndex = 0; iMeshIndex < mMeshNodeTest.size(); ++iMeshIndex)
	//		{
	//			CAssert::AreEqual(mMeshNodeTest[iMeshIndex], mMeshNodeResult[iMeshIndex], dTolerance);
	//		}
	//	}
	//};

	//// FEFunction tests
	//TEST_CLASS(FEFunctionTests)
	//{
	//public:
	//	TEST_METHOD(FunctionDeclarationAndOperations)
	//	{
	//		// f(x) = 1 + x
	//		std::function<double(Point)> function1 = [](const Point& point) { return 1.0 + point.GetX(); };
	//		std::function<double(Point)> function2 = [](const Point& point) { return 2.0 + point.GetX(); };

	//		// Constructor
	//		FEFunction f1(function1);
	//		FEFunction f2(function2);

	//		// x = 1
	//		Point x = Point(1.0, 0.0, 0.0);
	//		double scalar = 3.0;

	//		// f1(1) = 1 + 1 = 2
	//		CAssert::AreEqual(f1(x), 2.0);

	//		// f2(1) = 2 + 1 = 3
	//		CAssert::AreEqual(f2(x), 3.0);

	//		// addedFunction(x) = 3 + 2x
	//		FEFunction addedFunction = f1 + f2;
	//		CAssert::AreEqual(addedFunction(x), 5.0);
	//		
	//		// subtractedFunction(x) = -1
	//		FEFunction subtractedFunction = f1 - f2;
	//		CAssert::AreEqual(subtractedFunction(x), -1.0);

	//		// multipliedFunction(x) = (1 + x)(2 + x)
	//		FEFunction multipliedFunction = f1 * f2;
	//		CAssert::AreEqual(multipliedFunction(x), 6.0);

	//		// scalarMultipliedFunction(x) = (1 + x) * 3
	//		FEFunction scalarMultipliedFunction = f1 * scalar;
	//		CAssert::AreEqual(scalarMultipliedFunction(x), 6.0);

	//		// leftScalarMultipliedFunction(x) = 3 * (1 + x)
	//		FEFunction leftScalarMultipliedFunction = scalar * f1;
	//		CAssert::AreEqual(leftScalarMultipliedFunction(x), 6.0);

	//		// dividedFunction(x) = (1 + x) / (2 + x)
	//		FEFunction dividedFunction = f1 / f2;
	//		CAssert::AreEqual(dividedFunction(x), 2.0 / 3.0);

	//		// scalarDividedFunction(x) = (1 + x) / 2
	//		FEFunction scalarDividedFunction = f1 / scalar;
	//		CAssert::AreEqual(scalarDividedFunction(x), 2.0 / 3.0);
	//	}
	//};

	//// FEIntegrator tests
	//TEST_CLASS(FEIntegratorTests)
	//{
	//public:
	//	TEST_METHOD(MidpointMethod)
	//	{
	//		m_feIntegrator->SetIntegratorMethod(FEIntegrator::IntegratorMethod::eMidpoint);

	//		// \int_0^1 f1(x)dx = 1.5
	//		CAssert::AreEqual(m_feIntegrator->Integrate(m_f1, m_dLowerLimit, m_dUpperLimit, m_iDiscNum), 1.5, 1e-4);

	//		// \int_0^1 f2(x)dx = e - 1
	//		CAssert::AreEqual(m_feIntegrator->Integrate(m_f2, m_dLowerLimit, m_dUpperLimit, m_iDiscNum), std::exp(1.0) - 1.0, 1e-4);
	//	}

	//	TEST_METHOD(TrapezoidMethod)
	//	{
	//		m_feIntegrator->SetIntegratorMethod(FEIntegrator::IntegratorMethod::eTrapezoidal);

	//		// \int_0^1 f1(x)dx = 1.5
	//		CAssert::AreEqual(m_feIntegrator->Integrate(m_f1, m_dLowerLimit, m_dUpperLimit, m_iDiscNum), 1.5, 1e-4);

	//		// \int_0^1 f2(x)dx = e - 1
	//		CAssert::AreEqual(m_feIntegrator->Integrate(m_f2, m_dLowerLimit, m_dUpperLimit, m_iDiscNum), std::exp(1.0) - 1.0, 1e-4);
	//	}

	//	TEST_METHOD(Simpson13Method)
	//	{
	//		m_feIntegrator->SetIntegratorMethod(FEIntegrator::IntegratorMethod::eSimpson13);

	//		// \int_0^1 f1(x)dx = 1.5
	//		CAssert::AreEqual(m_feIntegrator->Integrate(m_f1, m_dLowerLimit, m_dUpperLimit, m_iDiscNum), 1.5, 1e-4);

	//		// \int_0^1 f2(x)dx = e - 1
	//		CAssert::AreEqual(m_feIntegrator->Integrate(m_f2, m_dLowerLimit, m_dUpperLimit, m_iDiscNum), std::exp(1.0) - 1.0, 1e-4);
	//	}

	//	TEST_METHOD(Simpson38Method)
	//	{
	//		m_feIntegrator->SetIntegratorMethod(FEIntegrator::IntegratorMethod::eSimpson38);

	//		// \int_0^1 f1(x)dx = 1.5
	//		CAssert::AreEqual(m_feIntegrator->Integrate(m_f1, m_dLowerLimit, m_dUpperLimit, m_iDiscNum + 1), 1.5, 1e-4);

	//		// \int_0^1 f2(x)dx = e - 1
	//		CAssert::AreEqual(m_feIntegrator->Integrate(m_f2, m_dLowerLimit, m_dUpperLimit, m_iDiscNum + 1), std::exp(1.0) - 1.0, 1e-4);
	//	}

	//	TEST_METHOD(GaussQuadratureMethod)
	//	{
	//		m_feIntegrator->SetIntegratorMethod(FEIntegrator::IntegratorMethod::eGauss);

	//		// \int_0^1 f1(x)dx = 1.5
	//		CAssert::AreEqual(m_feIntegrator->Integrate(m_f1, m_dLowerLimit, m_dUpperLimit, 5), 1.5, 1e-4);

	//		// \int_0^1 f2(x)dx = e - 1
	//		CAssert::AreEqual(m_feIntegrator->Integrate(m_f2, m_dLowerLimit, m_dUpperLimit, 5), std::exp(1.0) - 1.0, 1e-4);
	//	}

	//private:
	//	std::unique_ptr<FEIntegrator> m_feIntegrator = std::make_unique<FEIntegrator>();
	//	
	//	// f(x) = 1 + x
	//	FEFunction m_f1 = FEFunction([](const Point& point) { return 1.0 + point.GetX(); });
	//	
	//	// f(x) = exp(x)
	//	FEFunction m_f2 = FEFunction([](const Point& point) { return std::exp(point.GetX()); });

	//	// Integration parameters
	//	const double m_dLowerLimit = 0.0;
	//	const double m_dUpperLimit = 1.0;
	//	const int m_iDiscNum = 50;
	//};
}
