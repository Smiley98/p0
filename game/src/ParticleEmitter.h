#pragma once
#include "raylib.h"
#include "raymathext.h"
#include <vector>

enum ParticleShapeType
{
    PARTICLE_SHAPE_SPHERE,
    PARTICLE_SHAPE_BOX,
    PARTICLE_SHAPE_CONE,
    PARTICLE_SHAPE_TYPE_COUNT
};

union ParticleShape
{
    struct
    {
        float radius;
    } sphere;

    struct
    {
        Vector3 size;
    } box;

    struct
    {
        float angle;
    } cone;
};

struct Particle
{
    Vector3 position;
    Vector3 velocity;
    Color color;
    float size;
    float life;
    float life_max;
	// like mayflies, but less buggy (cmon that was funny)
};

struct ParticleEmitter
{
    ParticleShapeType shape_type = PARTICLE_SHAPE_TYPE_COUNT;
    ParticleShape shape;

    Vector3 position = Vector3Zeros;
    Vector3 direction = Vector3UnitY;

    float spawn_rate = 10.0f;
    float lifetime = 1.0f;

    float speed = 1.0f;
    float size = 0.1f;

    Color color_src = WHITE;
    Color color_dst = BLACK;

    std::vector<Particle> particles;
    float spawn_accumulator = 0.0f;
};

void DestroyParticleEmitter(ParticleEmitter* emitter);

void UpdateParticleEmitter(ParticleEmitter& emitter);
void DrawParticleEmitter(const ParticleEmitter& emitter, const Camera& camera);

// TODO - Particle management? -- Something like a ParticleSystem to handle rendering particles for given durations?
// Probably not worth adding the idea of "particle systems" until we've established visual effects like smoke/fire/electricity, etc
// Should also think of how to attach particles to entities, but that's more of a Connor problem
