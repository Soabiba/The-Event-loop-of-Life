#pragma once
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <unordered_map>

class Level;

class Agent
{
public:
	int id = 0;
	Vector2 position = {};
	bool dead = false;
	bool edible = false;
	double health = 0;

	virtual void sense(Level* level) = 0;
	virtual void decide(Level* level) = 0;
	virtual void act(Level* level) = 0;

	virtual void draw() = 0;

	virtual ~Agent() = default;
};

// This is just an example
class Grass : public Agent {
public:
	int growth_timeMax = 20;
	int growth_time = growth_timeMax;  // In number of frames
	int growth_rate = 2;  // The amount of growth per frame

	bool canSpread = false;
	int spreadTimerMax = 60;
	int spreadTimer = spreadTimerMax;
	float maxHealth = 100;
	float yellowHealth = maxHealth / 2;
	bool dirt = false;

	float trampleRange = 15;
	int gridX = 5;
	int gridY = 5;

	bool trampled = false;
	bool ready = false;
	bool spread = false;

	enum State {
		growing,
		spreading,
		trampledd,
		dirtt
	};
	State grassState = growing;
	struct Neighbour {
		int posX = 0;
		int posY = 0;
	};

	Neighbour spreadTo;


	Grass(int x, int y, Level* level);

	void sense(Level* level) override;

	void decide(Level* level);
	// If the growth time is up, increase the size of the grass.


	void act(Level* level);

	// Check if the growth size is above a certain threshold, and if so, create a new grass agent.
	// Also check if it's being trambled on and if so, dont grow or spread
	void draw() override;


	// Draw the grass on the screen using the position and size.

};

class Sheep : public Agent
{
public:
	enum state
	{
		running,
		wandering,
		goingToFood,
		breeding,
		still
	};

	state sheepState = wandering;
	double maxHealth = 120;
	double healthCanBreed = maxHealth - 15;
	float speed = 15;
	float imgSize = 18;

	int eatRange = 10;
	int senseRange = 100;
	double hunger = 35;
	double hungerLimit = 50;
	int reproduceTimerMax = 600;
	int reproduceTimer = reproduceTimerMax;

	bool currentlyBreeding = false;
	int birthingTimerMax = 40;
	int birthingTimer = 40;


	bool canBreed = false;
	bool wolfNear = false;
	bool grassNear = false;
	bool canEat = false;


	Vector2 velocity = { 0,0 };
	Vector2 wanderingVelocity = velocity;
	std::list<Agent*> nearbyWolves;
	Agent* closestGrass = NULL;

	Sheep(Level* level, double health, Vector2 position);

	void sense(Level* level) override;
	void decide(Level* level) override;
	void act(Level* level) override;
	void draw() override;


};

class Wolf : public Agent
{
public:
	enum state
	{
		wandering,
		goingToFood,
		breeding,
		still
	};

	state wolfState = wandering;
	double maxHealth = 220;
	double healthCanBreed = maxHealth - 20;
	float speed = 31;
	float imgSize = 22;

	int eatRange = 7;
	int senseRange = 170;
	double hunger = 0;
	double hungerLimit = 60;
	int reproduceTimerMax = 280;
	int reproduceTimer = reproduceTimerMax;

	bool canBreed = false;
	bool sheepNear = false;
	bool canEat = false;


	Vector2 velocity = { 0,0 };
	Vector2 wanderingVelocity = velocity;
	Agent* closestSheep = NULL;

	Wolf(Level* level, double health, Vector2 position);

	void sense(Level* level) override;
	void decide(Level* level) override;
	void act(Level* level) override;
	void draw() override;


};

class Level
{
	int last_id = 0;
	//NOTE(Filippo): Using a list here is not the best idea, ideally you should store agents in some other data structure that keeps them close to each other while being pointer-stable.
	// @AddMoreHere

	std::unordered_map<int, Agent*> id_to_agent;
	std::vector<Agent*> all_agents;

	std::vector<Agent*> pending_agents; // Agents that will be added at the beginning of the next frame
	int framesGameRanFor = 0;
	int frameTimerMax = 65;
	int frameTimer = frameTimerMax;
public:
	bool sense = true;
	static const int Tile_Size = 15;
	static const int MAP_WIDTH = 800 / Tile_Size;
	static const int MAP_HEIGHT = 450 / Tile_Size;
	std::list<Grass> grass_agents;
	std::list<Wolf> wolf_agents;
	std::list<Sheep> sheep_agents;
	int grass_map[MAP_WIDTH][MAP_HEIGHT];
	Agent* get_agent(int id);
	Agent* get_agent(int gridX, int gridY);
	Vector2 randPos();

	Agent* spawn_agent(Grass agent);
	Agent* spawn_agent(Wolf agent);
	Agent* spawn_agent(Sheep agent);
	// @AddMoreHere

	void reset();
	void update();
	void draw();

private:
	void remove_dead_and_add_pending_agents();
	// Remember, if you add more lists (see @AddMoreHere), edit this function so that dead agents are removed correctly without leaking memory
};