#ifndef HITABLEH
#define HITABLEH

#include "ray.h"

class material;

struct hit_record
{
	double t;
	vec3 p;
	vec3 normal;
	material *mat_ptr;
};

class hitable
{
public:
	virtual bool hit(const ray&r, double t_min, double t_max, hit_record& rec) const = 0;
};

double randd()
{
	return (double)rand() / ((double)RAND_MAX + 1);
}

vec3 reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n)*n;
}

vec3 random_in_unit_sphere()
{
	vec3 p;
	do
	{
		p = 2.0*vec3(randd(), randd(), randd()) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;
}

class material
{
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

class metal :public material
{
public:
	vec3 albedo;
	metal(const vec3& a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
	{
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected);
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
};

class lambertian : public material
{
public:
	vec3 albedo;
	lambertian(const vec3& a) :albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
	{
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}
};

#endif //!HITABLEH
#pragma once
