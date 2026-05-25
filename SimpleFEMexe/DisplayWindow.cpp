#include "DisplayWindow.h"
#include "imgui.h"
#include "implot.h"
#include "Point.h"

// Custom style for plot to display mesh
void FEWindow::CustomGraphStyleFor1DMesh()
{
    ImPlotStyle& style = ImPlot::GetStyle();

    //ImVec4* colors = style.Colors;
    //colors[ImPlotCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    //colors[ImPlotCol_PlotBg] = ImVec4(0.92f, 0.92f, 0.95f, 1.00f);
    //colors[ImPlotCol_PlotBorder] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    //colors[ImPlotCol_LegendBg] = ImVec4(0.92f, 0.92f, 0.95f, 1.00f);
    //colors[ImPlotCol_LegendBorder] = ImVec4(0.80f, 0.81f, 0.85f, 1.00f);
    //colors[ImPlotCol_LegendText] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    //colors[ImPlotCol_TitleText] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    //colors[ImPlotCol_InlayText] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    //colors[ImPlotCol_AxisText] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    //colors[ImPlotCol_AxisGrid] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    //colors[ImPlotCol_AxisBgHovered] = ImVec4(0.92f, 0.92f, 0.95f, 1.00f);
    //colors[ImPlotCol_AxisBgActive] = ImVec4(0.92f, 0.92f, 0.95f, 0.75f);
    //colors[ImPlotCol_Selection] = ImVec4(1.00f, 0.65f, 0.00f, 1.00f);
    //colors[ImPlotCol_Crosshairs] = ImVec4(0.23f, 0.10f, 0.64f, 0.50f);

    style.MousePosPadding = ImVec2(5, 5);
    style.PlotMinSize = ImVec2(300, 225);
    style.PlotBorderSize = 0;
    style.MinorAlpha = 1.0f;
    style.MajorTickLen = ImVec2(0, 0);
    style.MinorTickLen = ImVec2(0, 0);
    style.MajorTickSize = ImVec2(0, 0);
    style.MinorTickSize = ImVec2(0, 0);
    style.MajorGridSize = ImVec2(1.2f, 1.2f);
    style.MinorGridSize = ImVec2(1.2f, 1.2f);
    style.PlotPadding = ImVec2(12, 12);
    style.LabelPadding = ImVec2(5, 5);
    style.LegendPadding = ImVec2(5, 5);
    style.DigitalPadding = 20;
    style.DigitalSpacing = 4;
}

// Main function for displaying the domain/mesh
void FEWindow::ShowDisplayWindow(MainWindowObjects& mainWindowObjects, const int iNodeClicked)
{
	if (!ImGui::GetCurrentContext())
	{
		return;
	}

	const ImGuiViewport* pViewport = ImGui::GetMainViewport();
	const ImVec2 vBasePos = pViewport->Pos;
	const ImVec2 vWorkSize = pViewport->WorkSize;

	ImGui::SetNextWindowPos(ImVec2(vBasePos.x + vWorkSize.x * 0.3f, vBasePos.y + 20.0f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(vWorkSize.x * 0.7f, vWorkSize.y * 0.6f), ImGuiCond_Always);

	ImGuiWindowFlags fWindowFlags = 0;
	fWindowFlags |= ImGuiWindowFlags_NoMove;
	fWindowFlags |= ImGuiWindowFlags_NoResize;
	fWindowFlags |= ImGuiWindowFlags_NoCollapse;

	if (!ImGui::Begin("Display Window", nullptr, fWindowFlags) && iNodeClicked != -1)
	{
		ImGui::End();
		return;
	}

    FEDomain<1>* domain = mainWindowObjects.FindDisplayedDomain(iNodeClicked);
    if (!domain)
    {
        ImGui::End();
        return;
    }

    std::string strPlotName = domain->GetName() + " Domain";

    //CustomGraphStyleFor1DMesh();
	if (ImPlot::BeginPlot(strPlotName.c_str(), ImVec2(-1, 0), ImPlotFlags_NoLegend))
	{
        ImPlotAxisFlags eXAxisFlags = ImPlotAxisFlags_NoTickMarks;
        ImPlotAxisFlags eYAxisFlags = ImPlotAxisFlags_NoDecorations;
        ImPlot::SetupAxis(ImAxis_X1, "", eXAxisFlags);
        ImPlot::SetupAxis(ImAxis_Y1, "", eYAxisFlags);
        
        std::vector<Point<1>> vDomainPoints = domain->GetDomainOutline();

        double dEndpoint1 = vDomainPoints[0][0];
        double dEndpoint2 = vDomainPoints[1][0];

        ImPlot::SetupAxisLimits(ImAxis_X1, dEndpoint1 - 0.1, dEndpoint2 + 0.1);
        ImPlot::SetupAxisLimits(ImAxis_Y1, -0.5, 0.5);

		double x[2] = { dEndpoint1, dEndpoint2 };
        double y[2] = { 0.0, 0.0 }; // <- This is just for plotting purposes
        ImPlot::PlotLine(domain->GetName().c_str(), x, y, 2, {ImPlotProp_Marker, ImPlotMarker_Circle});
		ImPlot::EndPlot();
	}

	ImGui::End();
}
