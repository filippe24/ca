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


void MainWindow::on_action_Open_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open PLY"), ".", tr("*.ply"));

	ui->openGLWidget->loadMesh(filename);
}





void MainWindow::on_restart_clicked()
{
    ui->openGLWidget->resetAnimation();
}

void MainWindow::on_radius_spin_valueChanged(double arg1)
{
    ui->openGLWidget->update_radius(arg1);
}



void MainWindow::on_x_pos_valueChanged(double arg1)
{
    ui->openGLWidget->update_pos_x(arg1);
}
void MainWindow::on_y_pos_valueChanged(double arg1)
{
    ui->openGLWidget->update_pos_y(arg1);
}
void MainWindow::on_z_pos_valueChanged(double arg1)
{
    ui->openGLWidget->update_pos_z(arg1);
}


void MainWindow::on_radius_slider_valueChanged(int value)
{
    ui->openGLWidget->update_radius_int(value);
}

void MainWindow::on_pos_but_clicked()
{
    ui->openGLWidget->define_animation();
}

void MainWindow::on_fount_vel_valueChanged(double arg1)
{
    ui->openGLWidget->update_fountain_vel_y(arg1);
}


void MainWindow::on_fount_bu_clicked()
{
    ui->openGLWidget->define_animation();
}

void MainWindow::on_velocity_bu_toggled(bool checked)
{
    ui->openGLWidget->setFountainMode(not(checked));
}

void MainWindow::on_vel_x_spin_valueChanged(double arg1)
{
    ui->openGLWidget->update_vel_x(arg1);
}

void MainWindow::on_vel_y_spin_valueChanged(double arg1)
{
    ui->openGLWidget->update_vel_y(arg1);
}

void MainWindow::on_vel_z_spin_valueChanged(double arg1)
{
    ui->openGLWidget->update_vel_z(arg1);
}

void MainWindow::on_ke_spin_valueChanged(double arg1)
{
    ui->openGLWidget->set_elast_ke(arg1);
}

void MainWindow::on_l0_spin_valueChanged(double arg1)
{
    ui->openGLWidget->set_elast_l0(arg1);
}

void MainWindow::on_max_num_valueChanged(int arg1)
{
    ui->openGLWidget->update_max_num(arg1);
}

void MainWindow::on_kd_spin_valueChanged(double arg1)
{
    ui->openGLWidget->set_damp_kd(arg1);
}

void MainWindow::on_len_spring_valueChanged(int arg1)
{
    ui->openGLWidget->set_length_spring(arg1);
}

void MainWindow::on_set_spring_clicked()
{
    ui->openGLWidget->define_animation();
}
