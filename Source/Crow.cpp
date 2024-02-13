#include "Level.h"
#include <cmath>

// Constructor
Crow::Crow(Vector2 pos) : position(pos), velocity({ static_cast<float>(GetRandomValue(-2, 2)), static_cast<float>(GetRandomValue(-2, 2)) }), acceleration({ 0.0f, 0.0f }), maxForce(0.3f), maxSpeed(4.0f) {}

// Update the crow's position and velocity
void Crow::update() {
    velocity.x += acceleration.x;
    velocity.y += acceleration.y;
    // Limit speed
    float speed = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (speed > maxSpeed) {
        velocity.x = (velocity.x / speed) * maxSpeed;
        velocity.y = (velocity.y / speed) * maxSpeed;
    }
    position.x += velocity.x;
    position.y += velocity.y;
    acceleration = { 0, 0 }; // Reset acceleration to 0 each cycle
}

// Apply a force to the crow's acceleration
void Crow::applyForce(Vector2 force) {
    acceleration.x += force.x;
    acceleration.y += force.y;
}

// Steer the crow towards a target position
Vector2 Crow::seek(Vector2 target) {
    Vector2 desired = { target.x - position.x, target.y - position.y };
    // Normalize desired and scale to maximum speed
    float d = sqrt(desired.x * desired.x + desired.y * desired.y);
    if (d > 0) {
        desired.x /= d;
        desired.y /= d;
        desired.x *= maxSpeed;
        desired.y *= maxSpeed;
        // Steering = Desired minus Velocity
        Vector2 steer = { desired.x - velocity.x, desired.y - velocity.y };
        // Limit to maximum steering force
        float s = sqrt(steer.x * steer.x + steer.y * steer.y);
        if (s > maxForce) {
            steer.x /= s;
            steer.y /= s;
            steer.x *= maxForce;
            steer.y *= maxForce;
        }
        return steer;
    }
    return { 0, 0 };
}

// Calculate and apply the flocking forces
void Crow::flock(std::vector<Crow>& crows) {
    Vector2 sep = separate(crows); // Separation
    Vector2 ali = align(crows);    // Alignment
    Vector2 coh = cohesion(crows); // Cohesion

    // Arbitrarily weight these forces
    sep.x *= 1.5; sep.y *= 1.5;
    ali.x *= 1.0; ali.y *= 1.0;
    coh.x *= 1.0; coh.y *= 1.0;

    // Add the force vectors to acceleration
    applyForce(sep);
    applyForce(ali);
    applyForce(coh);
}

Vector2 LimitVector2(Vector2 v, float max) {
    float lengthSquared = v.x * v.x + v.y * v.y;
    if (lengthSquared > max * max) {
        float length = sqrtf(lengthSquared);
        v.x = (v.x / length) * max;
        v.y = (v.y / length) * max;
    }
    return v;
}

// Separation: steer to avoid crowding local flockmates
Vector2 Crow::separate(std::vector<Crow>& crows) {
    float desiredSeparation = 25.0f;
    Vector2 steer = { 0, 0 };
    int count = 0;
    for (Crow& other : crows) {
        float d = Vector2Distance(position, other.position);
        if ((d > 0) && (d < desiredSeparation)) {
            Vector2 diff = Vector2Subtract(position, other.position);
            diff = Vector2Normalize(diff);
            diff = Vector2Scale(diff, 1 / d); // Weight by distance
            steer = Vector2Add(steer, diff);
            count++;
        }
    }
    if (count > 0) {
        steer = Vector2Scale(steer, 1 / (float)count);
    }
    if (Vector2Length(steer) > 0) {
        steer = Vector2Normalize(steer);
        steer = Vector2Scale(steer, maxSpeed);
        steer = Vector2Subtract(steer, velocity);
        steer = LimitVector2(steer, maxForce);
    }
    return steer;
}

// Alignment: steer towards the average heading of local flockmates
Vector2 Crow::align(std::vector<Crow>& crows) {
    float neighborDist = 50.0f;
    Vector2 sum = { 0, 0 };
    int count = 0;
    for (Crow& other : crows) {
        float d = Vector2Distance(position, other.position);
        if ((d > 0) && (d < neighborDist)) {
            sum = Vector2Add(sum, other.velocity);
            count++;
        }
    }
    if (count > 0) {
        sum = Vector2Scale(sum, 1 / (float)count);
        sum = Vector2Normalize(sum);
        sum = Vector2Scale(sum, maxSpeed);
        Vector2 steer = Vector2Subtract(sum, velocity);
        steer = LimitVector2(steer, maxForce);
        return steer;
    }
    return { 0, 0 };
}

// Cohesion: steer to move towards the average position of local flockmates
Vector2 Crow::cohesion(std::vector<Crow>& crows) {
    float neighborDist = 50.0f;
    Vector2 sum = { 0, 0 };
    int count = 0;
    for (Crow& other : crows) {
        float d = Vector2Distance(position, other.position);
        if ((d > 0) && (d < neighborDist)) {
            sum = Vector2Add(sum, other.position);
            count++;
        }
    }
    if (count > 0) {
        sum = Vector2Scale(sum, 1 / (float)count);
        return seek(sum);
    }
    return { 0, 0 };
}

Flock::Flock() {}

void Flock::initialize(int numberOfCrows, int screenWidth, int screenHeight) {
    for (int i = 0; i < numberOfCrows; i++) {
        Vector2 pos = { static_cast<float>(GetRandomValue(0, screenWidth)), static_cast<float>(GetRandomValue(0, screenHeight)) };
        this->addCrow(Crow(pos));
    }
}

void Flock::addCrow(const Crow& crow) {
    crows.push_back(crow);
}

void Flock::update() {
    for (auto& crow : crows) {
        crow.flock(crows);
        crow.update();
    }
}

void Flock::draw() {
    for (auto& crow : crows) {
        DrawCircle(static_cast<int>(crow.position.x), static_cast<int>(crow.position.y), 5, VIOLET);
    }
}

