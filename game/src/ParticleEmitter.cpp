//where particles are born, live, and die... So deep :0
#include "ParticleEmitter.h"
#include <random>

extern Texture2D TEX;

//random, like a box of chocolates
static float RandomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

void ParticleEmitter::Update(float dt) {
    //legit murder dead particles
    particles.erase(std::remove_if(particles.begin(), particles.end(),
        [](const Particle& p) { return p.lifetime <= 0; }), particles.end());
                                                                                                                                                                //TraceLog(LOG_INFO, "particle count: %d", particles.size());

    //update existing particles
    for (auto& p : particles) {p.position = Vector3Add(p.position, Vector3Scale(p.velocity, dt));
        p.lifetime -= dt;
        //basic ahh fade
        float lifeRatio = p.lifetime / p.maxLife;
        p.color = startColor;
        p.color.a = static_cast<unsigned char>(startColor.a * lifeRatio);      
    }

    //make it rain
    spawnAccumulator += dt;
    float spawnInterval = 1.0f / spawnRate;
    while (spawnAccumulator >= spawnInterval) {
        SpawnParticle();
        spawnAccumulator -= spawnInterval;
    }
}

void ParticleEmitter::Draw(const Camera& camera) {
    for (const auto& p : particles) {
        //LOOK AT ME
        DrawBillboard(camera,TEX,p.position, p.size,p.color);//TODO need 2 load a texture for this
    }
}

void ParticleEmitter::SpawnParticle() {
    Particle p;
    p.position = Vector3Add(position, GetRandomPosition());
    Vector3 dir = (shape == Shape::CONE) ? GetConeDirection() : Vector3Normalize(direction);
    p.velocity = Vector3Scale(dir, particleSpeed);

    p.size = particleSize;
    p.maxLife = lifetime;
    p.lifetime = lifetime;
    p.color = startColor;
    particles.push_back(p);

}

Vector3 ParticleEmitter::GetRandomPosition() {
    switch (shape) {
    case Shape::SPHERE:
        return Vector3Scale(Vector3Normalize(Vector3{
            RandomFloat(-1, 1),
            RandomFloat(-1, 1),
            RandomFloat(-1, 1)
            }), RandomFloat(0, radius));
    case Shape::BOX:
        return {
            RandomFloat(-boxSize.x / 2, boxSize.x / 2),
            RandomFloat(-boxSize.y / 2, boxSize.y / 2),
            RandomFloat(-boxSize.z / 2, boxSize.z / 2)
        };
    case Shape::CONE:
        return { 0, 0, 0 }; // all particles spawn at origin of cone
    default:
        return { 0, 0, 0 };
    }
}

//cone shit
Vector3 ParticleEmitter::GetConeDirection() {
    float angleRad = (coneAngle * DEG2RAD);

    //random angle within cone
    float u = RandomFloat(0.0f, 1.0f);
    float v = RandomFloat(0.0f, 1.0f);
    float theta = u * 2.0f * PI;
    float phi = acos(1.0f - v * (1.0f - cos(angleRad)));

    //to cartesian
    float x = sin(phi) * cos(theta);
    float y = sin(phi) * sin(theta);
    float z = cos(phi);

    //cone dir in x
    Vector3 dir = { x, y, z };

    // rotate too match emitter direction
    Vector3 up = { 0, 0, 1 };
    if (Vector3LengthSqr(Vector3Subtract(direction, up)) < 0.001f)
        return dir; //already aligned
    else {
        Vector3 axis = Vector3CrossProduct(up, direction);
        float angle = acos(Vector3DotProduct(Vector3Normalize(up), Vector3Normalize(direction)));
        dir = Vector3RotateByAxisAngle(dir, axis, angle);
        return dir;
    }
}

