#include "particles.h"
#include <stdlib.h>
#include <math.h>
#include <cglm/cglm.h>

ParticleSystem* particles_init() {
    ParticleSystem* sys = (ParticleSystem*)malloc(sizeof(ParticleSystem));
    sys->count = 0;
    for (int i = 0; i < MAX_PARTICLES; i++) {
        sys->particles[i].active = 0;
    }
    return sys;
}

void particles_free(ParticleSystem* sys) {
    free(sys);
}

void particles_add(ParticleSystem* sys, vec3 pos, vec3 vel, float radius, float mass) {
    if (sys->count >= MAX_PARTICLES) return;
    
    Particle* p = &sys->particles[sys->count];
    glm_vec3_copy(pos, p->position);
    glm_vec3_copy(vel, p->velocity);
    p->radius = radius;
    p->mass = mass;
    p->active = 1;
    sys->count++;
}

void particles_check_floor_collision(Particle* p) {
    if (p->position[1] - p->radius < -2.5f) {
        p->position[1] = -2.5f + p->radius;
        p->velocity[1] = -p->velocity[1] * DAMPING;
        
        if (fabs(p->velocity[1]) < 0.1f) {
            p->velocity[1] = 0.0f;
        }
    }
    
    if (p->position[1] + p->radius > 2.5f) {
        p->position[1] = 2.5f - p->radius;
        p->velocity[1] = -p->velocity[1] * DAMPING;
    }
    
    if (p->position[0] - p->radius < -2.5f) {
        p->position[0] = -2.5f + p->radius;
        p->velocity[0] = -p->velocity[0] * DAMPING;
    }
    if (p->position[0] + p->radius > 2.5f) {
        p->position[0] = 2.5f - p->radius;
        p->velocity[0] = -p->velocity[0] * DAMPING;
    }
    
    if (p->position[2] - p->radius < -2.5f) {
        p->position[2] = -2.5f + p->radius;
        p->velocity[2] = -p->velocity[2] * DAMPING;
    }
    if (p->position[2] + p->radius > 2.5f) {
        p->position[2] = 2.5f - p->radius;
        p->velocity[2] = -p->velocity[2] * DAMPING;
    }
}

int particles_colliding(Particle* p1, Particle* p2) {
    vec3 diff;
    glm_vec3_sub(p1->position, p2->position, diff);
    float dist = glm_vec3_norm(diff);
    float minDist = p1->radius + p2->radius;
    return dist < minDist;
}

void particles_resolve_collision(Particle* p1, Particle* p2) {
    vec3 collisionNormal;
    glm_vec3_sub(p1->position, p2->position, collisionNormal);
    float dist = glm_vec3_norm(collisionNormal);
    
    if (dist < COLLISION_THRESHOLD) return;
    
    glm_vec3_scale(collisionNormal, 1.0f / dist, collisionNormal);
    
    vec3 relVel;
    glm_vec3_sub(p1->velocity, p2->velocity, relVel);
    
    float velAlongNormal = glm_vec3_dot(relVel, collisionNormal);
    
    if (velAlongNormal >= 0) return;
    
    float e = 0.8f;
    
    float impulse = -(1.0f + e) * velAlongNormal / (1.0f / p1->mass + 1.0f / p2->mass);
    
    vec3 impulseVec;
    glm_vec3_scale(collisionNormal, impulse / p1->mass, impulseVec);
    glm_vec3_add(p1->velocity, impulseVec, p1->velocity);
    
    glm_vec3_scale(collisionNormal, -impulse / p2->mass, impulseVec);
    glm_vec3_add(p2->velocity, impulseVec, p2->velocity);
    
    float overlap = p1->radius + p2->radius - dist;
    if (overlap > 0) {
        vec3 separation;
        glm_vec3_scale(collisionNormal, overlap / 2.0f + 0.001f, separation);
        glm_vec3_add(p1->position, separation, p1->position);
        glm_vec3_scale(separation, -1.0f, separation);
        glm_vec3_add(p2->position, separation, p2->position);
    }
}

void particles_update(ParticleSystem* sys, float deltaTime) {
    for (int i = 0; i < sys->count; i++) {
        Particle* p = &sys->particles[i];
        if (!p->active) continue;
        
        p->velocity[1] += GRAVITY * deltaTime;
        
        vec3 displacement;
        glm_vec3_scale(p->velocity, deltaTime, displacement);
        glm_vec3_add(p->position, displacement, p->position);

        particles_check_floor_collision(p);
    }
    
    for (int i = 0; i < sys->count; i++) {
        for (int j = i + 1; j < sys->count; j++) {
            Particle* p1 = &sys->particles[i];
            Particle* p2 = &sys->particles[j];
            
            if (!p1->active || !p2->active) continue;
            
            if (particles_colliding(p1, p2)) {
                particles_resolve_collision(p1, p2);
            }
        }
    }
}
