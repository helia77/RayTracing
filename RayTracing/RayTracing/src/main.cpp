#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>
#include<cmath>
#include<thread>
#include<ctime>

using namespace std;


struct vect {
	float x, y, z;
	float len;
	vect(float X, float Y, float Z)
		: x(X), y(Y), z(Z)
	{
		len = sqrt(X * X + Y * Y + Z * Z);
	}
	vect()
		: x(0), y(0), z(0), len(0)
	{}
	vect operator-(vect tar) {
		return vect(x - tar.x, y - tar.y, z - tar.z);
	}
	vect operator+(vect tar) {
		return vect(x + tar.x, y + tar.y, z + tar.z);
	}
	// dot (inner) product of two vectors
	float dot(vect& tar) {
		return (x * tar.x) + (y * tar.y) + (z * tar.z);
	}
	
	vect operator*(vect tar) {
		return vect(x * tar.x, y * tar.y, z * tar.z);
	}
	vect operator*(float t) {
		return vect(x * t, y * t, z * t);
	}
	vect operator/(float t) {
		return vect(x / t, y / t, z / t);
	}
	vect normalize() {
		return vect(x / len, y / len, z / len);
	}
	friend ostream& operator<<(ostream& out, const vect& v) {
		out << v.x << ' ' << v.y << ' ' << v.z;
		return out;
	}
};
struct color {
	float red, gre, blu;
	color(float r, float g, float b)
		: red(r), gre(g), blu(b)
	{}
	color()
		: red(0), gre(0), blu(0)
	{}
	color operator*(float i) {
		red *= i;
		gre *= i;
		blu *= i;
		return *this;
	}
	color operator*(color clr) {
		red *= clr.red;
		gre *= clr.gre;
		blu *= clr.blu;
		return *this;
	}
	color operator+(color clr) {
		color result;
		result.red = red * clr.red;
		result.blu = blu * clr.blu;
		result.gre = gre * clr.gre;
		return result;
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
	vect pov; //point of view
	vect v;
	rays()
		: pov(0,0,0), v(0,0,0)
	{}
	rays(vect p, vect vec)
		: pov(p), v(vec)
	{}
};

//If the input ray intersects the input sphere, ouput is a color array of r, g, b
color intersection(rays ray, vector<sphere> ss, vector<light> light1) {
	color pixel(1.0f, 1.0f, 1.0f);
	color light_clr;
	float delta;
	float a, b, c;
	float t = 0.0f;
	float min = (float)INT_MAX;							//to see the smallest t value
	float ka = 0.4f;					//light intensity
	for (int i = 0; i < ss.size(); i++) {
		sphere sph = ss.at(i);
		vect distance = ray.pov - sph.center;
		a = ray.v.dot(ray.v);
		b = ray.v.dot(distance) * 2;
		c = distance.dot(distance) - (sph.radius * sph.radius);
		delta = (b * b) - (4 * a * c);
		t = (-b - sqrt(delta)) / (2 * a);
		if (delta >= 0 && t < min && t > 0) {
			//pixel = sph.clr;
			min = t;
			float intens = 0.0f;			//light intensity
			rays light_r;
			//calculate		p + vt		or the pixel on the sphere
			light_r.pov = ray.v * t;

			//the norm of the sphere center to the pixel
			vect N = sph.Norm(light_r.pov);
			//color final;
			//calculate unit vector from pov to light center
			for (int j = 0; j < light1.size(); j++) {
				light lit = light1.at(j);
				light_r.v = (lit.center - sph.center).normalize();

				//calculating illumination
				intens += (light_r.v.dot(N) > 0) ? light_r.v.dot(N) : 0;
			}
			intens /= light1.size();
			pixel = sph.clr * intens;
		}
	}
	return pixel;


};

vector<sphere> ReadSphere(string s) {
	ifstream rfile(s);
	vector<sphere> spheres;
	float x, y, z;
	while (rfile) {
		sphere sph;
		rfile >> x >> y >> z;
		sph.center.x = x;
		sph.center.y = y;
		sph.center.z = z;
		rfile >> sph.radius;
		rfile >> sph.clr.blu >> sph.clr.gre >> sph.clr.red;
		spheres.push_back(sph);
	}
	return spheres;
}
vector<light> ReadLight(string s) {
	ifstream rfile(s);
	vector<light> Lights;
	float x, y, z;
	while (rfile) {
		light lit;
		rfile >> x >> y >> z;
		lit.center.x = x;
		lit.center.y = y;
		lit.center.z = z;
		rfile >> lit.clr.red >> lit.clr.gre >> lit.clr.blu;
		Lights.push_back(lit);
	}
	return Lights;
	//lights = Lights;
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

void renderImage(char* image, vector<sphere> spheres, vector<light> lights, short N, int start, int end) {
	float z = 1.5;
	float dx = 2.0f / (N - 1);			//change scale
	float dy = 2.0f / (N - 1);
	float sx = -1.0f;		//starting point
	float sy = -1.0f;
	for (short i = start; i < end; i++) {
		for (short j = 0; j < N; j++) {
			float rx = sx + j * dx;
			float ry = i;
			float rz = z;
			vect direction = vect(rx, ry, rz).normalize();
			vect view(0.0f, 0.0f, 0.0f);
			rays ray;
			ray.v = direction;
			ray.pov = view;
			color pixel_clr = intersection(ray, spheres, lights);
			image[i * N * 3 + j * 3 + 0] = pixel_clr.blu * 255;
			image[i * N * 3 + j * 3 + 1] = pixel_clr.gre * 255;
			image[i * N * 3 + j * 3 + 2] = pixel_clr.red * 255;
		}
	}

}


int main(int argc, char** argv[])
{
	short N = 1024;
	char* image = (char*)malloc(N * N * 3);
	vector<sphere> spheres;
	vector<light> lights;
	spheres = ReadSphere("spheres.txt");
	lights = ReadLight("lights.txt");
	
	int numThreads = 200; // number of threads to launch
	vector<std::thread> threads;

	time_t mT0 = time(NULL);

	for (int i = 0; i < numThreads; i++) {
		int start = (double)i / (double)numThreads * N;
		int stop = (double)(i + 1) / (double)numThreads * N;
		threads.push_back(std::thread(renderImage, image, spheres, lights, N, start, stop));
	}

	for (auto& thread : threads) {
		thread.join();
	}

	time_t mT1 = time(NULL);
	cout << "Execution time: " << (mT1 - mT0)* 1.000 << endl;

	write_image("spheres.tga", image, N);
	return 0;
}