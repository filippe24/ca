#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

    void addParticles(float x, float y, float z);
    void clearParticles();

private slots:
	void on_action_Quit_triggered();


	void on_action_Open_triggered();


    void on_restart_clicked();

    void on_radius_spin_valueChanged(double arg1);

    void on_z_pos_valueChanged(double arg1);

    void on_y_pos_valueChanged(double arg1);

    void on_x_pos_valueChanged(double arg1);



    void on_radius_slider_valueChanged(int value);

    void on_pos_but_clicked();

    void on_fount_vel_valueChanged(double arg1);


    void on_fount_bu_clicked();

    void on_velocity_bu_toggled(bool checked);

    void on_vel_x_spin_valueChanged(double arg1);

    void on_vel_y_spin_valueChanged(double arg1);

    void on_vel_z_spin_valueChanged(double arg1);

    void on_ke_spin_valueChanged(double arg1);

    void on_l0_spin_valueChanged(double arg1);

    void on_max_num_valueChanged(int arg1);

    void on_kd_spin_valueChanged(double arg1);

    void on_len_spring_valueChanged(int arg1);

    void on_set_spring_clicked();

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
