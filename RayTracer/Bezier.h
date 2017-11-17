#pragma once
#include <cassert>

#include <QVector>
#include <QPointF>


class BezierCurve
{
public:
	QVector<QPointF> ctrl_points_;
	QVector<QPointF> samples_;

	size_t sample_size_;

	void de_casteljao();

	BezierCurve(size_t sampleSizeInit = 50);
	~BezierCurve() {}

	void push_ctrl_points(const QPointF& p);
	void pop_ctrl_points();
	size_t ctrl_points_size() const { return ctrl_points_.size(); }
	size_t samples_size() const { return samples_.size(); }

	const QPointF& at(size_t pos) const { return samples_[pos]; }
};

double BernsteinBaseFunc(int i, int n, double t);