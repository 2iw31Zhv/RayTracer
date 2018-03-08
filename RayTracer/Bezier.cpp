#include "Bezier.h"


BezierCurve::BezierCurve(size_t sampleSizeInit) :
	sample_size_(sampleSizeInit),
	samples_(sampleSizeInit)
{

}

void BezierCurve::push_ctrl_points(const QPointF& p) 
{ 
	ctrl_points_.push_back(p);
	de_casteljao();
}

void BezierCurve::pop_ctrl_points() 
{ 
	ctrl_points_.pop_back();
	de_casteljao();
}

const QPointF _de_casteljao_impl(const QVector<QPointF>& ctrl_points_,
	int i,
	double t,
	int k)
{
	if (k == 0)
	{
		return ctrl_points_[i];
	}
	else
	{
		return  (1 - t) * _de_casteljao_impl(ctrl_points_, i, t, k - 1)
			+ t * _de_casteljao_impl(ctrl_points_, i + 1, t, k - 1);
	}
}

void BezierCurve::de_casteljao()
{
	int ctrl_size = ctrl_points_.size();
	samples_.resize(ctrl_size * sample_size_);
	
	int sample_size = samples_.size();
	double interval = 1.0f / (sample_size - 1);

	if (ctrl_size == 0) return;
	else if (ctrl_size == 1)
	{
		for (auto& i : samples_)
		{
			i = ctrl_points_[0];
		}
		return;
	}
	else if (ctrl_size == 2)
	{
		for (int t = 0; t < sample_size; ++t)
		{
			samples_[t] = (1 - t * interval) * ctrl_points_[0]
				+ t * interval * ctrl_points_[1];
		}
		return;
	}


	for (int t = 0; t < sample_size; ++t)
	{
		samples_[t] = _de_casteljao_impl(ctrl_points_,
			0,
			t * interval,
			ctrl_size - 1);
	}
}

double BernsteinBaseFunc(int i, int n, double t)
{
	assert(t >= 0.0 && t <= 1.0);
	double multiplier = 1.0;
	
	for (int k = 0; k < i; ++k)
	{
		multiplier = multiplier * t * (n - k) / (i - k);
	}
	return multiplier * pow(1-t, n-i);
}
