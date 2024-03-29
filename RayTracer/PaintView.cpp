#include <fstream>

#include <QPainter>
#include <QMouseEvent>
#include <QInputDialog>

#include "PaintView.h"

#include "Bezier.h"
#include "Geometry.h"
#include "Mesh.h"
#include "tools.h"



PaintView::PaintView() :
	bezier_curve_(new BezierCurve),
	pmesh_(new Mesh),
	is_mouse_enable_(true),
	rendering_stat_(RenderingBezierCurve)
{
	setAttribute(Qt::WA_DeleteOnClose, false);
}

PaintView::~PaintView()
{
}

void PaintView::paintEvent(QPaintEvent * e)
{
	QPainter painter(this);
	painter.fillRect(0, 0, width(), height(), QColor(0, 0, 0));

	switch (rendering_stat_)
	{
	case RenderingBezierCurve:
		draw_bezier_curve(painter);
		break;
	case RenderingBezierModel:
		draw_bezier_model(painter);
		break;
	default:
		break;
	}
	painter.end();
}

void PaintView::mousePressEvent(QMouseEvent * e)
{
	qreal x = e->localPos().x();
	qreal y = e->localPos().y();

	if (e->button() == Qt::LeftButton)
	{
		if (!is_mouse_enable_) return;
		if (bezier_curve_->ctrl_points_size() == 0)
		{
			if (std::fabs(x - width() * 0.5) <= 3.0)
			{
				bezier_curve_->push_ctrl_points(QPointF(0, y));
			}
		}
		else
		{
			if (std::fabs(x - width() * 0.5) <= 3.0)
			{
				is_mouse_enable_ = false;
				bezier_curve_->push_ctrl_points(QPointF(0.0, y));
			}
			else
			{
				if (x < width() * 0.5) return;
				bezier_curve_->push_ctrl_points(QPointF(x - width() * 0.5, y));
			}
		}
	}
	else if (e->button() == Qt::RightButton)
	{
		is_mouse_enable_ = true;
		if (bezier_curve_->ctrl_points_size() > 0)
		{
			bezier_curve_->pop_ctrl_points();
		}
	}

	update();
}

void PaintView::draw_bezier_curve(QPainter& painter)
{
	painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::RoundCap));
	painter.drawLine(QPointF(width() * 0.5, height() * 0.1), QPoint(width() * 0.5, height() * 0.9));

	painter.setPen(QPen(Qt::darkGreen, 1, Qt::SolidLine, Qt::RoundCap));
	for (int i = 0; i < bezier_curve_->samples_size(); ++i)
	{
		qreal x = bezier_curve_->at(i).x();
		qreal y = bezier_curve_->at(i).y();

		painter.drawPoint(QPointF(x + width() * 0.5, y));
	}
}

void PaintView::draw_bezier_model(QPainter& painter)
{
	painter.setPen(QPen(Qt::darkGreen, 1, Qt::SolidLine, Qt::RoundCap));
	for (int i = 1; i <= pmesh_->trifacets_num(); ++i)
	{
		const TriFacet& f = pmesh_->trifacet_at(i);
		QPointF p[3];
		p[0] = orthogonal_project(
			pmesh_->vertice_at(std::get<0>(f))) + QPointF(width() * 0.5, 0);
		p[1] = orthogonal_project(
			pmesh_->vertice_at(std::get<1>(f))) + QPointF(width() * 0.5, 0);
		p[2] = orthogonal_project(
			pmesh_->vertice_at(std::get<2>(f))) + QPointF(width() * 0.5, 0);
		painter.drawPolyline(p, 3);
	}

	for (int i = 1; i <= pmesh_->quadfacets_num(); ++i)
	{
		const QuadFacet& f = pmesh_->quadfacet_at(i);
		QPointF p[4];
		p[0] = orthogonal_project(
			pmesh_->vertice_at(std::get<0>(f))) + QPointF(width() * 0.5, 0);
		p[1] = orthogonal_project(
			pmesh_->vertice_at(std::get<1>(f))) + QPointF(width() * 0.5, 0);
		p[2] = orthogonal_project(
			pmesh_->vertice_at(std::get<2>(f))) + QPointF(width() * 0.5, 0);
		p[3] = orthogonal_project(
			pmesh_->vertice_at(std::get<3>(f))) + QPointF(width() * 0.5, 0);
		painter.drawPolyline(p, 4);
	}
}

void PaintView::on_start_rotate()
{
	convert(*pmesh_, *bezier_curve_, 72);
	rendering_stat_ = RenderingBezierModel;
	update();
}

void PaintView::on_close_and_save()
{
	bool ok;
	QString text = QInputDialog::getText(this, 
		tr("Save"),
		tr("file name:"),
		QLineEdit::Normal,
		tr("RotationModel.obj"), 
		&ok);

	if (ok && !text.isEmpty())
	{
		pmesh_->save(qPrintable(text));
		emit finish();
	}

}
