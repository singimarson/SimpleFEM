#include "ComponentWindow.h"
#include "imgui.h"
#include "MainWindow.h"

// Main function for showing the main SimpleFEM window
void FEWindow::ShowMainWindow(MainWindowObjects& mainWindowObjects)
{
	if (!ImGui::GetCurrentContext())
	{
		return;
	}

	const ImGuiViewport* pViewport = ImGui::GetMainViewport();
	static bool use_work_area = true;

	ImGui::SetNextWindowPos(use_work_area ? pViewport->WorkPos : pViewport->Pos);
	ImGui::SetNextWindowSize(use_work_area ? pViewport->WorkSize : pViewport->Size);

	// Window flags
	ImGuiWindowFlags fWindowFlags = 0;
	fWindowFlags |= ImGuiWindowFlags_NoTitleBar;
	fWindowFlags |= ImGuiWindowFlags_MenuBar;
	fWindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	fWindowFlags |= ImGuiWindowFlags_NoMove;
	fWindowFlags |= ImGuiWindowFlags_NoResize;
	fWindowFlags |= ImGuiWindowFlags_NoCollapse;
	
	// Uncomment this when we're off to the races.
	//fWindowFlags |= ImGuiWindowFlags_NoTitleBar;

	// Todo: Not sure how the colors here work but we need to statically set the colors or allow users
	// to adjust colors. Maybe slap it in the options menu or something

	// Next time: Start making the windows containing the domain/mesh making and parameters, etc.

	if (!ImGui::Begin("SimpleFEM Main Window", nullptr, fWindowFlags))
	{
		ImGui::End();
		return;
	}

	// Show menu items
	ShowMainWindowMainMenuBar();

	// Show Component Window
	ShowComponentWindow(mainWindowObjects);


	ImGui::End();
}

// Menu item in the main window bar
void FEWindow::ShowMainWindowMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			ShowMainWindowMenuBarFile();
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

// The file items for the menu bar
void FEWindow::ShowMainWindowMenuBarFile()
{
	ImGui::MenuItem("File Menu", nullptr, false, false);
	if (ImGui::MenuItem("New")) {}
	if (ImGui::MenuItem("Open", "Ctrl+O")) {}
	if (ImGui::BeginMenu("Open Recent")) 
	{
		ImGui::EndMenu();
	}

	ImGui::Separator();


}
