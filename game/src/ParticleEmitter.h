#pragma once
#include "raylib.h"
#include "raymath.h"
#include <vector>

struct Particle
{
    Vector3 position;
    Vector3 velocity;
    Color color;
    float size;
    float lifetime;
    float maxLife;
	// like mayflies, but less buggy (cmon that was funny)
};

class ParticleEmitter {
public:
    enum class Shape { SPHERE, BOX,CONE }; //emitter shape

    Vector3 position = { 0 };
    Shape shape = Shape::SPHERE;

    float radius = 1.0f;       //for sphere shape

    Vector3 boxSize = { 1,1,1 };//box shape

    float coneAngle = 30.0f; // for cone shape (indegrees)

    float spawnRate = 10.0f;   //particles per second
    float particleSpeed = 1.0f;
    Vector3 direction = { 0,1,0 }; //generally you want them to go... somewhere
    float particleSize = 0.1f;
    float lifetime = 1.0f;
    Color startColor = WHITE;
    Color endColor = WHITE;

    void Update(float dt);
    void Draw(const Camera& camera);

private:
    std::vector<Particle> particles;
    float spawnAccumulator = 0.0f;

    void SpawnParticle();
    Vector3 GetRandomPosition();
    Vector3 GetConeDirection();
};