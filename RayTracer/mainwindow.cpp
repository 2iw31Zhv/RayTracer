#include "mainwindow.h"

#include "PaintView.h"
#include "RayTracingScene.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	paint_view_(nullptr),
	ray_tracing_scene_(nullptr),
	is_paint_view_(false)
{
	ui.setupUi(this);


	connect(ui.actionBy_Rotation, SIGNAL(triggered()), this, SLOT(on_rotation_model()));
	connect(ui.actionNew_Scene, SIGNAL(triggered()), this, SLOT(on_new_scene()));
	connect(ui.actionRay_tracing, SIGNAL(triggered()), this, SLOT(on_ray_tracing()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::on_rotation_model()
{
	if (is_paint_view_) return;

	paint_view_.reset(new PaintView);
	paint_view_->resize(this->size());
	setCentralWidget(paint_view_.get());
	connect(paint_view_.get(), SIGNAL(finish()), this, SLOT(on_rotation_end()));

	ui.mainToolBar->addAction("Start", paint_view_.get(), SLOT(on_start_rotate()));
	ui.mainToolBar->addAction("Save and Close", paint_view_.get(), SLOT(on_close_and_save()));

	is_paint_view_ = true;
}

void MainWindow::on_rotation_end()
{
	ui.mainToolBar->clear();
	paint_view_->close();
	is_paint_view_ = false;
}

void MainWindow::on_new_scene()
{
	ray_tracing_scene_.reset(new RayTracingScene);
	ray_tracing_scene_->resize(this->size());
	setCentralWidget(ray_tracing_scene_.get());
}

void MainWindow::on_ray_tracing()
{
	ray_tracing_scene_->precompute();
	ray_tracing_scene_->update();
}