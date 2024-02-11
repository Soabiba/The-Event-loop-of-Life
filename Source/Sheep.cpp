#include "Level.h"
#include "string"

Sheep::Sheep([[maybe_unused]] Level* level, double _health, Vector2 _position)
{

	position = _position;

	currentlyBreeding = false;
	canBreed = false;
	birthingTimer = birthingTimerMax;
	reproduceTimer = reproduceTimerMax;

	health = _health;
}

void Sheep::sense(Level* level)
{

	if (closestGrass != NULL && closestGrass->dead == true)
	{
		closestGrass = NULL;
	}


	grassNear = false;
	for (Grass grass : level->grass_agents)
	{
		if (Vector2Distance(position, grass.position) <= senseRange && grass.edible)
		{
			grassNear = true;
			if (closestGrass == NULL)
			{
				closestGrass = level->get_agent(grass.id);
			}
			else
				if (Vector2Distance(position, grass.position) < Vector2Distance(position, closestGrass->position))
				{
					closestGrass = level->get_agent(grass.id);
				}

		}

	}



	nearbyWolves.clear();
	wolfNear = false;
	for (Wolf wolf : level->wolf_agents)
	{
		if (Vector2Distance(position, wolf.position) <= senseRange)
		{
			nearbyWolves.push_back(&wolf);
			wolfNear = true;
		}

	}


	int random_direction = GetRandomValue(0, 360);


	wanderingVelocity.x = (float)cos(random_direction);
	wanderingVelocity.y = (float)sin(random_direction);
	wanderingVelocity = Vector2Normalize(wanderingVelocity);
	wanderingVelocity = Vector2Scale(wanderingVelocity, speed);

}

void Sheep::decide([[maybe_unused]] Level* level)
{

	if (reproduceTimer > 0 && !currentlyBreeding)
	{
		--reproduceTimer;
	}
	else
	{
		canBreed = true;
		reproduceTimer = reproduceTimerMax;
	}


	sheepState = wandering;

	if (hunger > hungerLimit / 2 && closestGrass != NULL && closestGrass->edible)
	{
		sheepState = goingToFood;
		if (Vector2Distance(closestGrass->position, position) < eatRange)
		{
			sheepState = still;
			canEat = true;
		}
	}


	if (health >= healthCanBreed && canBreed)
	{
		sheepState = breeding;
		canBreed = false;
		currentlyBreeding = true;
	}

	if (currentlyBreeding)
	{
		sheepState = breeding;
	}

	if (wolfNear)
	{
		canEat = false;
		currentlyBreeding = false;
		birthingTimer = birthingTimerMax;
		sheepState = running;
	}

}

void Sheep::act(Level* level)
{
#pragma region HealthAndHunger
	double hungerGain = 0.0004;
	hunger += hungerGain;
	if (hunger > hungerLimit / 2)
	{
		health -= 0.001;
	}

	if (health <= 0)
	{
		dead = true;
	}
	else if (health >= maxHealth)
	{
		health = maxHealth;
	}
#pragma endregion HealthAndHunger

	if (sheepState == running)
	{
		Vector2 averageDirectionAway = { 0,0 };

		for (Agent* wolf : nearbyWolves)
		{
			Vector2 direction = Vector2Subtract(position, wolf->position);
			averageDirectionAway = Vector2Add(averageDirectionAway, direction);
		}

		averageDirectionAway = Vector2Normalize(averageDirectionAway);


		velocity = Vector2Scale(averageDirectionAway, speed);

		position = Vector2Add(position, Vector2Scale(velocity, GetFrameTime()));
		if (position.x + imgSize / 2 > 800)
		{
			position.x = 800 - imgSize / 2;
		}
		if (position.x < 0)
		{
			position.x = 0 + imgSize / 2;
		}
		if (position.y + imgSize * 2 > 450)
		{
			position.y = 450 - imgSize / 2;
		}
		if (position.y < 0)
		{
			position.y = 0 + imgSize / 2;
		}
		return;
	}
	if (sheepState == breeding)
	{
		if (birthingTimer <= 0)
		{

			level->spawn_agent(Sheep(level, maxHealth / 2, position));
			health = maxHealth / 2;
			birthingTimer = birthingTimerMax;
			currentlyBreeding = false;
			hunger = 90;
		}
		else
		{
			birthingTimer--;
		}
		return;
	}
	if (canEat && sheepState == still)
	{
		Agent* grass = level->get_agent(closestGrass->id);

		health += 60;
		hunger -= 20;
		grass->health -= 60;
		if (position.x + imgSize / 2 > 800)
		{
			position.x = 800 - imgSize / 2;
		}
		if (position.x < 0)
		{
			position.x = 0 + imgSize / 2;
		}
		if (position.y + imgSize > 450)
		{
			position.y = 450 - imgSize;
		}
		if (position.y < 0)
		{
			position.y = 0 + imgSize / 2;
		}
		return;
	}
	if (sheepState == goingToFood)
	{
		Vector2 direction = Vector2Subtract(closestGrass->position, position);
		direction = Vector2Normalize(direction);
		velocity = Vector2Scale(direction, speed);

		position = Vector2Add(position, Vector2Scale(velocity, GetFrameTime()));
		if (position.x + imgSize / 2 > 800)
		{
			position.x = 800 - imgSize / 2;
		}
		if (position.x < 0)
		{
			position.x = 0 + imgSize / 2;
		}
		if (position.y + imgSize > 450)
		{
			position.y = 450 - imgSize;
		}
		if (position.y < 0)
		{
			position.y = 0 + imgSize / 2;
		}
		return;
	}
	if (sheepState == wandering)
	{
		velocity = wanderingVelocity;

		position = Vector2Add(position, Vector2Scale(velocity, GetFrameTime()));
	}
	if (position.x + imgSize / 2 > 800)
	{
		position.x = 800 - imgSize / 2;
	}
	if (position.x < 0)
	{
		position.x = 0 + imgSize / 2;
	}
	if (position.y + imgSize > 450)
	{
		position.y = 450 - imgSize;
	}
	if (position.y < 0)
	{
		position.y = 0 + imgSize / 2;
	}
}

void Sheep::draw()
{
	Color color = BLACK;
	if (sheepState == goingToFood)
	{
		color = ORANGE;
	}
	if (sheepState == wandering)
	{
		color = BLUE;
	}
	if (sheepState == still)
	{
		color = DARKBLUE;
	}
	if (sheepState == breeding)
	{
		color = PINK;
	}
	if (sheepState == running)
	{
		color = RED;
	}

	float temp = imgSize;
	imgSize = (float)(temp * health / maxHealth);
	DrawRectangle((int)position.x, (int)position.y, (int)imgSize, (int)imgSize, color);
	DrawLineV(Vector2AddValue(position, imgSize / 2), Vector2Add(position, Vector2Scale(velocity, 1)), RED);
	imgSize = temp;

}
