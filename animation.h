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
    void initializeValues();
    std::vector<float> animate_frame();
    void addParticles(int new_particles = 1);

    void setRoomParam(float f, float d);
    void setTriangleParam(glm::vec3 ver1, glm::vec3 ver2, glm::vec3 ver3);
    void setSphereParam(glm::vec3 cent, float rad);
    void setParticleParam(int num_part, float life, float bounce);
    void setFountain(float x, float y, float z,float fount_y);

    void clearParticles();

private:
    void createRoom();
    void createTriangle();
    void createSphere();
    void createParticles();

    std::vector<Particle> current_particles;
    std::vector<Plane> planes;
    std::vector<Triangle> triangles;
    std::vector<Sphere> spheres;



    //particle parameters
    int num_particles_per_frame;
    //initial position of particles
    float initial_x = 0.0f;
    float initial_y = 0.5f;
    float initial_z = 0.0f;
    //life
    float dt = 0.01f;
    float tini = 0.0f;
    float lifetime = 5.0f;
    //moving
    float bouncing_par = 0.6f;
    float fountain_y = 3.0f;


    //room parameters
    float floor = 0.0f;
    float dimension = 0.5f;

    //triangles parameters
    glm::vec3 triangle_vertex1 = glm::vec3(0.2f, 0.6f, 0.4f);
    glm::vec3 triangle_vertex2 = glm::vec3(0.3f, 0.7f, 0.1f);
    glm::vec3 triangle_vertex3 = glm::vec3(-0.2f, 0.6f, 0.3f);

    //sphere parameters
    glm::vec3 sphere_center = glm::vec3(-0.1f, 0.3f, 0.1f );
    float sphere_radius = 0.2f;




};

#endif // ANIMATION_H
