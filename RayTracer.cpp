#include <iostream>
#include <opencv2\opencv.hpp>
#include "ray.h"
#include "hitablelist.h"
#include "sphere.h"
#include "camera.h"
#include <random>

double rand1()
{
	return (double)rand() / ((double)RAND_MAX + 1);
}

vec3 color(const ray& r, hitable *world, int depth)
{
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation*color(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0, 0, 0);
		}
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		double t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);

	}
}

hitable *random_scene()
{
	int n = 500;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			double choose_mat = randd();
			vec3 center(a + 0.9*randd(), 0.2, b + 0.9*randd());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9)
			{
				if (choose_mat < 0.8)
				{//diffuse
					list[i++] = new sphere(center, 0.2, new lambertian(vec3(randd()*randd(), randd()*randd(), randd()*randd())));
				}
				else if (choose_mat < 0.95)
				{//metal
					list[i++] = new sphere(center, 0.2,
						new metal(vec3(0.5*(1 + randd()), 0.5*(1 + randd()), 0.5*(1 + randd())), 0.5*randd()));
				}
				else
				{//glass
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
	return new hitable_list(list, i);
}

int main()
{
	int nx = 2000;
	int ny = 1000;
	int ns = 100;

	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);

	//set the camera
	vec3 lookfrom(3, 3, 2);
	vec3 lookat(0, 0, -1);
	double dist_to_focus = (lookfrom - lookat).length();
	double aperture = 2.0;
	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, double(nx) / double(ny), aperture, dist_to_focus);

	hitable *world = random_scene();

	// set the image matrix
	cv::Mat testImage(nx,ny,CV_8UC3);
	for (int j = ny - 1; j >=0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				double u = double(i + rand1()) / double(nx);
				double v = double(j + rand1()) / double(ny);
				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0);
				col += color(r, world, 0);
			}
			col /= double(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			cv::Vec3b& rgb = testImage.at<cv::Vec3b>(i, j);
			rgb[0] = ir;
			rgb[1] = ig;
			rgb[2] = ib;
			//std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
	cv::imwrite("test.png", testImage);

	return 1;
}