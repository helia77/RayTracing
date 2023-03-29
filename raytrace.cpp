#include<iostream>
#include<fstream>
#include<cmath>
#include<vector>
#include<thread>
#include<ctime>

using namespace std;


struct color {
	float red, gre, blu;
	color(float b, float g, float r)
		: blu(b), gre(g), red(r) {}
	color()
		: blu(0), gre(0), red(0) {}
	color operator*(float i) {
		blu *= i;
		gre *= i;
		red *= i;
		return *this;
	}
};

// vectors
struct vect {

	float x, y, z;
	float length;

	//constructor
	vect(float x0, float y0, float z0)
		: x(x0), y(y0), z(z0), length(sqrt(x0 * x0 + y0 * y0 + z0 * z0)) {}

	//default constructor
	vect()
		: x(0), y(0), z(0), length(0) {}

	// dot (inner) product
	float dot(vect v) {
		return (x * v.x) + (y * v.y) + (z * v.z);
	}
	vect scalar(float t) {
		return vect(x * t, y * t, z * t);
	}
	vect normalize() {
		return vect(x / length, y / length, z / length);
	}
	vect operator+(vect v) {
		return vect(x + v.x, y + v.y, z + v.z);
	}
	vect operator-(vect v) {
		return vect(x - v.x, y - v.y, z - v.z);
	}
};

struct sphere {
	float radius{};
	vect center;
	color clr;
	vect Norm(vect point) {
		vect norm((point.x - center.x) / radius, (point.y - center.y) / radius, (point.z - center.z) / radius);
		return norm;
	}
};

struct light {
	vect center;
	color clr;
};

struct rays {
	vect point;
	vect v;

	rays(vect p, vect vec)
		: point(p), v(vec) {}
	rays()
		: point(0,0,0), v(0,0,0) {}
	
	vect unit(sphere sph) {
		return vect((point.x - sph.center.x) / sph.radius, (point.y - sph.center.y) / sph.radius, (point.z - sph.center.z) / sph.radius);
	}
};

// Checks if the input ray intersects the sphere, returns output color of the intersected sphere
// Calculate the light intensity from surrounding lights on the intersection pixel

color Intersection(rays ray, vector<sphere> ss, vector<light> Lights) {
	color pixel(1.0f, 1.0f, 1.0f);
	color light_clr;
	float delta;
	float a, b, c;
	float t = 0.0f;
	float min = (float)INT_MAX;							// to see the smallest t value

	for (int i = 0; i < ss.size(); i++) {
		sphere sph = ss.at(i);
		vect p_c = ray.point - sph.center;
		a = ray.v.dot(ray.v);
		b = ray.v.dot(p_c) * 2;
		c = p_c.dot(p_c) - (sph.radius * sph.radius);
		delta = (b * b) - (4 * a * c);
		t = (-b - sqrt(delta)) / (2 * a);

		// 2 or 1 intersection:
		// lowest t to print closest sphere
		// no object behind camera matters in this case
		if (delta >= 0 && t < min && t > 0) {
			min = t;
			float intens = 0.0f;		// light intensity
			rays light_r;

			//	p + vt	or the intersection point (pixel) on the sphere
			light_r.point = ray.v.scalar(t);

			// unit vector from sphere center to the intersection point
			vect N = light_r.unit(sph);

			// unit vector from intersection point to light center
			for (int j = 0; j < Lights.size(); j++) {
				light lit = Lights.at(j);
				light_r.v = (lit.center - sph.center).normalize();

				// light contribution
				intens += (light_r.v.dot(N) > 0) ? light_r.v.dot(N) : 0;
			}
			intens /= Lights.size();
			pixel = sph.clr * intens;
		}
	}
	return pixel;
};

vector<sphere> ReadSphere(string s) {
	ifstream file(s);
	vector<sphere> Spheres;
	while (file) {
		sphere sph;
		file >> sph.center.x >> sph.center.y >> sph.center.z;
		file >> sph.radius;
		file >> sph.clr.blu >> sph.clr.gre >> sph.clr.red;
		Spheres.push_back(sph);
	}
	return Spheres;
}

vector<light> ReadLight(string s) {
	ifstream file(s);
	vector<light> Lights;
	while (file) {
		light lit;
		file >> lit.center.x >> lit.center.y >> lit.center.z;
		file >> lit.clr.blu >> lit.clr.gre >> lit.clr.red;
		Lights.push_back(lit);
	}
	return Lights;
}

void write_image(string filename, char* bytes, short N) {
	std::ofstream outfile;
	outfile.open(filename, std::ios::binary | std::ios::out);	// open a binary file
	outfile.put(0);						// id length (field 1)
	outfile.put(0);						// color map type (field 2)
	outfile.put(2);						// image_type (field 3)
	outfile.put(0); outfile.put(0);		// color map field entry index (field 4)
	outfile.put(0); outfile.put(0);		// color map length (field 4)
	outfile.put(0);				// color map entry size (field 4)
	outfile.put(0); outfile.put(0);		// x origin (field 5)
	outfile.put(0); outfile.put(0);		// y origin (field 5)
	outfile.write((char*)&N, 2);		// image width (field 5)
	outfile.write((char*)&N, 2);		// image height (field 5)
	outfile.put(24);				// pixel depth (field 5)
	outfile.put(0);				// image descriptor (field 5)
	outfile.write(bytes, N * N * 3);		// write the image data
	outfile.close();				// close the file
}

void RayTracing(char* image, vector<sphere> spheres, vector<light> lights, int begin, int end, short N, float z) {

	float scale = N - 1;					// scale pixel size
	float start = -1.0f;						//starter point

	//start and stop point for each column
	for (short y = begin; y < end; y++) {
		for (short x = 0; x < N; x++) {

			float rx = 2 * (x / scale) + start;
			float ry = 2 * (y / scale) + start;
			float rz = z;
			vect direction = vect(rx, ry, rz).normalize();
			vect camera(0.0f, 0.0f, 0.0f);
			rays ray(camera, direction);
			color pixel_clr = Intersection(ray, spheres, lights);
			image[y * N * 3 + x * 3 + 0] = pixel_clr.red * 255;
			image[y * N * 3 + x * 3 + 1] = pixel_clr.gre * 255;
			image[y * N * 3 + x * 3 + 2] = pixel_clr.blu * 255;
		}
	}

}


int main(int argc, char** argv[])
{
	cout << "Number of processors: " << thread::hardware_concurrency() << endl;
	float z = 1;
	short N = 1024;
	char* Img = (char*)malloc(N * N * 3);
	vector<sphere> spheres;
	vector<light> lights;

	spheres = ReadSphere("spheres.txt");
	lights = ReadLight("lights.txt");
	
	// number of threads: dividing the image into sub-problems (number of processors)
	int numTh = N / thread::hardware_concurrency();
	vector<thread> threads;

	// timer starts
	clock_t start = clock();

	// number of columns that each thread computes
	float col_num = float(N) / float(numTh);

	int begin{}, end{};

	// running threads
	// each thread executes a number of columns
	for (int i = 0; i < numTh; i++) {
		begin = i * col_num;
		end = (i + 1) * col_num;
		threads.push_back(thread(RayTracing, Img, spheres, lights, begin, end, N, z));
	}
	for (auto& th : threads) {
		th.join();
	}

	// timer ends
	clock_t stop = clock();
	cout << "Execution time: " << (double)(stop - start)/ CLOCKS_PER_SEC << endl;

	write_image("spheres.tga", Img, N);
	return 0;
}