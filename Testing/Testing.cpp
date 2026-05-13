#include "pch.h"
#include "CppUnitTest.h"

#include "CAssert.h"

#include "../SimpleFEM/Constants.h"
#include "../SimpleFEM/Point.h"
#include "../SimpleFEM/FEFunction.h"
#include "../SimpleFEM/FEMesh.h"
#include "../SimpleFEM/FEDomain.h"
#include "../SimpleFEM/FEIntegrator.h"
#include "../SimpleFEM/FEMatrix.h"
#include "../SimpleFEM/FESolver.h"
#include "../SimpleFEM/FESquareMatrix.h"
#include "../SimpleFEM/FETypes.h"
#include "../SimpleFEM/FEVector.h"
#include "../SimpleFEM/MeshCreation.h"
#include "../SimpleFEM/SystemBuilder.h"

#include <cmath>
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

		TEST_METHOD(PointOperations)
		{
			Point<3> ptA(2.3, 1.1, 8.2);
			Point<3> ptB(4.4, 2.1, 0.1);
			Point<3> ptSum(6.7, 3.2, 8.3);
			Point<3> ptDiff(2.1, 1.0, -8.1);

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
			const std::size_t dim = 1;
			FEDomain<dim>* m_pDomain = new FEDomain<dim>();

			Point<dim> ptA(0.0);
			Point<dim> ptB(1.0);
			std::vector<Point<dim>> vDomainPoints = { ptA, ptB };
			m_pDomain->SetDomainOutline(vDomainPoints);
			const std::vector<Point<dim>> vTestDomainPoints = m_pDomain->GetDomainOutline();

			CAssert::AreEqual(vDomainPoints.size(), vTestDomainPoints.size());
			for (std::size_t ptIndex = 0; ptIndex < vTestDomainPoints.size(); ++ptIndex)
			{
				CAssert::AreEqual(vDomainPoints[ptIndex][0], vTestDomainPoints[ptIndex][0]);
			}

			delete m_pDomain;
		}
	};

	// FEMesh tests
	TEST_CLASS(MeshCreatorTests)
	{
	public:
		TEST_METHOD(MeshCreation)
		{
			// Create the finite element domain
			const int dim = 1;
			std::vector<Point<dim>> m_vEndpoints = { Point<dim>(), Point<dim>(1.0) };
			std::unique_ptr<FEDomain<dim>> m_pDomain = std::make_unique<FEDomain<dim>>(m_vEndpoints);

			// Initialize the mesh class, uniform by default
			std::unique_ptr<FEMesh<dim>> m_pMesh = std::make_unique<FEMesh<dim>>(m_pDomain.get());

			// MeshCreator class
			const int iElementNumber = 2;
			MeshCreation::CreateUniformMesh(m_pMesh.get(), iElementNumber);

			// We expect the mesh with 2 elements to look like this:
			//
			// |--------------|--------------|
			// 0             0.5             1
			//
			// Now we test what MeshCreator gives us

			std::unordered_map<int, Point<dim>> mMeshNodeTest;
			mMeshNodeTest[0] = Point<dim>();
			mMeshNodeTest[1] = Point<dim>(0.5);
			mMeshNodeTest[2] = Point<dim>(1.0);

			std::unordered_map<int, Point<dim>> mMeshNodeResult = m_pMesh->GetNodes();

			// These aren't equal, fix the MeshCreator
			CAssert::AreEqual(mMeshNodeTest.size(), mMeshNodeResult.size());
			for (int iMeshIndex = 0; iMeshIndex < mMeshNodeTest.size(); ++iMeshIndex)
			{
				CAssert::AreEqual(mMeshNodeTest[iMeshIndex], mMeshNodeResult[iMeshIndex], dTolerance);
			}

			// Boundary nodes
			std::vector<int> vExpectedBC({ 0, 2 });
			CAssert::AreEqual(vExpectedBC, m_pMesh->GetBoundaryNodes());
		}
	};

	// FEFunction tests
	TEST_CLASS(FEFunctionTests)
	{
	public:
		TEST_METHOD(FunctionDeclarationAndOperations)
		{
			const std::size_t dim = 1;

			// f(x) = 1 + x
			std::function<double(Point<dim>)> function1 = [](const Point<dim>& point) { return 1.0 + point[0]; };
			std::function<double(Point<dim>)> function2 = [](const Point<dim>& point) { return 2.0 + point[0]; };
			// Constructor
			FEFunction<dim> f1(function1);
			FEFunction<dim> f2(function2);

			// x = 1
			Point<dim> x = Point<dim>(1.0);
			double scalar = 3.0;

			// f1(1) = 1 + 1 = 2
			CAssert::AreEqual(f1(x), 2.0);

			// f2(1) = 2 + 1 = 3
			CAssert::AreEqual(f2(x), 3.0);

			// addedFunction(x) = 3 + 2x
			FEFunction<dim> addedFunction = f1 + f2;
			CAssert::AreEqual(addedFunction(x), 5.0);
			
			// subtractedFunction(x) = -1
			FEFunction<dim> subtractedFunction = f1 - f2;
			CAssert::AreEqual(subtractedFunction(x), -1.0);

			// multipliedFunction(x) = (1 + x)(2 + x)
			FEFunction<dim> multipliedFunction = f1 * f2;
			CAssert::AreEqual(multipliedFunction(x), 6.0);

			// scalarMultipliedFunction(x) = (1 + x) * 3
			FEFunction<dim> scalarMultipliedFunction = f1 * scalar;
			CAssert::AreEqual(scalarMultipliedFunction(x), 6.0);

			// leftScalarMultipliedFunction(x) = 3 * (1 + x)
			FEFunction<dim> leftScalarMultipliedFunction = scalar * f1;
			CAssert::AreEqual(leftScalarMultipliedFunction(x), 6.0);

			// dividedFunction(x) = (1 + x) / (2 + x)
			FEFunction<dim> dividedFunction = f1 / f2;
			CAssert::AreEqual(dividedFunction(x), 2.0 / 3.0);

			// scalarDividedFunction(x) = (1 + x) / 2
			FEFunction<dim> scalarDividedFunction = f1 / scalar;
			CAssert::AreEqual(scalarDividedFunction(x), 2.0 / 3.0);
		}
	};

	// FEIntegrator tests
	TEST_CLASS(FEIntegratorTests)
	{
	public:
		TEST_METHOD(MidpointMethod)
		{
			m_feIntegrator->SetIntegratorMethod(FEIntegrator<1>::IntegratorMethod::eMidpoint);
			
			// \int_0^1 f1(x)dx = 1.5
			CAssert::AreEqual(m_feIntegrator->Integrate(m_f1, m_dLowerLimit, m_dUpperLimit, m_iDiscNum), 1.5, 1e-4);

			// \int_0^1 f2(x)dx = e - 1
			CAssert::AreEqual(m_feIntegrator->Integrate(m_f2, m_dLowerLimit, m_dUpperLimit, m_iDiscNum), std::exp(1.0) - 1.0, 1e-4);
		}

		TEST_METHOD(TrapezoidMethod)
		{
			m_feIntegrator->SetIntegratorMethod(FEIntegrator<1>::IntegratorMethod::eTrapezoidal);

			// \int_0^1 f1(x)dx = 1.5
			CAssert::AreEqual(m_feIntegrator->Integrate(m_f1, m_dLowerLimit, m_dUpperLimit, m_iDiscNum), 1.5, 1e-4);

			// \int_0^1 f2(x)dx = e - 1
			CAssert::AreEqual(m_feIntegrator->Integrate(m_f2, m_dLowerLimit, m_dUpperLimit, m_iDiscNum), std::exp(1.0) - 1.0, 1e-4);
		}

		TEST_METHOD(Simpson13Method)
		{
			m_feIntegrator->SetIntegratorMethod(FEIntegrator<1>::IntegratorMethod::eSimpson13);

			// \int_0^1 f1(x)dx = 1.5
			CAssert::AreEqual(m_feIntegrator->Integrate(m_f1, m_dLowerLimit, m_dUpperLimit, m_iDiscNum), 1.5, 1e-4);

			// \int_0^1 f2(x)dx = e - 1
			CAssert::AreEqual(m_feIntegrator->Integrate(m_f2, m_dLowerLimit, m_dUpperLimit, m_iDiscNum), std::exp(1.0) - 1.0, 1e-4);
		}

		TEST_METHOD(Simpson38Method)
		{
			m_feIntegrator->SetIntegratorMethod(FEIntegrator<1>::IntegratorMethod::eSimpson38);

			// \int_0^1 f1(x)dx = 1.5
			CAssert::AreEqual(m_feIntegrator->Integrate(m_f1, m_dLowerLimit, m_dUpperLimit, m_iDiscNum + 1), 1.5, 1e-4);

			// \int_0^1 f2(x)dx = e - 1
			CAssert::AreEqual(m_feIntegrator->Integrate(m_f2, m_dLowerLimit, m_dUpperLimit, m_iDiscNum + 1), std::exp(1.0) - 1.0, 1e-4);
		}

		TEST_METHOD(GaussQuadratureMethod)
		{
			m_feIntegrator->SetIntegratorMethod(FEIntegrator<1>::IntegratorMethod::eGauss);

			// \int_0^1 f1(x)dx = 1.5
			CAssert::AreEqual(m_feIntegrator->Integrate(m_f1, m_dLowerLimit, m_dUpperLimit, 5), 1.5, 1e-4);

			// \int_0^1 f2(x)dx = e - 1
			CAssert::AreEqual(m_feIntegrator->Integrate(m_f2, m_dLowerLimit, m_dUpperLimit, 5), std::exp(1.0) - 1.0, 1e-4);
		}

	private:
		std::unique_ptr<FEIntegrator<1>> m_feIntegrator = std::make_unique<FEIntegrator<1>>();
		
		// f(x) = 1 + x
		FEFunction<1> m_f1 = FEFunction<1>([](const Point<1>& point) { return 1.0 + point[0]; });
		
		// f(x) = exp(x)
		FEFunction<1> m_f2 = FEFunction<1>([](const Point<1>& point) { return std::exp(point[0]); });

		// Integration parameters
		const double m_dLowerLimit = 0.0;
		const double m_dUpperLimit = 1.0;
		const int m_iDiscNum = 50;
	};

	// Linear algebra tests
	TEST_CLASS(FELinearAlgebraTests)
	{
	public:
		TEST_METHOD(VectorTests)
		{
			FEVector vec1({ 1.0, 2.0, 3.0 });
			FEVector vec2({ 4.0, 5.0, 6.0 });

			// Element accessing
			CAssert::AreEqual(vec1.Get(0), 1.0);
			CAssert::AreEqual(vec1[0], 1.0);
			CAssert::AreEqual(vec1.GetSize(), 3);

			// Operations
			double scalar = 3.0;
			FEVector sum({ 5.0, 7.0, 9.0 });
			FEVector diff({ 3.0, 3.0, 3.0 });
			FEVector mult({ 3.0, 6.0, 9.0 });
			CAssert::AreEqual(vec1 + vec2, sum);
			CAssert::AreEqual(vec2 - vec1, diff);
			CAssert::AreEqual(scalar * vec1, mult);

			// Vector operations
			CAssert::AreEqual(vec1.Norm(), std::sqrt(14.0));
			CAssert::AreEqual(vec1.DotProduct(vec2), 32.0);
		}

		TEST_METHOD(MatrixTests)
		{
			FEMatrix matrix1({ {1.0, 2.0, 3.0}, { 2.0, 3.0, 4.0 }, { 3.0, 4.0, 5.0 } });
			FEMatrix matrix2({ {1.0, 3.0, 5.0}, { 2.0, 4.0, 6.0 }, { 3.0, 6.0, 7.0 } });

			// matrix characteristics
			CAssert::AreEqual(matrix1.GetRowSize(), 3);
			CAssert::AreEqual(matrix1.GetColumnSize(), 3);

			// Getting vectors
			FEVector matrix1row2({ 2.0, 3.0, 4.0 });
			FEVector matrix2column3({ 5.0, 6.0, 7.0 });
			CAssert::AreEqual(matrix1.GetRow(1), matrix1row2);
			CAssert::AreEqual(matrix2.GetColumn(2), matrix2column3);

			// Operations
			FEMatrix sum({ {2.0, 5.0, 8.0}, { 4.0, 7.0, 10.0 }, { 6.0, 10.0, 12.0 } });
			FEMatrix diff({ {0.0, 1.0, 2.0}, { 0.0, 1.0, 2.0 }, { 0.0, 2.0, 2.0 } });
			CAssert::AreEqual(matrix1 + matrix2, sum);
			CAssert::AreEqual(matrix2 - matrix1, diff);

			// Setting rows and columns
			FEVector newRow({ 6.0, 6.0, 6.0 });
			FEVector newColumn({ 5.0, 5.0, 5.0 });
			FEMatrix matrix1NewRow2({ {1.0, 2.0, 3.0}, { 6.0, 6.0, 6.0 }, { 3.0, 4.0, 5.0 } });
			FEMatrix matrix2NewColumn3({ {1.0, 3.0, 5.0}, { 2.0, 4.0, 5.0 }, { 3.0, 6.0, 5.0 } });
			matrix1.SetRow(1, newRow);
			matrix2.SetColumn(2, newColumn);
			CAssert::AreEqual(matrix1NewRow2, matrix1);
			CAssert::AreEqual(matrix2NewColumn3, matrix2);
		}

		TEST_METHOD(LinearAlgebraOperations)
		{
			FEMatrix matrix1({ {1.0, 2.0, 3.0}, { 2.0, 3.0, 4.0 }, { 3.0, 4.0, 5.0 }, {4.0, 5.0, 6.0} });
			FEVector vector({ 1.0, 2.0, 3.0 });

			// Matrix Vector product
			FEVector vecProduct({ 14.0, 20.0, 26.0, 32.0 });
			CAssert::AreEqual(matrix1 * vector, vecProduct);

			// Matrix-matrix product
			FEMatrix matrix2({ { 1.0, 2.0, 3.0 }, { 5.0, 2.0, 3.0 }, {7.0, 8.0, 9.0} });
			FEMatrix matProduct({ {32.0, 30.0, 36.0}, {45.0, 42.0, 51.0}, {58.0, 54.0, 66.0}, {71.0, 66.0, 81.0} });
			CAssert::AreEqual(matrix1 * matrix2, matProduct);
		}
	};

	// Linear solving tests
	TEST_CLASS(FELinearSolverTests)
	{
	public:
		TEST_METHOD(LUDecomposition)
		{
			FESolver::DirectSolverMethod eMethod = FESolver::DirectSolverMethod::eLUDecomposition;
			FEVector x = FESolver::SolveSystemDirect(m_matrix1, m_vec, eMethod);

			CAssert::AreEqual(m_sol, x, 1e-7);
		}

	private:
		FESquareMatrix m_matrix1 = FESquareMatrix({ {2.0, -1.0, -2.0}, {-4.0, 6.0, 3.0}, {-4.0, -2.0, 8.0 } });
		FEVector m_vec = FEVector({ 1.0, 2.0, 2.0 });
		FEVector m_sol = FEVector({ 4.0, 5.0 / 3.0, 8.0 / 3.0 });
	};

	// System builder 1D tests
	TEST_CLASS(SystemBuilder1DTests)
	{
	public:
		// Mainly interested in the system
		// u''(x) = f(x), u(0) = u(1) = 0
		//
		// Consider u(x) = sin(2 * pi * x)
		// f(x) = -4 * pi^2 * sin(2 * pi * x)

		TEST_METHOD(MatrixBuildImplementationZeroOrder)
		{
			const int iElementNumber = 10;
			MeshCreation::CreateUniformMesh(m_pMesh.get(), iElementNumber);

			SystemBuilder sysBuilder;
			FESquareMatrix mSystemMatrix;
			FEVector vRHS;

			// Equation would be u(x) = f(x) (silly but this is for testing)
			m_tTerms.m_bD0 = true;
			m_tTerms.m_bD1 = false;

			// f(x) = -4 * pi^2 * sin(x)
			auto func = [](const Point<1>& point) {
				return -4.0 * M_PI_SQ * std::sin(2 * M_PI * point[0]);
				};
			FEFunction<1> rhsFunc(func);

			// Create system matrix
			// todo: rhs function testing
			std::tie(mSystemMatrix, vRHS) = sysBuilder.CreateSystem(m_pMesh.get(), m_tTerms, m_eOrder, rhsFunc, m_vBoundaryConditions);

			// This can be calculated rather easily with basic knowledge of finite element method
			FESquareMatrix mExpectedMatrix(iElementNumber + 1);
			for (int iMatrixIter = 1; iMatrixIter < mExpectedMatrix.GetRowSize() - 1; ++iMatrixIter)
			{
				mExpectedMatrix.Set(iMatrixIter, iMatrixIter, 1.0 / 15.0);
				if (iMatrixIter < mExpectedMatrix.GetRowSize() - 2)
					mExpectedMatrix.Set(iMatrixIter, iMatrixIter + 1, 1.0 / 60.0);
				if (iMatrixIter > 1)
					mExpectedMatrix.Set(iMatrixIter, iMatrixIter - 1, 1.0 / 60.0);
			}
			mExpectedMatrix.Set(0, 0, 1.0);
			mExpectedMatrix.Set(mExpectedMatrix.GetRowSize() - 1, mExpectedMatrix.GetRowSize() - 1, 1.0);

			CAssert::AreEqual(mExpectedMatrix, mSystemMatrix, 1e-5);
			CAssert::AreEqual(m_vExpectedVector, vRHS, 1e-5);
		}

		TEST_METHOD(MatrixBuildImplementationFirstOrder)
		{
			const int iElementNumber = 10;
			MeshCreation::CreateUniformMesh(m_pMesh.get(), iElementNumber);

			SystemBuilder sysBuilder;
			FESquareMatrix mSystemMatrix;
			FEVector vRHS;

			// Equation would be u'(x) = f(x)
			m_tTerms.m_bD0 = false;
			m_tTerms.m_bD1 = true;

			// f(x) = -4 * pi^2 * sin(x)
			FEFunction<1> rhsFunc([](const Point<1>& point) { return -4.0 * M_PI_SQ * std::sin(2.0 * M_PI * point[0]); });

			// Create system matrix NEEDS WORK CURRENTLY BROKEN WITH BOUNDARY CONDITIONS
			std::tie(mSystemMatrix, vRHS) = sysBuilder.CreateSystem(m_pMesh.get(), m_tTerms, m_eOrder, rhsFunc, m_vBoundaryConditions);

			// This can be calculated rather easily with basic knowledge of finite element method
			FESquareMatrix mExpectedMatrix(iElementNumber + 1);
			for (int iMatrixIter = 1; iMatrixIter < mExpectedMatrix.GetRowSize() - 1; ++iMatrixIter)
			{
				mExpectedMatrix.Set(iMatrixIter, iMatrixIter, 20);
				if (iMatrixIter < mExpectedMatrix.GetRowSize() - 2)
					mExpectedMatrix.Set(iMatrixIter, iMatrixIter + 1, -10.0);
				if (iMatrixIter > 1)
					mExpectedMatrix.Set(iMatrixIter, iMatrixIter - 1, -10.0);
			}
			mExpectedMatrix.Set(0, 0, 1.0);
			mExpectedMatrix.Set(mExpectedMatrix.GetRowSize() - 1, mExpectedMatrix.GetRowSize() - 1, 1.0);

			CAssert::AreEqual(mExpectedMatrix, mSystemMatrix, 1e-5);
			CAssert::AreEqual(m_vExpectedVector, vRHS, 1e-5);
		}

	private:
		const int m_iDim = 1;
		std::vector<Point<1>> m_vEndpoints = { Point<1>(), Point<1>(1.0) };
		std::unique_ptr<FEDomain<1>> m_pDomain = std::make_unique<FEDomain<1>>(m_vEndpoints);
		std::unique_ptr<FEMesh<1>> m_pMesh = std::make_unique<FEMesh<1>>(m_pDomain.get());
		std::vector<double> m_vBoundaryConditions = { 0.0, 0.0 };
		
		// These values were verified in Symbolab
		FEVector m_vExpectedVector = FEVector({ 0.0,
												-2.2451398709,
												-3.6327126207,
												-3.6327126207,
												-2.2451398710,
												0.0,
												2.2451398710,
												3.6327126207,
												3.6327126207,
												2.2451398709,
												0.0 });

		SystemBuilder::EquationTerms m_tTerms;
		SystemBuilder::ElementOrder m_eOrder = SystemBuilder::ElementOrder::eLinear;
	};

	// Finite element solve tests (method of manufactured solutions
	TEST_CLASS(FiniteElementSolve1DTests)
	{
	public:
		TEST_METHOD(FEMSolve)
		{
			const int iElementNumber = 10;
			MeshCreation::CreateUniformMesh(m_pMesh.get(), iElementNumber);

			SystemBuilder sysBuilder;
			FESquareMatrix mSystemMatrix;
			FEVector vRHS;

			// Equation would be -(u'(x), v'(x)) = (f(x), v(x))
			m_tTerms.m_bD0 = false;
			m_tTerms.m_bD1 = true;

			// f(x) = -4 * pi^2 * sin(x)
			// For the sake of the FEM solve itself, it's easier to set this function to be negative
			// So what gets pumped into the code is -f(x) here.
			FEFunction<1> rhsFunc([](const Point<1>& point) { return 4.0 * M_PI_SQ * std::sin(2.0 * M_PI * point[0]); });

			// Create system matrix
			std::tie(mSystemMatrix, vRHS) = sysBuilder.CreateSystem(m_pMesh.get(), m_tTerms, m_eOrder, rhsFunc, m_vBoundaryConditions);

			// Linear solve WRONG RN
			FESolver::DirectSolverMethod eMethod = FESolver::DirectSolverMethod::eLUDecomposition;
			FEVector x = FESolver::SolveSystemDirect(mSystemMatrix, vRHS, eMethod);

			// Now calculate the actual solution on the mesh
			std::vector<double> vSolution;
			for (int iNodeIter = 0; iNodeIter < iElementNumber + 1; ++iNodeIter)
			{
				double dNodeValue = m_vEndpoints[0][0] + static_cast<double>(iNodeIter) * (m_vEndpoints[1][0] - m_vEndpoints[0][0]) / static_cast<double>(iElementNumber);
				vSolution.push_back(std::sin(2.0 * M_PI * dNodeValue));
			}
			FEVector expected(vSolution);

			CAssert::AreEqual(x.L2Error(expected), 0.0, 1e-5);
		}

	private:
		const int m_iDim = 1;
		std::vector<Point<1>> m_vEndpoints = { Point<1>(), Point<1>(1.0) };
		std::unique_ptr<FEDomain<1>> m_pDomain = std::make_unique<FEDomain<1>>(m_vEndpoints);
		std::unique_ptr<FEMesh<1>> m_pMesh = std::make_unique<FEMesh<1>>(m_pDomain.get());
		std::vector<double> m_vBoundaryConditions = { 0.0, 0.0 };

		SystemBuilder::EquationTerms m_tTerms;
		SystemBuilder::ElementOrder m_eOrder = SystemBuilder::ElementOrder::eLinear;
	};
}
