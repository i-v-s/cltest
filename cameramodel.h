#pragma once

#include <Eigen/Core>

using namespace Eigen;

template<typename real>
class CameraModel
{
private:
	real baseline, lfx, lcx, lfy, lcy, rfx, rcx, rfy, rcy;
public:
	inline real leftCenterY() { return lcy; }
	inline real rightCenterY() { return rcy; }
	typedef Matrix<real, 3, 1> Vector3;
	typedef Matrix<real, 2, 1> Vector2;
	void pointByScreen(Vector2 &result, real lx, real rx) const
	{
		lx -= lcx; rx -= rcx;
		real x = baseline * lx * rfx / (lx * rfx - rx * lfx);
		real z = x * lfx / lx;
		result << x, z;
	}
	void pointByScreen(Vector3 &result, real lx, real rx, real ly) const
	{
		lx -= lcx; rx -= rcx; ly -= lcy;
		real x = baseline * lx * rfx / (lx * rfx - rx * lfx);
		real z = x * lfx / lx;
		real y = ly / lfy * z;
		result << x, y, z;
	}
	inline Vector2 pointByScreen(real lx, real rx) const { Vector2 r; pointByScreen(r, lx, rx); return r; }
	inline Vector3 pointByScreen(real lx, real rx, real ly) const { Vector3 r; pointByScreen(r, lx, rx, ly); return r; }
	void setParams(real baseline, real lfx, real lcx, real lfy, real lcy, real rfx, real rcx, real rfy, real rcy)
	{
		this->baseline = baseline;
		this->lfx = lfx;
		this->lcx = lcx;
		this->lfy = lfy;
		this->lcy = lcy;
		this->rfx = rfx;
		this->rcx = rcx;
		this->rfy = rfy;
		this->rcy = rcy;
	}
	void predict(const Quaternion<real> &r, const Vector3 &t, Vector3 &v)
	{
		auto irm = r.inverse().toRotationMatrix();
		v = irm * (v - t);
	}
};