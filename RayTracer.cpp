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

int main()
{
	int nx = 200;
	int ny = 100;
	int ns = 100;

	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);
	hitable *list[4];
	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.8,0.3,0.3)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2)));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new metal(vec3(0.8, 0.8, 0.8)));
	
	hitable *world = new hitable_list(list, 4);
	
	//set the camera
	camera cam;

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