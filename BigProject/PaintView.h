#pragma once
#include <memory>
#include <cmath>

#include <QWidget>


class QMouseEvent;
class QPaintEvent;
class QPainter;

class BezierCurve;
class Mesh;


enum RenderingStatus
{
	RenderingBezierCurve = 0,
	RenderingBezierModel,
};

class PaintView :
	public QWidget
{
	Q_OBJECT
public:
	PaintView();
	~PaintView();

protected:
	virtual void paintEvent(QPaintEvent * e);
	virtual void mousePressEvent(QMouseEvent *e);
private:
	std::shared_ptr<BezierCurve> bezier_curve_;
	std::shared_ptr<Mesh> pmesh_;
	bool is_mouse_enable_;
	RenderingStatus rendering_stat_;

	void draw_bezier_curve(QPainter& painter);
	void draw_bezier_model(QPainter& painter);
public slots:
	void on_start_rotate();
	void on_close_and_save();
signals:
	void finish();
};
