#pragma once
#include "FEDomain.h"

#include <vector>

namespace FEWindow
{

struct MainWindowObjects
{
	std::vector<FEDomain<1>*> vDomains1D;
	std::vector<FEDomain<2>*> vDomains2D;
	std::vector<FEDomain<3>*> vDomains3D;

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
	}
};

void ShowMainWindow(MainWindowObjects& mainWindowObjects);

static void ShowMainWindowMainMenuBar();
static void ShowMainWindowMenuBarFile();

} // namespace MainWindow