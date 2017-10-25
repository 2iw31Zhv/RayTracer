#include <cmath>

#include "RotationBezierSurface.h"
#include "tools.h"
#include "easylogging++\easylogging++.h"

RotationBezierSurface::RotationBezierSurface(const BezierCurve& bezier_curve,
	const Vertice& center,
	float size):
	center_(center),
	size_(size),
	u_(0.0),
	v_(0.0)
{
	for (const auto& c : bezier_curve.ctrl_points_)
	{
		bezier_curve_.push_ctrl_points(c);
	}
	// set bounding box
}

// 3 elements 3 equations
// center_ + size * (x(u)cosv, y(u), x(u)sinv) = e + t * d
bool RotationBezierSurface::hit(const Ray & ray, float t0, float t1, float & t)
{
	//bounding box check
	double u(0.0), v(0.0), tempt(t0);
	//initial guess (u, v, t)
	double residual = DBL_MAX;
	double error = DBL_MAX;
	
	double ** jacobian = new double*[3];
	double ** r = new double*[3];
	for (int i = 0; i < 3; ++i)
	{
		jacobian[i] = new double[3];
		r[i] = new double[3];
	}
	
	double * f = new double[3]();
	double * update = new double[3]();

	int iterate_num = 0;
	while (1)
	{	// if not converge return false
		if (iterate_num > 1000)
		{
			return false;
		}
		//compute f(u, v, t)
		double xu = pu_(u).x();
		double yu = pu_(u).y();
		double dxu = dpu_(u).x();
		double dyu = dpu_(u).y();
		LOG(DEBUG) << "du " << dxu << " " << dyu;
		f[0] = size_ * xu * cos(v)
			- ray.at(tempt).x() + center_.x();
		f[1] = size_ * yu
			- ray.at(tempt).y() + center_.y();
		f[2] = size_ * xu * sin(v)
			- ray.at(tempt).z() + center_.z();

		double residual = nrmi(3, f);
		LOG(DEBUG) << f[0] << " " << f[1] << " " << f[2];

		if (residual < 1e-4)
		{
			break;
		}

		//compute Jacobian 3 x 3
		jacobian[0][0] = size_ * cos(v) * dxu;
		jacobian[0][1] = -size_ * xu * sin(v);
		jacobian[0][2] = -ray.d().x();

		jacobian[1][0] = size_ * dyu;
		jacobian[1][1] = 0.0;
		jacobian[1][2] = -ray.d().y();

		jacobian[2][0] = size_ * sin(v) * dxu;
		jacobian[2][1] = size_ * xu * cos(v);
		jacobian[2][2] = -ray.d().z();

		LOG(DEBUG) << jacobian[0][0] << " " << jacobian[0][1] << " " << jacobian[0][2];
		LOG(DEBUG) << jacobian[1][0] << " " << jacobian[1][1] << " " << jacobian[1][2];
		LOG(DEBUG) << jacobian[2][0] << " " << jacobian[2][1] << " " << jacobian[2][2];

		double detj = determinant3(jacobian[0][0], jacobian[0][1], jacobian[0][2],
			jacobian[1][0], jacobian[1][1], jacobian[1][2],
			jacobian[2][0], jacobian[2][1], jacobian[2][2]);

		if (fabs(detj) < 1e-4)
		{
			return false;
		}
		LOG(DEBUG) << detj;
		//compute inverse jacobian
		auto PROCESS = [&](int row, int col,
			int row1, int row2,
			int col1, int col2, double sign)
		{
			r[row][col] = sign
				* determinant2(jacobian[row1][col1], jacobian[row1][col2],
					jacobian[row2][col1], jacobian[row2][col2]) / detj;
		};

		PROCESS(0, 0, 1, 2, 1, 2, 1.0);
		PROCESS(0, 1, 1, 2, 0, 2, -1.0);
		PROCESS(0, 2, 1, 2, 0, 1, 1.0);
		PROCESS(1, 0, 0, 2, 1, 2, -1.0);
		PROCESS(1, 1, 0, 2, 0, 2, 1.0);
		PROCESS(1, 2, 0, 2, 0, 1, -1.0);
		PROCESS(2, 0, 0, 1, 1, 2, 1.0);
		PROCESS(2, 1, 0, 1, 0, 2, -1.0);
		PROCESS(2, 2, 0, 1, 0, 1, 1.0);
		
		LOG(DEBUG) << r[0][0];

		update[0] = r[0][0] * f[0] + r[0][1] * f[1] + r[0][2] * f[2];
		update[1] = r[1][0] * f[0] + r[1][1] * f[1] + r[1][2] * f[2];
		update[2] = r[2][0] * f[0] + r[2][1] * f[1] + r[2][2] * f[2];

		LOG(DEBUG) << update[0] <<" "<< update[1] << " " << update[2];
		u -= update[0];
		v -= update[1];
		t -= update[2];

		iterate_num++;

		error = nrmi(3, update);
		if (error < 1e-4)
		{
			break;
		}
	}

	delete[] f;
	delete[] update;
	for (int i = 0; i < 3; ++i)
	{
		delete[] r[i];
		delete[] jacobian[i];
	}
	delete[] jacobian;
	delete[] r;

	if (u < 0 || u > 1)
	{
		return false;
	}

	u_ = u;
	v_ = v;
	LOG(DEBUG) << u << " " << v;

	if (tempt < t0 || tempt >= t1)
	{
		return false;
	}

	t = tempt;
	LOG(DEBUG) << t;
	return true;
}

//(dyu cos(v) , -dxu, dyu sin(v))
Point3F RotationBezierSurface::hit_normal(const Ray & ray, float t) const
{
	Point3F result(-dpu_(u_).y() * cos(v_),
		dpu_(u_).x(), 
		-dpu_(u_).y() * sin(v_));
	return result.normalize();
}

QPointF RotationBezierSurface::pu_(double u) const
{
	int n = bezier_curve_.ctrl_points_size() - 1;
	QPointF p(0.0, 0.0);
	for (int i = 0; i <= n; ++i)
	{
		p += bezier_curve_.ctrl_points_[i] * BernsteinBaseFunc(i, n, u);
	}
	return p;
}

QPointF RotationBezierSurface::dpu_(double u) const
{
	int n = bezier_curve_.ctrl_points_size() - 1;
	QPointF dp(0.0, 0.0);
	for (int i = 0; i < n; ++i)
	{
		dp += bezier_curve_.ctrl_points_[i]
			* (BernsteinBaseFunc(i - 1, n - 1, u) - BernsteinBaseFunc(i, n - 1, u));
	}
	return n * dp;
}
