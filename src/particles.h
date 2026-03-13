#ifndef PARTICLES_H
#define PARTICLES_H

#include <cglm/vec3.h>

#define MAX_PARTICLES 1000
#define GRAVITY 0.0f
#define DAMPING 1.0f  
#define COLLISION_THRESHOLD 0.01f

typedef struct {
    vec3 position;
    vec3 velocity;
    float radius;
    float mass;
    int active;
} Particle;

typedef struct {
    Particle particles[MAX_PARTICLES];
    int count;
} ParticleSystem;

ParticleSystem* particles_init();

void particles_free(ParticleSystem* sys);

void particles_add(ParticleSystem* sys, vec3 pos, vec3 vel, float radius, float mass);

void particles_update(ParticleSystem* sys, float deltaTime);

int particles_colliding(Particle* p1, Particle* p2);

void particles_resolve_collision(Particle* p1, Particle* p2);

void particles_check_floor_collision(Particle* p);

#endif
