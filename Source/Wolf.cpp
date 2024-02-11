#include "Level.h"
#include "string"

Wolf::Wolf([[maybe_unused]] Level* level, double _health, Vector2 _position)
{

	position = _position;

	hunger = hungerLimit;
	canBreed = false;
	health = _health;
}

void Wolf::sense(Level* level)
{

	if (closestSheep != NULL && closestSheep->dead == true)
	{
		closestSheep = NULL;
	}

	sheepNear = false;
	for (Sheep sheep : level->sheep_agents)
	{
		if (Vector2Distance(position, sheep.position) <= senseRange)
		{
			sheepNear = true;
			if (closestSheep == NULL)
			{
				closestSheep = level->get_agent(sheep.id);
			}
			else
				if (Vector2Distance(position, sheep.position) < Vector2Distance(position, closestSheep->position))
				{
					closestSheep = level->get_agent(sheep.id);
				}

		}

	}


	int random_direction = GetRandomValue(0, 360);


	wanderingVelocity.x = (float)cos(random_direction);
	wanderingVelocity.y = (float)sin(random_direction);
	wanderingVelocity = Vector2Normalize(wanderingVelocity);
	wanderingVelocity = Vector2Scale(wanderingVelocity, speed);

}

void Wolf::decide([[maybe_unused]] Level* level)
{

	if (reproduceTimer > 0 && canBreed == false)
	{
		--reproduceTimer;
	}
	else
	{
		canBreed = true;
		reproduceTimer = reproduceTimerMax;
	}


	wolfState = wandering;

	if (hunger > hungerLimit / 2 && closestSheep != NULL && sheepNear)
	{
		wolfState = goingToFood;
		if (Vector2Distance(closestSheep->position, position) < eatRange)
		{
			wolfState = still;
			canEat = true;
		}
	}


	if (health >= healthCanBreed && canBreed)
	{
		wolfState = breeding;
		canBreed = false;
	}
}

void Wolf::act(Level* level)
{
	if (hunger > hungerLimit / 2)
	{

		health -= 000000.1;
	}
	hunger += 0.01;
	if (health <= 0)
	{
		dead = true;
	}


	if (wolfState == breeding)
	{
		level->spawn_agent(Wolf(level, (int)maxHealth / 2, position));
		health -= maxHealth / 2;
		hunger += hungerLimit / 4;
		return;
	}
	if (canEat && wolfState == still)
	{
		level->get_agent(closestSheep->id)->health -= 50;
		health += healthCanBreed / 4;
		hunger -= hungerLimit / 2;
		return;
	}
	if (wolfState == goingToFood)
	{
		int chance = GetRandomValue(0, 10);
		if (chance > 2)
		{
			Vector2 direction = Vector2Subtract(closestSheep->position, position);
			direction = Vector2Normalize(direction);
			velocity = Vector2Scale(direction, speed);

			position = Vector2Add(position, Vector2Scale(velocity, GetFrameTime()));
			return;
		}
		else
		{
			wolfState = still;
		}
	}
	if (wolfState == wandering)
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
		position.y = 450 - imgSize / 2;
	}
	if (position.y < 0)
	{
		position.y = 0 + imgSize / 2;
	}
}

void Wolf::draw()
{
	Color color = MAGENTA;
	if (wolfState == goingToFood || wolfState == still)
	{
		color = ORANGE;
	}
	if (wolfState == wandering)
	{
		color = RED;
	}
	if (wolfState == breeding)
	{
		color = PINK;
	}

	float temp = imgSize;
	imgSize = (float)(temp * health / maxHealth);
	DrawCircle((int)position.x, (int)position.y, imgSize, color);
	DrawLineV(Vector2AddValue(position, imgSize / 2), Vector2Add(position, Vector2Scale(velocity, 1)), RED);
	imgSize = temp;

}