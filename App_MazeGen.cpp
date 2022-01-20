#include "stdafx.h"
#include "App_MazeGen.h"

App_MazeGen::~App_MazeGen()
{
	delete m_pGridGraph;
}

void App_MazeGen::Start()
{
	DEBUGRENDERER2D->GetActiveCamera()->SetZoom(100.0f);
	DEBUGRENDERER2D->GetActiveCamera()->SetCenter(Elite::Vector2(150.0f, 75.0f));

	srand((unsigned)time(NULL));

	//Create Graph
	MakeGridGraph();
}

void App_MazeGen::Update(float deltaTime)
{
	MakeMaze();

	bool const middleMousePressed = INPUTMANAGER->IsMouseButtonUp(Elite::InputMouseButton::eMiddle);
	if (middleMousePressed)
	{
		MakeGridGraph();
	}
}

void App_MazeGen::Render(float deltaTime) const
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
}

void App_MazeGen::MakeGridGraph()
{
	m_pGridGraph = new Elite::GridGraph<Elite::GridTerrainNode, Elite::GraphConnection>(COLUMNS, ROWS, m_SizeCell, false, false, 1.f, 1.5f);

	for (size_t i = 1; i < m_pGridGraph->GetAllNodes().size(); i++)
	{
		m_pGridGraph->GetNode(i)->SetTerrainType(TerrainType::Water);
	}

	for ( auto connection : m_pGridGraph->GetNodeConnections(m_pGridGraph->GetNode(0)))
	{
		m_Walls.push_back(m_pGridGraph->GetNode(connection->GetTo()));
		m_WallOpenings[m_pGridGraph->GetNode(connection->GetTo())] += 1;
	}
	m_pGridGraph->RemoveConnectionsToAdjacentNodes(rand() % m_pGridGraph->GetAllNodes().size());
}

void App_MazeGen::MakeMaze()
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

				for (auto connection : m_pGridGraph->GetNodeConnections(m_Walls[currIndex]))
				{
					bool dubbleNode{ false };
					for (auto currNode : m_Walls)
					{
						if (m_pGridGraph->GetNode(connection->GetTo()) == currNode) dubbleNode = true;
					}
					if (!dubbleNode) m_Walls.push_back(m_pGridGraph->GetNode(connection->GetTo()));
					m_WallOpenings[m_pGridGraph->GetNode(connection->GetTo())] += 1;
				}

				//m_pGridGraph->RemoveConnectionsToAdjacentNodes(currIndex);
			}
			m_Walls.erase(m_Walls.begin() + currIndex);
		}
	}
}
