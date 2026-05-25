#pragma once
#include "FEDomain.h"
#include "FEMesh.h"
#include "FEObject.h"

#include <unordered_map>
#include <vector>

namespace FEWindow
{

struct MainWindowObjects
{
	std::unordered_map<int, GUID> mDisplayedDomains;

	std::vector<FEDomain<1>*> vDomains1D;
	std::vector<FEDomain<2>*> vDomains2D;
	std::vector<FEDomain<3>*> vDomains3D;

	std::vector<FEMesh<1>*> vMeshes1D;
	std::vector<FEMesh<2>*> vMeshes2D;
	std::vector<FEMesh<3>*> vMeshes3D;

	~MainWindowObjects()
	{
		for (auto domain : vDomains1D)
		{
			delete domain;
		}

		for (auto domain : vDomains2D)
		{
			delete domain;
		}

		for (auto domain : vDomains3D)
		{
			delete domain;
		}

		for (auto mesh : vMeshes1D)
		{
			delete mesh;
		}

		for (auto mesh : vMeshes2D)
		{
			delete mesh;
		}

		for (auto mesh : vMeshes3D)
		{
			delete mesh;
		}
	}

	// Sift through the objects and return the object of matching guid
	// We will need to get creative once 2D/3D get implemented.
	FEDomain<1>* FindDisplayedDomain(const int iDisplayNumber)
	{
		if (mDisplayedDomains.find(iDisplayNumber) == mDisplayedDomains.end())
		{
			return nullptr;
		}

		GUID guid = mDisplayedDomains[iDisplayNumber];
		for (const auto& domain : vDomains1D)
		{
			if (guid == domain->GetID())
			{
				return domain;
			}
		}

		return nullptr;
	}
};

void ShowMainWindow(MainWindowObjects& mainWindowObjects, int& iSelectedDomain);

static void ShowMainWindowMainMenuBar();
static void ShowMainWindowMenuBarFile();

} // namespace MainWindow