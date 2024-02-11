#include "Level.h"
#include "string"

Grass::Grass(int x, int y, Level* level)
{
	gridX = x;
	gridY = y;

	edible = true;
	canSpread = false;
	spread = false;
	health = maxHealth / 4;
	//makes it occupied
	level->grass_map[x][y] = 1;

}

void Grass::sense(Level* level)
{
	if (!dirt)
	{

		trampled = false;

		for (Sheep sheep : level->sheep_agents)
		{
			if (Vector2Distance(position, sheep.position) <= trampleRange)
			{
				trampled = true;

			}
		}
		for (Wolf wolf : level->wolf_agents)
		{
			if (Vector2Distance(position, wolf.position) <= trampleRange)
			{
				trampled = true;

			}
		}

		ready = true;
	}
}


void Grass::decide(Level* level)
{
	grassState = growing;

	if (spreadTimer <= 0 && health == maxHealth)
	{
		canSpread = true;
		spreadTimer = spreadTimerMax;
	}
	else
	{
		spreadTimer--;
	}

	if (canSpread && ready)
	{
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (i == 0 && j == 0)
				{
					continue;
				}
				// checks if empty
				if (gridX + i > 53 || gridX + i < 0 || gridY + j > 30 || gridY + j < 0)
				{
					continue;
				}
				if (level->grass_map[gridX + i][gridY + j] == 0 ||
					level->grass_map[gridX + i][gridY + j] == 3)
				{
					if (GetRandomValue(0, 100) >= 98)
					{
						ready = false;
						grassState = spreading;
						Neighbour neighbour{ gridX + i,gridY + j };
						spreadTo = neighbour;
						break;
					}
				}
			}
		}
	}

	if (trampled)
	{
		grassState = trample;
	}
	if (dirt)
	{
		grassState = dirty;
	}
}

void Grass::act(Level* level)
{
	if (health <= 0 || level->grass_map[gridX][gridY] == 3)
	{
		level->grass_map[gridX][gridY] = 3;
		dirt = true;
		edible = false;
		return;
	}


	if (grassState == spreading)
	{
		if (level->grass_map[spreadTo.posX][spreadTo.posY] == 0)
		{
			level->grass_map[spreadTo.posX][spreadTo.posY] = 1;
			level->spawn_agent(Grass(spreadTo.posX, spreadTo.posY, level));
		}
		else if (level->grass_map[spreadTo.posX][spreadTo.posY] == 3)
		{
			if (level->get_agent(spreadTo.posX, spreadTo.posY) != nullptr)
			{
				level->get_agent(spreadTo.posX, spreadTo.posY)->dead = true;
			}
			level->grass_map[spreadTo.posX][spreadTo.posY] = 1;
			level->spawn_agent(Grass(spreadTo.posX, spreadTo.posY, level));
		}
		canSpread = false;
		spreadTimer = spreadTimerMax;
	}
	if (grassState == growing)
	{

		if (growth_time == 0) {
			health += growth_rate;
			growth_time = growth_timeMax;
		}
		else {
			growth_time--;
		}
		if (health > maxHealth)
		{
			health = maxHealth;
		}
	}
	if (grassState == trample)
	{
		health -= 0.0001f;
	}

	if (position.x > 800)
	{
		dead = true;
	}
	if (position.x < 0)
	{
		dead = true;
	}
	if (position.y > 450)
	{
		dead = true;
	}
	if (position.y < 0)
	{
		dead = true;
	}
}

void Grass::draw()
{
	Color color = GREEN;
	if (grassState == trample)
	{
		color = DARKGREEN;
	}
	if (health <= yellowHealth)
	{
		color = YELLOW;
	}
	if (dirt)
	{
		color = BROWN;
	}
	if (!dirt)
	{

		DrawRectangle((int)position.x, (int)position.y, (int)(0.14 * health), (int)(0.14 * health), color);
	}
	else
	{
		DrawRectangle((int)position.x, (int)position.y, 14, 14, color);
	}

}