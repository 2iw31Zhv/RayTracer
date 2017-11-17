#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QtWidgets/QMainWindow>

#include "ui_mainwindow.h"

class PaintView;
class RayTracingScene;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindowClass ui;
	std::shared_ptr<PaintView> paint_view_;
	std::shared_ptr<RayTracingScene> ray_tracing_scene_;

	bool is_paint_view_;
public slots:
	void on_rotation_model();
	void on_rotation_end();
	void on_new_scene();
	void on_ray_tracing();
};

#endif // MAINWINDOW_H
