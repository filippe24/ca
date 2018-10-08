#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_action_Quit_triggered()
{
	 QApplication::quit();
}

void MainWindow::on_checkBoxFill_toggled(bool checked)
{
	 ui->openGLWidget->setPolygonMode(checked);
}


void MainWindow::on_checkBoxPart_toggled(bool checked)
{
     ui->openGLWidget->setPolygonMode(checked);
}


void MainWindow::on_action_Open_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open PLY"), ".", tr("*.ply"));

	ui->openGLWidget->loadMesh(filename);
}

void MainWindow::addParticles(float x, float y, float z)
{
    ui->openGLWidget->addParticle(x,y,z);
}

void MainWindow::clearParticles()
{
    ui->openGLWidget->clearParticles();
}
