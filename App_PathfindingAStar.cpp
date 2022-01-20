//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

//Includes
#include "App_PathfindingAStar.h"
#include "framework\EliteAI\EliteGraphs\EliteGraphAlgorithms\EAstar.h"
#include "framework\EliteAI\EliteGraphs\EliteGraphAlgorithms\EBFS.h"

using namespace Elite;

//Destructor
App_PathfindingAStar::~App_PathfindingAStar()
{
	SAFE_DELETE(m_pGridGraph);
}

//Functions
void App_PathfindingAStar::Start()
{
	//Set Camera
	DEBUGRENDERER2D->GetActiveCamera()->SetZoom(39.0f);
	DEBUGRENDERER2D->GetActiveCamera()->SetCenter(Elite::Vector2(73.0f, 35.0f));

	//Create Graph
	MakeGridGraph();

	//Setup default start path
	startPathIdx = 0;
	endPathIdx = ROWS * COLUMNS - 1;
	CalculatePath();
}

void App_PathfindingAStar::Update(float deltaTime)
{
	UNREFERENCED_PARAMETER(deltaTime);
	
	//INPUT
	bool const middleMousePressed = INPUTMANAGER->IsMouseButtonUp(InputMouseButton::eMiddle);
	bool const leftMousePressed = INPUTMANAGER->IsMouseButtonUp(InputMouseButton::eLeft);
	if (middleMousePressed)
	{
		MouseData mouseData = { INPUTMANAGER->GetMouseData(Elite::InputType::eMouseButton, Elite::InputMouseButton::eMiddle) };
		Elite::Vector2 mousePos = DEBUGRENDERER2D->GetActiveCamera()->ConvertScreenToWorld({ (float)mouseData.X, (float)mouseData.Y });

		//Find closest node to click pos
		int closestNode = m_pGridGraph->GetNodeIdxAtWorldPos(mousePos);
		if (m_StartSelected)
		{
			startPathIdx = closestNode;
			CalculatePath();
		}
		else
		{
			endPathIdx = closestNode;
			CalculatePath();
		}
	}

	if (leftMousePressed)
	{
		MakeGridGraph();
	}

	MakeMaze();

	MakeMazeSolid();

	//IMGUI
	UpdateImGui();

	//UPDATE/CHECK GRID HAS CHANGED
	if (leftMousePressed)
	{
		CalculatePath();
	}
}

void App_PathfindingAStar::Render(float deltaTime) const
{
	UNREFERENCED_PARAMETER(deltaTime);
	//Render grid
	m_GraphRenderer.RenderGraph(
		m_pGridGraph,
		m_bDrawGrid,
		m_bDrawNodeNumbers,
		m_bDrawConnections,
		m_bDrawConnectionsCosts
	);

	//Render start node on top if applicable
	if (startPathIdx != invalid_node_index)
	{
		m_GraphRenderer.HighlightNodes(m_pGridGraph, { m_pGridGraph->GetNode(startPathIdx) }, START_NODE_COLOR);
	}

	//Render end node on top if applicable
	if (endPathIdx != invalid_node_index)
	{
		m_GraphRenderer.HighlightNodes(m_pGridGraph, { m_pGridGraph->GetNode(endPathIdx) }, END_NODE_COLOR);
	}

	//render path below if applicable
	if (m_vPath.size() > 0)
	{
		m_GraphRenderer.HighlightNodes(m_pGridGraph, m_vPath);
	}

}

void App_PathfindingAStar::MakeGridGraph()
{
	m_pGridGraph = new GridGraph<GridTerrainNode, GraphConnection>(COLUMNS, ROWS, m_SizeCell, false, false, 1.f, 1.5f);

	//Setup default terrain
	//m_pGridGraph->GetNode(86)->SetTerrainType(TerrainType::Water);
	//m_pGridGraph->GetNode(66)->SetTerrainType(TerrainType::Water);
	//m_pGridGraph->GetNode(67)->SetTerrainType(TerrainType::Water);
	//m_pGridGraph->GetNode(47)->SetTerrainType(TerrainType::Water);
	//m_pGridGraph->RemoveConnectionsToAdjacentNodes(86);
	//m_pGridGraph->RemoveConnectionsToAdjacentNodes(66);
	//m_pGridGraph->RemoveConnectionsToAdjacentNodes(67);
	//m_pGridGraph->RemoveConnectionsToAdjacentNodes(47);

	for (size_t i = 1; i < m_pGridGraph->GetAllNodes().size(); i++)
	{
		m_pGridGraph->GetNode(i)->SetTerrainType(TerrainType::Water);
	}

	for (auto connection : m_pGridGraph->GetNodeConnections(m_pGridGraph->GetNode(0)))
	{
		m_Walls.push_back(m_pGridGraph->GetNode(connection->GetTo()));
		m_WallOpenings[m_pGridGraph->GetNode(connection->GetTo())] += 1;
	}
	m_pGridGraph->RemoveConnectionsToAdjacentNodes(rand() % m_pGridGraph->GetAllNodes().size());
}

void App_PathfindingAStar::UpdateImGui()
{
#ifdef PLATFORM_WINDOWS
#pragma region UI
	//UI
	{
		//Setup
		int menuWidth = 115;
		int const width = DEBUGRENDERER2D->GetActiveCamera()->GetWidth();
		int const height = DEBUGRENDERER2D->GetActiveCamera()->GetHeight();
		bool windowActive = true;
		ImGui::SetNextWindowPos(ImVec2((float)width - menuWidth - 10, 10));
		ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)height - 20));
		ImGui::Begin("Gameplay Programming", &windowActive, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		ImGui::PushAllowKeyboardFocus(false);

		//Elements
		ImGui::Text("CONTROLS");
		ImGui::Indent();
		ImGui::Text("LMB: target");
		ImGui::Text("RMB: start");
		ImGui::Unindent();

		/*Spacing*/ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing(); ImGui::Spacing();

		ImGui::Text("STATS");
		ImGui::Indent();
		ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Unindent();

		/*Spacing*/ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing(); ImGui::Spacing();

		ImGui::Text("A* Pathfinding");
		ImGui::Spacing();

		ImGui::Text("Middle Mouse");
		ImGui::Text("controls");
		std::string buttonText{ "" };
		if (m_StartSelected)
			buttonText += "Start Node";
		else
			buttonText += "End Node";

		if (ImGui::Button(buttonText.c_str()))
		{
			m_StartSelected = !m_StartSelected;
		}

		ImGui::Checkbox("Grid", &m_bDrawGrid);
		ImGui::Checkbox("NodeNumbers", &m_bDrawNodeNumbers);
		ImGui::Checkbox("Connections", &m_bDrawConnections);
		ImGui::Checkbox("Connections Costs", &m_bDrawConnectionsCosts);
		if (ImGui::Combo("", &m_SelectedHeuristic, "Manhattan\0Euclidean\0SqrtEuclidean\0Octile\0Chebyshev", 4))
		{
			switch (m_SelectedHeuristic)
			{
			case 0:
				m_pHeuristicFunction = HeuristicFunctions::Manhattan;
				break;
			case 1:
				m_pHeuristicFunction = HeuristicFunctions::Euclidean;
				break;
			case 2:
				m_pHeuristicFunction = HeuristicFunctions::SqrtEuclidean;
				break;
			case 3:
				m_pHeuristicFunction = HeuristicFunctions::Octile;
				break;
			case 4:
				m_pHeuristicFunction = HeuristicFunctions::Chebyshev;
				break;
			default:
				m_pHeuristicFunction = HeuristicFunctions::Chebyshev;
				break;
			}
		}
		ImGui::Spacing();

		//End
		ImGui::PopAllowKeyboardFocus();
		ImGui::End();
	}
#pragma endregion
#endif
}

void App_PathfindingAStar::CalculatePath()
{
	//Check if valid start and end node exist
	if (startPathIdx != invalid_node_index
		&& endPathIdx != invalid_node_index
		&& startPathIdx != endPathIdx)
	{
		//BFS Pathfinding
		//auto pathfinder = BFS<GridTerrainNode, GraphConnection>(m_pGridGraph);
		auto pathfinder = AStar<GridTerrainNode, GraphConnection>(m_pGridGraph, m_pHeuristicFunction);
		auto startNode = m_pGridGraph->GetNode(startPathIdx);
		auto endNode = m_pGridGraph->GetNode(endPathIdx);

		m_vPath = pathfinder.FindPath(startNode, endNode);


		std::cout << "New Path Calculated" << std::endl;
	}
	else
	{
		std::cout << "No valid start and end node..." << std::endl;
		m_vPath.clear();
	}
}

void App_PathfindingAStar::MakeMaze()
{
	while (m_Walls.size() > 0)
	{
		bool opendWall{ false };
		while (!opendWall && m_Walls.size() > 0)
		{
			int currIndex{ int(rand() % m_Walls.size()) };

			if (m_WallOpenings[m_Walls[currIndex]] < 2)
			{
				opendWall = true;
				m_Walls[currIndex]->SetTerrainType(TerrainType::Ground);

				if (m_pGridGraph->GetNodeConnections(m_Walls[currIndex]).size() > 0)
				{
					for (auto connection : m_pGridGraph->GetNodeConnections(m_Walls[currIndex]))
					{
						bool dubbleNode{ false };
						for (auto currNode : m_Walls)
						{
							if (m_pGridGraph->GetNode(connection->GetTo()) == currNode) dubbleNode = true;
						}
						if (!dubbleNode) m_Walls.push_back(m_pGridGraph->GetNode(connection->GetTo()));
						m_WallOpenings[m_pGridGraph->GetNode(connection->GetTo())] += 1;
						if (m_WallOpenings[m_pGridGraph->GetNode(connection->GetTo())] < 2)
						{
							//m_pGridGraph->RemoveConnectionsToAdjacentNodes(connection->GetTo());
						}
					}
				}

				//m_pGridGraph->RemoveConnectionsToAdjacentNodes(currIndex);
			}
			m_Walls.erase(m_Walls.begin() + currIndex);
		}
	}

	m_pGridGraph->GetNode(ROWS * COLUMNS - 1)->SetTerrainType(TerrainType::Ground);
}

void App_PathfindingAStar::MakeMazeSolid()
{
	for (auto node : m_pGridGraph->GetAllNodes())
	{
		if (node->GetTerrainType() == TerrainType::Water)
		{
			m_pGridGraph->RemoveConnectionsToAdjacentNodes(node->GetIndex());
		}
	}
}
