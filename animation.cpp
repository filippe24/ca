#include <iostream>
#include <vector>
#include "particle.h"
#include "geometry.h"
#include "animation.h"

animation::animation()
{

}

animation::animation(int num_par, float delta_time, float initial_time, float x , float y , float z )
{

    // define a floor plane for collision
    glm::vec3 punt(0.0f);
    glm::vec3 normal(0, 1, 0);
    Plane plane(punt, normal);
    planes.push_back(plane);

    int num_particles = num_par;

    dt = delta_time;

    tini = initial_time;

    initial_x = x;
    initial_y = y;
    initial_z = z;


    // particles creation
    for (int i = 0; i < num_particles; i++)
    {
        Particle p1(initial_x, initial_y, initial_z); //initial position of the particle

        p1.setLifetime(5.0f);
        std::cout << "Lifetime =" << p1.getLifetime() << std::endl;
        p1.setBouncing(0.8f);
        p1.addForce(0, -9.8f, 0);
        //	p.setFixed(true);

        p1.setVelocity((((double) rand() / (RAND_MAX))  - 0.5f), 5, (((double) rand() / (RAND_MAX))  - 0.5f));
        std::cout<< "Initial Velocity  x: " << p1.getVelocity().x << " y: " << p1.getVelocity().y <<" z: " << p1.getVelocity().z << std::endl;

        current_particles.push_back(p1);

    }

    std::cout<<"animation.ccp:  number of particles : "<< num_particles << " created particles :" << current_particles.size() << std::endl;



}

void animation::addParticles(int new_particles)
{
    std::cout<<" addParticles:      new particles :" << new_particles <<std::endl;
    std::cout<<" addParticles:      current_particles before : " << current_particles.size() << std::endl;

    // particles creation
    for (int i = 0; i < new_particles; i++)
    {
        Particle p1(initial_x, initial_y, initial_z); //initial position of the particle

        p1.setLifetime(5.0f);
        std::cout <<" addParticles:      Lifetime =" << p1.getLifetime() << std::endl;
        p1.setBouncing(0.8f);
        p1.addForce(0, -9.8f, 0);
        //	p.setFixed(true);

        p1.setVelocity((((double) rand() / (RAND_MAX))  - 0.5f), 5, (((double) rand() / (RAND_MAX))  - 0.5f));
        std::cout<<" addParticles:      Initial Velocity  x: " << p1.getVelocity().x << " y: " << p1.getVelocity().y <<" z: " << p1.getVelocity().z << std::endl;

        current_particles.push_back(p1);


    }
    std::cout<<" addParticles:      current_particles after : " << current_particles.size() << std::endl;
}





std::vector<float> animation::animate_frame()
{

    std::vector<float> returned_positions;

    std::cout<<"animation.ccp:animate:  number of  created particles :" << current_particles.size() << std::endl;


    Plane plane = planes[0];

    for (uint i = 0; i < current_particles.size(); i++)
    {
        std::cout << "num of particles iteration i:" <<i << std::endl;
        Particle p = current_particles[i];
        // simulation loop
        int count = 0;
        float disact, disant;
        disact = plane.distPoint2Plane(p.getCurrentPosition());

        if (p.getLifetime() > 0) {

                p.updateParticle(dt, Particle::UpdateMethod::EulerOrig);
                std::cout << "lifetime = " << p.getLifetime() << std::endl;
                std::cout << "position = " << p.getCurrentPosition().x << "  " << p.getCurrentPosition().y
                    << "  " << p.getCurrentPosition().z << "  time = " << time << std::endl;


                //Check for floor collisions
                disant = disact;
                p.setLifetime(p.getLifetime() - dt);
                disact = plane.distPoint2Plane(p.getCurrentPosition());

                if (disant*disact < 0.0f) {
                    //VERY IMPORTANT: only valid for the plane y=0 (floor plane)
                    //Must be addapted to a general plane,
                    p.setPosition(p.getCurrentPosition().x, -p.getCurrentPosition().y, p.getCurrentPosition().z);
                    p.setVelocity(p.getVelocity().x, -p.getVelocity().y, p.getVelocity().z);
                    std::cout << "Bounce = " << count++ << std::endl;
                    disact = -disact;

                }

                current_particles[i] = p;

                float x = p.getCurrentPosition().x;
                float y = p.getCurrentPosition().y;
                float z = p.getCurrentPosition().z;

                returned_positions.push_back(x);
                returned_positions.push_back(y);
                returned_positions.push_back(z);


            }

    }

    std::cout<<"animation.ccp:animate created particles :" << returned_positions.size() << std::endl;
    return returned_positions;

}


