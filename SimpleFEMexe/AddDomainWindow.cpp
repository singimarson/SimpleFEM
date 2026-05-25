#include "AddDomainWindow.h"
#include "FEDomain.h"
#include "imgui.h"
#include "Point.h"

// Shows the window for adding a domain
void FEWindow::ShowAddDomainWindow(FEWindow::MainWindowObjects& mainWindowObjects, const char* charAddDomain)
{
	if (!ImGui::GetCurrentContext() || charAddDomain != "Add Domain")
	{
		return;
	}

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal(charAddDomain, NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Select Dimension of Domain.");
		static int iDim = 1;
		ImGui::RadioButton("1D", &iDim, 1); ImGui::SameLine();
		ImGui::RadioButton("2D", &iDim, 2); ImGui::SameLine();
		ImGui::RadioButton("3D", &iDim, 3);
		ImGui::Separator();

		static char charDomainName[128] = "Untitled Domain Name";
		ImGui::InputText("input text", charDomainName, IM_COUNTOF(charDomainName));


		// This will need to change when 2D-3D is implemented, but for now it's just 1D 
		static double dX0 = 0.0, dX1 = 1.0;
		ImGui::InputDouble("Left Domain Boundary", &dX0, 0.01f, 1.0f, "%.8f");
		ImGui::InputDouble("Right Domain Boundary", &dX1, 0.01f, 1.0f, "%.8f");


		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0))) 
		{
			// Can probably be streamlined to >1 dimension, but meh for rn
			if (iDim == 1)
			{
				Point<1> ptX0(dX0), ptX1(dX1);
				FEDomain<1>* domain = new FEDomain<1>({ ptX0, ptX1 });
				
				std::string strName = static_cast<std::string>(charDomainName);
				domain->SetName(static_cast<std::string>(charDomainName));

				mainWindowObjects.vDomains1D.push_back(domain);

				// This is scary right now, this'll need touched when deleting objects starts up
				mainWindowObjects.mDisplayedDomains[static_cast<int>(mainWindowObjects.mDisplayedDomains.size())] = domain->GetID();
			}
			ImGui::CloseCurrentPopup(); 
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}

	ImGui::EndPopup();
}
