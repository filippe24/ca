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


    //positions of all the particles in each frame
    std::vector<float> positions;
    //maximum number of partilces available
    uint MaxParticles = 5;

    //add a Particles to the list of particles
    void addParticle(float x, float y, float z);
    void clearParticles();

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

private:
    bool bPolygonFill, particleMode;
	float angleX, angleY, distance;
	QPoint lastMousePos;

	QOpenGLShaderProgram *program;
	TriangleMesh mesh;

    GLuint quad_vertex_buffer, particles_position_buffer, particles_color_buffer;
    GLuint quadVBO, valueVBO, quadTextCoordVBO,vao;

    animation an;

};

#endif // GLWIDGET_H
