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

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
