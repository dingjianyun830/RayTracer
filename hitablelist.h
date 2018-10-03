#ifndef HITABLELISTH
#define HITABLELISTH

#include "hitable.h"

class hitable_list: public hitable
{
public:
	hitable ** list;
	int list_size;
	hitable_list() {}
	hitable_list(hitable **l, int n)
	{
		list = l;
		list_size = n;
	}
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
};

bool hitable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
	hit_record tem_rec;
	bool hit_anything = false;
	double closest_so_far = t_max;
	for (int i = 0; i < list_size; i++)
	{
		if (list[i]->hit(r, t_min, closest_so_far, tem_rec))
		{
			hit_anything = true;
			closest_so_far = tem_rec.t;
			rec = tem_rec;
		}
	}

	return hit_anything;
}
#endif //!HITABLELISTH
