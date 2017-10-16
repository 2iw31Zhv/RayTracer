#pragma once
#include <cmath>
#include <algorithm>

template <typename T>
class Point3
{
private:
	T x_;
	T y_;
	T z_;
public:
	Point3(T x = T(), T y = T(), T z = T()) :
		x_(x), y_(y), z_(z)
	{
	}
	const T& x() const { return x_; }
	const T& y() const { return y_; }
	const T& z() const { return z_; }

	T& x() { return x_; }
	T& y() { return y_; }
	T& z() { return z_; }

	Point3<T>& operator+= (const Point3<T>& rhs);
	Point3<T>& operator-= (const Point3<T>& rhs);
	Point3<T>& operator*= (const T& val);
	Point3<T>& operator/= (const T& val);

	friend Point3<T> operator+(const Point3<T>& lhs, const Point3<T>& rhs)
	{
		return Point3<T>(lhs.x_ + rhs.x_, lhs.y_ + rhs.y_, lhs.z_ + rhs.z_);
	}

	friend Point3<T> operator-(const Point3<T>& lhs, const Point3<T>& rhs)
	{
		return Point3<T>(lhs.x_ - rhs.x_, lhs.y_ - rhs.y_, lhs.z_ - rhs.z_);
	}

	friend Point3<T> operator*(const Point3<T>& lhs, const T& val)
	{
		return Point3<T>(lhs.x_ * val, lhs.y_ * val, lhs.z_ * val);
	}

	friend Point3<T> operator*(const T& val, const Point3<T>& rhs)
	{
		return Point3<T>(val * rhs.x_, val * rhs.y_, val * rhs.z_);
	}

	friend Point3<T> operator/(const Point3<T>& lhs, const T& val)
	{
		return Point3<T>(lhs.x_ / val, lhs.y_ / val, lhs.z_ / val);
	}

	friend bool operator==(const Point3<T>& lhs, const Point3<T>& rhs)
	{
		return (lhs.x_ == rhs.x_) && (lhs.y_ == rhs.y_) && (lhs.z_ == rhs.z_);
	}

	friend bool operator!=(const Point3<T>& lhs, const Point3<T>& rhs)
	{
		return !(lhs == rhs);
	}

	T dot(const Point3<T>& rhs) const;
	Point3<T> cross(const Point3<T>& rhs) const;

	Point3<T>& reverse();
	Point3<T>& normalize();

	T norm2() const { return std::sqrt(this->dot(*this)); }
	T normi() const { using std::max; return max(max(fabs(x_), fabs(y_)), fabs(z_)); }
};

template <typename T>
Point3<T>& Point3<T>::operator+= (const Point3<T>& rhs)
{
	x_ += rhs.x_;
	y_ += rhs.y_;
	z_ += rhs.z_;
	return *this;
}

template <typename T>
Point3<T>& Point3<T>::operator-= (const Point3<T>& rhs)
{
	x_ -= rhs.x_;
	y_ -= rhs.y_;
	z_ -= rhs.z_;
	return *this;
}

template <typename T>
Point3<T>& Point3<T>::operator*= (const T& val)
{
	x_ *= val;
	y_ *= val;
	z_ *= val;
	return *this;
}

template <typename T>
Point3<T>& Point3<T>::operator/= (const T& val)
{
	x_ /= val;
	y_ /= val;
	z_ /= val;
	return *this;
}



template <typename T>
T Point3<T>::dot(const Point3<T>& rhs) const
{
	return x_ * rhs.x_ + y_ * rhs.y_ + z_ * rhs.z_;
}


template <typename T>
Point3<T> Point3<T>::cross(const Point3<T>& rhs) const
{
	return Point3<T>(y_ * rhs.z_ - z_ * rhs.y_,
		z_ * rhs.x_ - x_ * rhs.z_,
		x_ * rhs.y_ - y_ * rhs.x_);
}

template <typename T>
Point3<T>& Point3<T>::reverse()
{
	x_ = -x_;
	y_ = -y_;
	z_ = -z_;
	return *this;
}

template <typename T>
Point3<T>& Point3<T>::normalize()
{
	T length = norm2();
	x_ /= length;
	y_ /= length;
	z_ /= length;
	return *this;
}

template class Point3<float>;
template class Point3<double>;

typedef Point3<float> Point3F;
typedef Point3<double> Point3D;

