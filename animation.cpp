#include <iostream>
#include <vector>
#include "particle.h"
#include "geometry.h"
#include "animation.h"

animation::animation()
{

}



void animation::initializeValues()
{
    planes.clear();
    triangles.clear();
    spheres.clear();
    triangles.clear();
    current_particles.clear();
    createRoom();
    createTriangle();
    createSphere();
    createParticles();

}

void animation::setUpdateMode(int selected)
{
    if(selected == 0)
        current_method = Particle::UpdateMethod::EulerOrig;
    else if(selected == 1)
        current_method = Particle::UpdateMethod::EulerSemi;
    else if(selected == 2)
        current_method = Particle::UpdateMethod::Verlet;
    else
        current_method = Particle::UpdateMethod::EulerSemi;
}
void animation::setParticleParam(int num_part, float life, float bounce)
{
    std::cout << "set " <<num_part << " number of particles" << std::endl;
    num_particles_per_frame = num_part;
    lifetime = life;
    bouncing_par = bounce;
}
void animation::setFountain(float x, float y, float z, float fount_y)
{
    initial_x = x;
    initial_y = y;
    initial_z = z;
    fountain_y = fount_y;
}
void animation::setInitialVelocity(float vx, float vy, float vz)
{
    velocity_x = vx;
    velocity_y = vy;
    velocity_z = vz;
}
void animation::setFountainMode(bool b)
{
    fountain_b = b;
}
void animation::createParticles()
{
    addParticles(num_particles_per_frame);
}
void animation::clearParticles()
{
    current_particles.clear();
}

void animation::setSphereParam(glm::vec3 cent, float rad)
{
    sphere_center = cent;
    sphere_radius = rad;
    createSphere();
}
void animation::createSphere()
{
    spheres.clear();
    //define the sphere for collision
    Sphere sphere(sphere_center, sphere_radius);
    spheres.push_back(sphere);
}

void animation::setTriangleParam(glm::vec3 ver1, glm::vec3 ver2, glm::vec3 ver3)
{
    triangle_vertex1 = ver1;
    triangle_vertex2 = ver2;
    triangle_vertex3 = ver3;
    createTriangle();
}
void animation::createTriangle()
{
    triangles.clear();

    Triangle triangle1(triangle_vertex1, triangle_vertex2, triangle_vertex3);
    triangles.push_back(triangle1);
}


void animation::setRoomParam(float f, float d)
{
    floor = f;
    dimension = d;
    createRoom();
}
void animation::createRoom()
{
    planes.clear();

    //room parameters
    float f = floor;
    float d = dimension;

    // define planes for collision
    //floor
    glm::vec3 punt(0.0f, f, 0.0f);
    glm::vec3 normal(0, 1, 0);
    Plane plane(punt, normal);
    planes.push_back(plane);
    //right
    glm::vec3 point2(d, 0.0f, 0.0f);
    glm::vec3 normal2(-1, 0, 0);
    Plane plane2(point2, normal2);
    planes.push_back(plane2);
    //left
    glm::vec3 point3(-d, 0.0f, 0.0f);
    glm::vec3 normal3(1, 0, 0);
    Plane plane3(point3, normal3);
    planes.push_back(plane3);
    //back
    glm::vec3 point4(0.0f, 0.0f, d);
    glm::vec3 normal4(0, 0, -1);
    Plane plane4(point4, normal4);
    planes.push_back(plane4);
    //front
    glm::vec3 point5(0.0f, 0.0f, -d);
    glm::vec3 normal5(0, 0, 1);
    Plane plane5(point5, normal5);
    planes.push_back(plane5);
}




void animation::setGravityPatam(float grav)
{
    g_a = grav;
}
void animation::setSpringMode(bool s_b,float ke_param, float l0_param, float kd_param, int s_l)
{
    spring1D_b = s_b;
    k_e = ke_param;
    l_0 = l0_param;
    k_d = kd_param;
    length_spring1D = s_l;
}
void animation::set2DSpringMode(bool s2d_b)
{
    spring2D_b = s2d_b;
}




void animation::addParticles(int new_particles)
{
    // particles creation
    if(spring1D_b)
        new_particles = length_spring1D;

    for (int i = 0; i < new_particles; i++)
    {
        Particle p1(initial_x, initial_y, initial_z); //initial position of the particle
        p1.setLifetime(lifetime);
        p1.setBouncing(bouncing_par);
        p1.addForce(0, g_a, 0);
        float vx,vy,vz;
        vx = velocity_x;
        vy = velocity_y;
        vz = velocity_z;

        //SPRING MODE
        if(spring1D_b)
        {
            //1D: the gravity is 0 because is added after computing the spring forces
            if(not(spring2D_b))
            {
                p1.setPosition(initial_x,initial_y, initial_z + i*l_0);
                p1.setForce(0.0,0.0,0.0);
                p1.setVelocity(0.0, 0.0, 0.0);
                current_particles.push_back(p1);
            }
            //2D MODE: create a matrix with lenght_spring1D rows and lenght_spring2d columns
            else
            {
                for(int j = 0; j < length_spring2D ; j++)
                {
                    p1.setPosition(initial_x + j*l_0 ,initial_y, initial_z + i*l_0);
                    p1.setForce(0.0,0.0,0.0);
                    p1.setVelocity(0.0, 0.0, 0.0);
                    current_particles.push_back(p1);
                }
            }
        }
        //fountain mode
        else if(fountain_b)
        {
            vx = velocity_x + (((double) rand() / (RAND_MAX))  - 0.5f) ;
            vy = velocity_y + fountain_y;
            vz = velocity_z + (((double) rand() / (RAND_MAX))  - 0.5f);
            p1.setVelocity(vx, vy, vz);
            current_particles.push_back(p1);
        }
        //velocity mode
        else
        {
            p1.setVelocity(vx, vy, vz);
            current_particles.push_back(p1);
        }
    }
}





std::vector<float> animation::animate_frame()
{
    //we compute the 1dor 2d spring for all the particles in order, before checking for collision
    if(spring1D_b)
    {
        if(spring2D_b)
            compute_spring2D();
        else
            compute_spring1D();
    }


    std::vector<float> returned_positions;
    for (uint i = 0; i < current_particles.size(); i++)
    {
        Particle p = current_particles[i];


        // simulation loop
        if (p.getLifetime() > 0) {

            if(current_method == Particle::UpdateMethod::Verlet)
            {
                if(p.getLifetime() > (lifetime - dt))
                {
                    std::cout << "verlet, set previous position for particle " << i << " at lifetime " << p.getLifetime() << "  of " << lifetime << std::endl;
                    glm::vec3 ini_pos = p.getCurrentPosition();
                    glm::vec3 ini_vel = p.getVelocity();
                    p.setPreviousPosition(ini_pos.x - (ini_vel.x)*dt, ini_pos.y - (ini_vel.y)*dt, ini_pos.z - (ini_vel.z)*dt);
                }
            }
            p.updateParticle(dt, current_method);
            p.setLifetime(p.getLifetime() - dt);

            //**********************
            //**floor collisions****
            //**********************
            for (uint p_j = 0; p_j < planes.size(); p_j++)
            {
                float disact, disant;

                Plane plane = planes[p_j];
                disant = plane.distPoint2Plane(p.getPreviousPosition());
                disact = plane.distPoint2Plane(p.getCurrentPosition());


                if (disant*disact < 0.0f)
                {

                    float b = p.getBouncing();

                    glm::vec3 n = plane.normal;
                    glm::vec3 pos = p.getCurrentPosition();
                    glm::vec3 newPos = (glm::dot(pos,n) + plane.dconst)*n;
                    p.setPosition(pos.x - (1+b)*newPos.x, pos.y - (1+b)*newPos.y, pos.z - (1+b)*newPos.z);

                    glm::vec3 vel = p.getVelocity();
                    glm::vec3 newVel = (glm::dot(vel,n))*n;
                    p.setVelocity((vel.x - (1+b)*newVel.x), (vel.y - (1+b)*newVel.y), (vel.z - (1+b)*newVel.z));

                    //update of previous position for velrlet (maybe not necessary)
                    if(current_method == Particle::UpdateMethod::Verlet)
                    {
                        glm::vec3 normal_plane = planes[p_j].normal;
                        // use distance with previous plane: disant
                        glm::vec3 normal_x_distance = normal_plane*disant;
                        glm::vec3 prev_pos = p.getPreviousPosition();
                        glm::vec3 n_o_p = prev_pos - 2.0f*normal_x_distance;
                        p.setPreviousPosition(n_o_p);
                    }
                }
            }

            //**********************
            //**triangle collision*
            //**********************
            for (uint t_i = 0; t_i < triangles.size(); t_i++)
            {
                float disact, disant;

                Triangle t = triangles[t_i];
                disant = t.distPoint2Plane(p.getPreviousPosition());
                disact = t.distPoint2Plane(p.getCurrentPosition());
                if(disant*disact < 0.0f)
                {
                    if ( t.isInside(p.getCurrentPosition()))
                    {


                        float b = p.getBouncing();

                        glm::vec3 n = t.normal;
                        glm::vec3 pos = p.getCurrentPosition();
                        glm::vec3 newPos = (glm::dot(pos,n) + t.dconst)*n;
                        p.setPosition(pos.x - (1+b)*newPos.x, pos.y - (1+b)*newPos.y, pos.z - (1+b)*newPos.z);

                        glm::vec3 vel = p.getVelocity();
                        glm::vec3 newVel = (glm::dot(vel,n))*n;
                        p.setVelocity((vel.x - (1+b)*newVel.x), (vel.y - (1+b)*newVel.y), (vel.z - (1+b)*newVel.z));

                        //update of previous position for velrlet (maybe not necessary)
                        if(current_method == Particle::UpdateMethod::Verlet)
                        {
                            glm::vec3 normal_plane = t.normal;
                            // use distance with previous plane: disant
                            glm::vec3 normal_x_distance = normal_plane*disant;
                            glm::vec3 prev_pos = p.getPreviousPosition();
                            glm::vec3 n_o_p = prev_pos - 2.0f*normal_x_distance;
                            p.setPreviousPosition(n_o_p);
                        }
                    }
                }


            }

            //**********************
            //**sphere collision*
            //**********************
            for (uint s_i = 0; s_i < spheres.size(); s_i++)
            {
                bool inOld = spheres[s_i].isInside(p.getPreviousPosition());
                bool inNew = spheres[s_i].isInside(p.getCurrentPosition());

                if(!inOld and inNew)
                {
                    //computing the intersection
                    glm::vec3 interPoint, interPoint1, interPoint2;
                    float lambda1, lambda2;
                    float rad = spheres[s_i].radius;
                    glm::vec3 p_old = p.getPreviousPosition();
                    glm::vec3 p_new = p.getCurrentPosition();
                    glm::vec3 c = spheres[s_i].center;
                    glm::vec3 v = p_new - p_old;
                    float alpha = glm::dot(v, v);
                    float beta = 2*glm::dot((v), (p_old - c));
                    float gamma = glm::dot(c,c) + glm::dot(p_old,p_old) - 2*glm::dot(p_old, c) - (rad*rad);

                    float delta = beta*beta - 4*alpha*gamma;
                    if(delta >= 0)
                    {
                        lambda1 = -beta + sqrt(delta);
                        lambda2 = -beta - sqrt(delta);



                        interPoint1 = p_old + lambda1*v;
                        interPoint2 = p_old + lambda2*v;
                        //default value
                        interPoint = interPoint1;

                    }
                    else
                        std::cerr << "segment-sphere intersection have no solution" << std::endl;

                    if(lambda1 >= 0.0f and lambda1 <= 1.0f)
                        interPoint = interPoint1;
                    else if(lambda2 >= 0.0f and lambda2 <= 1.0f)
                        interPoint = interPoint2;
                    else
                        std::cerr << "segment-sphere intersection : none of the 2 lambdas is between 0 and 1"<< std::endl;


                    glm::vec3 norm_vec = glm::normalize(interPoint - c);
                    Plane tang_plane(interPoint, norm_vec);

                    //bouncing
                    float b = p.getBouncing();

                    glm::vec3 pos = p.getCurrentPosition();

                    glm::vec3 newPos = (glm::dot(pos,norm_vec) + tang_plane.dconst)*norm_vec;
                    p.setPosition(pos.x - (1+b)*newPos.x, pos.y - (1+b)*newPos.y, pos.z - (1+b)*newPos.z);

                    glm::vec3 vel = p.getVelocity();

                    glm::vec3 newVel = (glm::dot(vel,norm_vec))*norm_vec;
                    p.setVelocity((vel.x - (1+b)*newVel.x), (vel.y - (1+b)*newVel.y), (vel.z - (1+b)*newVel.z));

                }
                if(inNew)
                {
                            //std::cerr << "NOW INSIDE THE SPHERE" << std::endl;
                }
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

    return returned_positions;

}

glm::vec3 animation::spring_force(int i, int j, float l0, float ke, float kd)
{
    //check for division by 0
    if((distance_particles(i,j) > 0 + l_0/2) or (distance_particles(i,j) < 0 - l_0/2))
    {
        Particle p_i = current_particles[i];
        Particle p_j = current_particles[j];
        glm::vec3 v_i = p_i.getVelocity();
        glm::vec3 v_j = p_j.getVelocity();
        glm::vec3 x_i = p_i.getCurrentPosition();
        glm::vec3 x_j = p_j.getCurrentPosition();

        glm::vec3 direction = (x_j - x_i)/(glm::length(x_j - x_i));
        glm::vec3 returned = (ke*(glm::length(x_j - x_i) - l0) + kd*(v_j - v_i)*direction)*direction ;
        return returned;
     }
    else
        return glm::vec3(0.0,0.0,0.0);

}
float animation::distance_particles(int i, int j)
{
    Particle p_i = current_particles[i];
    Particle p_j = current_particles[j];
    glm::vec3 x_i = p_i.getCurrentPosition();
    glm::vec3 x_j = p_j.getCurrentPosition();
    return glm::length(x_j-x_i);
}
glm::vec3 animation::elastic_force(int i, int j)
{
    Particle p_i = current_particles[i];
    Particle p_j = current_particles[j];
    glm::vec3 x_i = p_i.getCurrentPosition();
    glm::vec3 x_j = p_j.getCurrentPosition();
    return k_e*(glm::length(x_j - x_i) - l_0)*((x_j - x_i)/glm::length(x_j - x_i));
}

glm::vec3 animation::damping_force(int i, int j)
{
    Particle p_i = current_particles[i];
    Particle p_j = current_particles[j];
    glm::vec3 v_i = p_i.getVelocity();
    glm::vec3 v_j = p_j.getVelocity();
    glm::vec3 x_i = p_i.getCurrentPosition();
    glm::vec3 x_j = p_j.getCurrentPosition();
    return k_d*(v_j - v_i)*(x_j - x_i)/(glm::length(x_j - x_i));
}



//precomputation of the spring force in 1D for each particle with the previous and the next
void animation::compute_spring1D()
{
    glm::vec3 spring_force_prev;
    for(uint i = 0; i < current_particles.size(); i++)
    {
        Particle p = current_particles[i];
        if(p.getLifetime() > 0)
        {
            //spring
            p.setForce(0.0,0.0,0.0);
            if(i == 0)
            {
                //first particle is fixed
                p.setPosition(initial_x, initial_y, initial_z);
                p.setForce(0.0,0.0,0.0);
                glm::vec3 spring_force_next = spring_force(i,i+1, l_0, k_e, k_d);
                spring_force_prev = -spring_force_next;
            }
            else if(i == current_particles.size()-1)
            {
                //last particle
                p.addForce(spring_force_prev);
            }
            else
            {
                //internal particles
                glm::vec3 spring_force_next = spring_force(i,i+1, l_0, k_e, k_d);
                p.addForce(spring_force_prev);
                p.addForce(spring_force_next);
                spring_force_prev = -spring_force_next;

            }
            if(i!=0)
                p.addForce(0.0,g_a,0.0);
        }
        current_particles[i] = p;
    }
}

//precomputation of the 2d spring interactions
//the 2d structures is based on a matrix formed by lenght_spring1D x lenght_spring2D
//all of this is saved in the current_particles vector in orderto be printed
//we need the access the i and j index to define the forces
void animation::compute_spring2D()
{
    glm::vec3 streach_prev;
    std::vector<glm::vec3> spring_force_down;
    spring_force_down.resize(length_spring1D);

    std::vector<glm::vec3> spring_force_up_l;
    std::vector<glm::vec3> spring_force_up_r;
    spring_force_up_l.resize(length_spring2D);
    spring_force_up_r.resize(length_spring2D);

    for(uint p_ = 0; p_ < current_particles.size(); p_++)
    {
        Particle p = current_particles[p_];
        p.setForce(0.0,0.0,0.0);

        if(p.getLifetime() > 0)
        {
            int i = (int) p_ / length_spring2D;
            int j = (int) p_ % length_spring2D;
            std::cout << " particle :" << p_ << " with indices: (" << i << ", " << j << ")" << std::endl;

            //STREACH
            //row
            if(j == 0)
            {
                //first particle of row
                int p_next = index2D(i,j+1);
                glm::vec3 streach_next = spring_force(p_,p_next, l_0, k_e, k_d);
                p.addForce(streach_next);
                streach_prev = -streach_next;
                std::cout << "next particle is: " << p_next << std::endl;
            }
            else if(j == length_spring2D-1)
            {
                //last particle of row
                p.addForce(streach_prev);
            }
            else
            {
                //internal particles of row
                int p_next = index2D(i,j+1);
                glm::vec3 streach_next =  spring_force(p_,p_next, l_0, k_e, k_d);
                p.addForce(streach_prev);
                p.addForce(streach_next);
                streach_prev = -streach_next;
                std::cout << "next particle is: " << p_next << std::endl;

            }
            //columns
            if(i == 0)
            {
                //first particle down of column
                int p_up = index2D(i+1,j);
                glm::vec3 spring_force_up = spring_force(p_,p_up, l_0, k_e, k_d);
                p.addForce(spring_force_up);
                spring_force_down[j] = -spring_force_up;
            }
            else if(i == length_spring1D-1)
            {
                //last particle up of column
                p.addForce(spring_force_down[j]);
            }
            else
            {
                //internal particles of column
                int p_up = index2D(i+1,j);
                glm::vec3 spring_force_up = spring_force(p_,p_up, l_0, k_e, k_d);
                p.addForce(spring_force_down[j]);
                p.addForce(spring_force_up);
                spring_force_down[j] = -spring_force_up;
            }



            //SHEAR
            if((i != 0) and(j != 0))
            {
                int p_up_l = index2D(i-1, j-1);
                glm::vec3 shear_1 = -spring_force(p_, p_up_l, l_0*1.41f,k_e, k_d);
                p.addForce(spring_force_up_l[j-1]);

            }
            if((i != 0) and (j != length_spring2D-1))
            {
                int p_up_r = index2D(i-1, j+1);
                glm::vec3 shear_2 = -spring_force(p_up_r, p_,l_0*1.41f,k_e, k_d);
                p.addForce(spring_force_up_r[j+1]);

            }
            if((i != length_spring1D-1) and (j != 0))
            {
                int p_dw_l = index2D(i+1, j-1);
                glm::vec3 shear_3 = spring_force(p_, p_dw_l,l_0*1.41f,k_e, k_d);
                spring_force_up_r[j] = -shear_3;
                p.addForce(shear_3);
            }
            if((i != length_spring1D-1) and (j != length_spring2D-1))
            {
                int p_dw_r = index2D(i+1, j+1);
                glm::vec3 shear_4 = spring_force(p_, p_dw_r,l_0*1.41f,k_e, k_d);
                spring_force_up_l[j] = -shear_4;
                p.addForce(shear_4);
            }


            //fix the particles

            if(((i==0) and (j == 0)) or ((i==0) and (j==length_spring2D-1)))
            {
                p.setFixed(true);
                p.setForce(0,0,0);
            }




//            //BEND
//            if(i > 1)
//            {
//                int p_2prev = index2D(i-2,j);
//                glm::vec3 bend_1 = -spring_force(p_2prev, p_);
//                p.addForce(bend_1);
//            }
//            if(i < length_spring1D-2)
//            {
//                int p_2next = index2D(i+2,j);
//                glm::vec3 bend_2 = spring_force(p_, p_2next);
//                p.addForce(bend_2);
//            }
//            if(j > 1)
//            {
//                int p_2up = index2D(i,j-2);
//                glm::vec3 bend_3 = -spring_force(p_2up, p_);
//                p.addForce(bend_3);
//            }
//            if( j < length_spring2D-2)
//            {
//                int p_2down = index2D(i, j+2);
//                glm::vec3 bend_4 = spring_force(p_, p_2down);
//                p.addForce(bend_4);
//            }


            p.addForce(0.0,g_a,0.0);

        }
        current_particles[p_] = p;
    }
}
//give back the index of the vector current_particles, correspondent to the matrix position (i,j)
int animation::index2D(int i,int j)
{
    return i*length_spring2D + j;
}
