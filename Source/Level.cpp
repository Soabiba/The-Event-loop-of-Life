#include "Level.h"
#include "string"

Agent* Level::get_agent(int id)
{
	auto agent_iterator = id_to_agent.find(id);
	if (agent_iterator != id_to_agent.end())
	{
		return agent_iterator->second;
	}

	return nullptr;
}

Agent* Level::get_agent(int gridX, int gridY)
{
	for (Grass grass : grass_agents)
	{
		if (grass.gridX == gridX && grass.gridY == gridY)
		{
			return get_agent(grass.id);
		}
	}

	return nullptr;
}

void Level::remove_dead_and_add_pending_agents()
{
	auto agent_iterator = all_agents.begin();
	while (agent_iterator != all_agents.end())
	{
		if ((*agent_iterator)->dead)
		{
			id_to_agent.erase((*agent_iterator)->id);
			agent_iterator = all_agents.erase(agent_iterator);
		}
		else
		{
			agent_iterator++;
		}
	}
	
	grass_agents.remove_if([](Grass& a) { return a.dead; });
	sheep_agents.remove_if([](Sheep& a) { return a.dead; });
	wolf_agents.remove_if([](Wolf& a) { return a.dead; });

	// Add all pending agents
	for (Agent* agent : pending_agents)
	{
		last_id += 1;
		agent->id = last_id;

		all_agents.push_back(agent);
		id_to_agent.insert({ agent->id, agent });
	}

	pending_agents.clear(); 
}

Agent* Level::spawn_agent(Grass agent)
{
	Agent* result = nullptr;

	grass_agents.push_back(agent);
	result = &grass_agents.back();

	pending_agents.push_back(result); // Enqueue it so that it can be added to the level at the beginning of the next frame

	return result;
}

Agent* Level::spawn_agent(Sheep agent)
{
	Agent* result = nullptr;

	sheep_agents.push_back(agent);
	result = &sheep_agents.back();

	pending_agents.push_back(result); // Enqueue it so that it can be added to the level at the beginning of the next frame

	return result;
}

Agent* Level::spawn_agent(Wolf agent)
{
	Agent* result = nullptr;

	wolf_agents.push_back(agent);
	result = &wolf_agents.back();

	pending_agents.push_back(result); // Enqueue it so that it can be added to the level at the beginning of the next frame

	return result;
}

Vector2 Level::randPos()
{
	
	Vector2 result;

	result.x = (float)GetRandomValue(50, 750);
	result.y = (float)GetRandomValue(50, 400);
	return result;
}

void Level::reset()
{
	all_agents.clear();
	wolf_agents.clear();
	grass_agents.clear();
	sheep_agents.clear();

	for (int x = 0; x < MAP_WIDTH; x++)
	{
		for (int y = 0; y < MAP_HEIGHT; y++)
		{
			grass_map[x][y] = 0;
		}
	}

	for (int i = 0; i < 13; i++)
	{
		int x = GetRandomValue(3, 50);
		int y = GetRandomValue(3, 27);
		spawn_agent(Grass(x, y, this)); // Call without assigning the return value
	}

	Sheep exampleSheep(this, 100, randPos());
	Wolf exampleWolf(this, 100, randPos());

	for (int x = 0; x < 10; x++)
	{

		spawn_agent(Sheep(this, exampleSheep.maxHealth / 1.5f, randPos()));

	}
	for (int x = 1; x < 4; x++)
	{
		Vector2 position = { x * 200.0f,35.0f };
		spawn_agent(Wolf(this, exampleWolf.maxHealth / 1.5f, position));

	}
}

void Level::update()
{
	remove_dead_and_add_pending_agents();
	framesGameRanFor++;
	if (frameTimer <= 0)
	{
		sense = true;
		frameTimer = frameTimerMax;
	}
	else
	{
		frameTimer--;
	}

	for (auto& agent : all_agents)
	{
		if (sense)
		{
			agent->sense(this);
		}

		agent->decide(this);
		agent->act(this);
	}
	sense = false;

	for (Grass& agent : grass_agents)
	{
		if (agent.id != 0)
		{
			Agent* grass = get_agent(agent.id);
			if (grass_map[agent.gridX][agent.gridY] == 1)
			{
				grass->position.x = (float)(agent.gridX * Tile_Size);
				grass->position.y = (float)(agent.gridY * Tile_Size);
			}
		}
	}
}

void Level::draw()
{

	for (auto& grass : grass_agents)
	{
		grass.draw();
	}
	for (auto& sheep : sheep_agents)
	{
		sheep.draw();
	}
	for (auto& wolf : wolf_agents)
	{
		wolf.draw();
	}

	char sheep[64];
	sprintf_s(sheep, "%d", (int)sheep_agents.size());
	char grass[64];
	sprintf_s(grass, "%d", (int)grass_agents.size());
	char wolf[64];
	sprintf_s(wolf, "%d", (int)wolf_agents.size());
	char frames[64];
	sprintf_s(frames, "%d", framesGameRanFor);

}