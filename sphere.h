#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere :public hitable
{
public:
	vec3 center;
	double radius;
	material* mat_pt;
	sphere() {}
	sphere(vec3 cen, double r) :center(cen), radius(r) {};
	sphere(vec3 cen, double r, lambertian *mat_ptr) :center(cen), radius(r), mat_pt(mat_ptr) {};
	sphere(vec3 cen, double r, metal *mat_ptr) :center(cen), radius(r) , mat_pt(mat_ptr) {};
	sphere(vec3 cen, double r, dielectric *mat_ptr) :center(cen), radius(r), mat_pt(mat_ptr) {};
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;

};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
	vec3 oc = r.origin() - center;
	double a = dot(r.direction(), r.direction());
	double b = dot(oc, r.direction());
	double c = dot(oc, oc) - radius*radius;
	double discriminant = b*b - a*c;
	if (discriminant > 0)
	{
		double temp = (-b - sqrt(b*b - a*c)) / a;
		if (temp<t_max&&temp>t_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_pt;
			return true;
		}
		
		temp = (-b + sqrt(b*b - a*c)) / a;
		if (temp<t_max&&temp>t_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_pt;
			return true;
		}
	}

	return false;
}
#endif // !SPHEREH

