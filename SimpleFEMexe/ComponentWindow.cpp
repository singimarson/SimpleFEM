#include "AddDomainWindow.h"
#include "imgui.h"
#include "ComponentWindow.h"

// Component window, lists domains/meshes/objects, etc.
void FEWindow::ShowComponentWindow(FEWindow::MainWindowObjects& mainWindowObjects, int& iSelectedDomain)
{
	if (!ImGui::GetCurrentContext())
	{
		return;
	}

	const ImGuiViewport* pViewport = ImGui::GetMainViewport();
	const ImVec2 vBasePos = pViewport->Pos;
	const ImVec2 vWorkSize = pViewport->WorkSize;

	ImGui::SetNextWindowPos(ImVec2(vBasePos.x, vBasePos.y + 20.0f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(vWorkSize.x * 0.3f, vWorkSize.y * 0.6f), ImGuiCond_Always);

	ImGuiWindowFlags fWindowFlags = 0;
	fWindowFlags |= ImGuiWindowFlags_NoMove;
	fWindowFlags |= ImGuiWindowFlags_NoResize;
	fWindowFlags |= ImGuiWindowFlags_NoCollapse;

	if (!ImGui::Begin("Component Overview", nullptr, fWindowFlags))
	{
		ImGui::End();
		return;
	}

	int iNodeClicked = -1;

	// Display list of meshes
	// Next need to list the actual components.
	if (ImGui::CollapsingHeader("Domains & Meshes"))
	{
		if (ImGui::BeginPopupContextItem())
		{
			const char* cAddDomain = "Add Domain";
			if (ImGui::Button(cAddDomain))
				ImGui::OpenPopup(cAddDomain);

			ShowAddDomainWindow(mainWindowObjects, cAddDomain);
		}

		// Loop responsible for listing the domains
		// Will need to do something with the multi-dimension aspect, later problem
		static ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_SpanAvailWidth | 
			ImGuiTreeNodeFlags_DrawLinesFull | 
			ImGuiTreeNodeFlags_DrawLinesToNodes |
			ImGuiTreeNodeFlags_OpenOnArrow |
			ImGuiTreeNodeFlags_OpenOnDoubleClick;
		static int iSelectionMask = (1 << 2);

		for (int iNodeIter = 0; iNodeIter < mainWindowObjects.vDomains1D.size(); ++iNodeIter)
		{
			ImGuiTreeNodeFlags nodeFlags = baseFlags;
			const bool bIsSelected = (iSelectionMask & (1 << iNodeIter)) != 0;
			if (bIsSelected)
			{
				nodeFlags |= ImGuiTreeNodeFlags_Selected;
			}

			if (ImGui::TreeNodeEx(mainWindowObjects.vDomains1D[iNodeIter]->GetName().c_str(), nodeFlags))
			{
				if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
				{
					iNodeClicked = iNodeIter;
				}
				ImGui::TreePop();
			}
		}

		// Node selection
		if (iNodeClicked != -1)
		{
			iSelectionMask = (1 << iNodeClicked);           // Click to single-select
			iSelectedDomain = iNodeClicked;
		}
	}

	ImGui::End();
}
