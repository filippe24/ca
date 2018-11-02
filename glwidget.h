#ifndef GLWIDGET_H
#define GLWIDGET_H


#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include "trianglemesh.h"
#include "animation.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
	GLWidget(QWidget *parent);
	~GLWidget();

	void loadMesh(const QString &filename);
	void setPolygonMode(bool bFill);
    void setPartMode(bool bPart);

    void resetAnimation();
    void define_animation();
    void clearParticles();

    void update_radius(float rad);
    void update_radius_int(int rad);
    void update_max_num(int num);
    void update_pos_x(float x_pos);
    void update_pos_y(float y_pos);
    void update_pos_z(float z_pos);
    void update_fountain_vel_y(float vy);

    void setFountainMode(bool b);
    void update_vel_x(float x_vel);
    void update_vel_y(float y_vel);
    void update_vel_z(float z_vel);

    void set_elast_ke(float ke);
    void set_elast_l0(float l0);
    void set_damp_kd(float kd);
    void set_length_spring(int s_l);
    void set_spring_param(float ke, float l0, float kd, int s_l);


public slots:
    void animate();

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	void setProjection(float aspect);
	void setModelview();

    void initializePlane(glm::vec3 rb, glm::vec3 rt, glm::vec3 lt, glm::vec3 lb);
    void initializeTriangle(glm::vec3 ver1, glm::vec3 ver2, glm::vec3 ver3);


private:
    bool bPolygonFill, particleMode;
	float angleX, angleY, distance;
	QPoint lastMousePos;

    QOpenGLShaderProgram *program, *programGeneral;
	TriangleMesh mesh;

    GLuint quad_vertex_buffer, particles_position_buffer, particles_color_buffer;
    GLuint quadVBO, valueVBO, quadTextCoordVBO,vao;

    std::vector<GLuint> planesVBO, trianglesVBO;

    animation an;

    GLuint activeId;
    GLuint particleShaderId,generalShaderId;

    //dimension of the room
    float room_dim_param = 2.0f;
    float floor = 0.0f;
    float roof = 2.0f;

    //triangle paramters
    glm::vec3 tri1 = glm::vec3(0.2f, 0.6f, 0.4f);
    glm::vec3 tri2 = glm::vec3(0.3f, 0.7f, 0.1f);
    glm::vec3 tri3 = glm::vec3(-0.2f, 0.6f, 0.3f);

    //gravity force
    float gravity = -9.8f;

    //PARTICLES PARAMETERS
    //radius of particles
    float radius = 0.02f;
    int num_part_per_frame =10;
    float part_lifetime = 5.0f;
    float part_bouncing = 0.9f;
    int max_num_of_part =  400;
    glm::vec3 par_initial_position = glm::vec3(0.0f, 1.5f, 0.0f);
    float par_fountain_y = 3.0f;
    bool fountain_mode = true;

    float par_i_velocity_x = 0.0f;
    float par_i_velocity_y = 0.0f;
    float par_i_velcoity_z = 0.0f;

    //SPHERE PARAMETERS
    glm::vec3 sphere_center = glm::vec3(-0.1f, 0.3f, 0.1f );
    float sphere_radius = 0.2f;
    float sphere_radius_delta = 0.1f;

    //SPRINGS
    //ELASTIC FORCE PARAMETERS
    bool spring1Dbool = false;
    int spring_lenght = 10;
    float elast_ke = 98;
    float elast_l = 0.1f;
    float damp_kd = 9.8f;

    //positions of all the particles in each frame
    std::vector<float> positions;





};

#endif // GLWIDGET_H
