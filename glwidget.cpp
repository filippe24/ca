#include "glwidget.h"
#include <iostream>
#include <QApplication>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <plyreader.h>
#include <QTimer>


using namespace std;


const float rotationFactor = 0.5f;
const float maxRotationCamera = 75.0f;
const float minDistanceCamera = 1.0f;
const float maxDistanceCamera = 10.0f;

int counter = 0;
float current_time = 0.0f;


GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent), bPolygonFill(true), angleX(0.0f), angleY(0.0f), distance(4.0f)
{

    std::cout<< "0.glWidget"<<std::endl;

    program = NULL;
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    setFormat(format);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
    timer->start(33);

}

GLWidget::~GLWidget()
{
	if(program) delete program;
}


void GLWidget::initializeGL()
{
	initializeOpenGLFunctions();

    std::cout<< " 1. initialize GL" <<std::endl;

    particleMode = true;


    if(particleMode)
    {

        typedef void (APIENTRY *_glGenVertexArrays) (GLsizei, GLuint*);
        typedef void (APIENTRY *_glBindVertexArray) (GLuint);
        _glGenVertexArrays glGenVertexArrays;
         _glBindVertexArray glBindVertexArray;
        glGenVertexArrays = (_glGenVertexArrays) QOpenGLWidget::context()->getProcAddress("glGenVertexArrays");
        glBindVertexArray = (_glBindVertexArray) QOpenGLWidget::context()->getProcAddress("glBindVertexArray");

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        program = new QOpenGLShaderProgram();
        program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/particle.vert");
        program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/particle.frag");

        program->bindAttributeLocation("position",0);
        program->bindAttributeLocation("value",1);

        program->link();
        if(!program->isLinked())
        {
                cout << "Shader program has not linked" << endl << endl << "Log: " << endl << endl << program->log().toStdString();
                QApplication::quit();
        }
        program->bind();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);


       //initialize the quad
        std::cout << "defining the quad" << std::endl;
        GLfloat quadVertices[] =
        {
             // Positions
            0.1f,  -0.1f, 0.0f,
            0.1f,  0.1f,  0.0f,
            -0.1f, 0.1f,  0.0f,
            -0.1f, -0.1f, 0.0f,
        };

        //position
        glGenBuffers(1, &quadVBO);
        //Generate, bind and fill VBO for vertices
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);


        float valueBuffer[12]={
            //lx ly
            1.0, -1.0,
            1.0, 1.0,
            -1.0, 1.0,
            -1.0, -1.0,
        };

        //value
        glGenBuffers(1, &valueVBO);
        glBindBuffer(GL_ARRAY_BUFFER, valueVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(valueBuffer), &valueBuffer, GL_STATIC_DRAW);




        //initialize animation
        an = animation(5,0.01f,0.0f,0.0f,0.5f,0.0f);






    }
    else
    {
        program = new QOpenGLShaderProgram();
        program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simpleshader.vert");
        program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simpleshader.frag");
        program->link();
        if(!program->isLinked())
        {
                cout << "Shader program has not linked" << endl << endl << "Log: " << endl << endl << program->log().toStdString();
                QApplication::quit();
        }
        program->bind();

        mesh.buildCube();
        if(!mesh.init(program))
        {
                cout << "Could not create vbo" << endl;
                QApplication::quit();
        }

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);

    }


}

void GLWidget::resizeGL(int w, int h)
{
    std::cout<< " resize GL" <<std::endl;


	glViewport(0,0,w,h);
	setProjection((float)w/h);
	setModelview();
}

void GLWidget::paintGL()
{

    std::cout<< " 2. paint GL" <<std::endl;


    if(particleMode)
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(uint i= 0; i<positions.size(); i= i+3)
        {
            QVector3D par_pos;
            par_pos.setX(positions[i]);
            par_pos.setY(positions[i+1]);
            par_pos.setZ(positions[i+2]);

            float radius = 0.1f;


            program->bind();
            program->setUniformValue("color", QVector4D(0.75f, 0.8f, 0.9f, 1.0f));
            program->setUniformValue("particle_pos", par_pos);
            program->setUniformValue("radius", radius);

            glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

            enum
            {
                VERTICES,
                VALUE
            };

            std::cout << "***DRAWING PARTICLE *** iteration : " << i << std::endl;
            std::cout << "     in Position pos  x:"<< positions[i] << " y:" << positions[i+1] << " z:" << positions[i+2] << std::endl;
            std::cout << "     with number of Particles:" << positions.size()/3 << " and total number of coordinate:" << positions.size() << std::endl;


            glEnableVertexAttribArray(VERTICES);
            glBindBuffer(GL_ARRAY_BUFFER,quadVBO);
            glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

            glEnableVertexAttribArray(VALUE);
            glBindBuffer(GL_ARRAY_BUFFER,valueVBO);
            glVertexAttribPointer(VALUE, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


            //unbind
            glBindBuffer(GL_ARRAY_BUFFER,0);
            glDisableVertexAttribArray(0);


            program->release();


        }


    }
    else
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program->bind();
        program->setUniformValue("bLighting", bPolygonFill);

        if(bPolygonFill)
            program->setUniformValue("color", QVector4D(0.75f, 0.8f, 0.9f, 1.0f));
        else
        {
            program->setUniformValue("color", QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(0.5f, 1.0f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            //cube.render(*this);
            mesh.render(*this);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_POLYGON_OFFSET_FILL);
            program->setUniformValue("color", QVector4D(0.05, 0.05, 0.15, 1.0));
        }


        //cube.render(*this);
        mesh.render(*this);



        program->release();

    }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	lastMousePos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	// Rotation
	if(event->buttons() & Qt::LeftButton)
	{
		angleX += rotationFactor * (event->y() - lastMousePos.y());
		angleX = max(-maxRotationCamera, min(angleX, maxRotationCamera));
		angleY += rotationFactor * (event->x() - lastMousePos.x());
	}
	// Zoom
	if(event->buttons() & Qt::RightButton)
	{
		distance += 0.01f * (event->y() - lastMousePos.y());
		distance = max(minDistanceCamera, min(distance, maxDistanceCamera));
	}

    lastMousePos = event->pos();animation:

	makeCurrent();
	setModelview();
	doneCurrent();
	update();
}

void GLWidget::setProjection(float aspect)
{
    std::cout<< "       --set projection" <<std::endl;

	QMatrix4x4 projectionMatrix;

	projectionMatrix.perspective(60, aspect, 0.01, 100.0);
	program->bind();
	program->setUniformValue("projection", projectionMatrix);
	program->release();
}

void GLWidget::setModelview()
{

    std::cout<< "       --set model view" <<std::endl;

	QMatrix4x4 modelviewMatrix;

	modelviewMatrix.translate(0, 0, -distance);
	modelviewMatrix.rotate(angleX, 1.0f, 0.0f, 0.0f);
	modelviewMatrix.rotate(angleY, 0.0f, 1.0f, 0.0f);
	program->bind();
	program->setUniformValue("modelview", modelviewMatrix);
	program->setUniformValue("normalMatrix", modelviewMatrix.normalMatrix());
	program->release();
}

void GLWidget::setPolygonMode(bool bFill)
{

    std::cout<< "       --set polygonal mode" <<std::endl;

    bPolygonFill = bFill;

    makeCurrent();
    if(bFill)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    doneCurrent();
	update();
}

void GLWidget::setPartMode(bool bPart)
{

    std::cout<< "       --set particle mode" <<std::endl;

    particleMode = bPart;

    update();
}


void GLWidget::loadMesh(const QString &filename)
{
	PLYReader reader;

	mesh.destroy();
	reader.readMesh(filename, mesh);
	makeCurrent();
	if(!mesh.init(program))
	{
			cout << "Could not create vbo" << endl;
			QApplication::quit();
	}
	doneCurrent();
	update();
}

void GLWidget::addParticle(float x, float y, float z)
{
    positions.push_back(x);
    positions.push_back(y);
    positions.push_back(z);
}

void GLWidget::clearParticles()
{
    positions.clear();
}


void GLWidget::animate()
{
    positions.clear();
    positions = an.animate_frame();

    std::cout<< "animate:  initialized " << positions.size()/3 << " positions " << std::endl;
    std::cout<< "          and current time :" << current_time << " and time:" << time << std::endl;

    //adding particle animation
    if(counter < 30 )
    {
        an.addParticles(5);

    }

    current_time + 0.01f;
    counter ++;

    update();
}
