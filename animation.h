#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <vector>
#include "particle.h"
#include "geometry.h"


class animation
{
public:
    animation();
    animation(int num_particles, float delta_time = 0.01f, float initial_time = 0.0f,  float x = 0.0f, float y = 0.5f, float z = 0.0f);
    std::vector<float> animate_frame();
    void addParticles(int new_particles = 1);

private:
    std::vector<Particle> current_particles;
    std::vector<Plane> planes;
    float dt;
    float tini;
    float initial_x = 0.0f;
    float initial_y = 0.5f;
    float initial_z = 0.0f;



};

#endif // ANIMATION_H
