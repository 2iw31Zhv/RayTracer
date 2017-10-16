#pragma once
#include <memory>
#include <cmath>

#include <QWidget>


class QMouseEvent;
class QPaintEvent;
class QPainter;

class BesselCurve;
class Mesh;


enum RenderingStatus
{
	RenderingBesselCurve = 0,
	RenderingBesselModel,
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
	std::shared_ptr<BesselCurve> bessel_curve_;
	std::shared_ptr<Mesh> pmesh_;
	bool is_mouse_enable_;
	RenderingStatus rendering_stat_;

	void draw_bessel_curve(QPainter& painter);
	void draw_bessel_model(QPainter& painter);
public slots:
	void on_start_rotate();
	void on_close_and_save();
signals:
	void finish();
};
