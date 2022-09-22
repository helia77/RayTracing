#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>
#include<cmath>

using namespace std;

struct sphere {
	float radius;
	vect points;
	float color[3];

};

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
		vect result;
		result.x = x - tar.x;
		result.y = y - tar.y;
		result.z = z - tar.z;
		return result;
	}
	vect operator+(vect tar) {
		vect result;
		result.x = x + tar.x;
		result.y = y + tar.y;
		result.z = z + tar.z;
		return result;
	}
	// dot (inner) product of two vectors
	float dot(vect tar) {
		float result = 0.0f;
		result = result + x * tar.x;
		result = result + y * tar.y;
		result = result + z * tar.z;
		return result;
	}
	// scalar 
	vect scalar(vect tar) {
		vect result;
		result.x = x * tar.x;
		result.y = y * tar.y;
		result.z = z * tar.z;
		return result;
	}
	vect operator*(float t) {
		vect result;
		result.x = x * t;
		result.y = y * t;
		result.z = z * t;
		return result;
	}
	vect normalize() {
		vect result;
		result.x = x / len;
		result.y = y / len;
		result.z = z / len;
		return result;
	}
};
struct ray {
	vect p;
	vect v;
	vect position(float t) {
		vect result;
		result.x = p.x + v.x * t;
		result.y = p.y + v.y * t;
		result.z = p.z + v.z * t;
		return p + (v*t);
	}
};
struct intersection {
	vect pts;
	std::vector<float> color;
	
	intersection(vect pos, std::vector<float> col) {
		pts = pos;
		color = col;
	}
};

vector<sphere> ReadSphere(string s) {
	ifstream rfile(s);
	vector<sphere> spheres;

	while (rfile) {
		sphere s;
		rfile >> s.points.x >> s.points.y >> s.points.z;
		rfile >> s.radius;
		rfile >> s.color[0] >> s.color[1] >> s.color[2];
		spheres.push_back(s);
	}
	return spheres;
}

void make_Sphere(sphere center, double r, std::vector<sphere>& spherePoints);

void write_image(std::string filename, char* bytes, short N) {
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

int main(int argc, char** argv[])
{
	float z = 2;
	short N = 512;
	/*char* image = (char*)malloc(N * N * 3);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			image[j * 300*3 + i * 3 + 0] = '0';
			image[j * 300*3 + i * 3 + 1] = '0';
			image[j * 300*3 + i * 3 + 2] = 'r';
		}
	}
	*/
	//write_image("spheres.tga", image, N);
	vector<sphere> spheres;
	spheres = ReadSphere("spheres.txt");


	char* image = (char*)malloc(300 * 300 * 3);
	for (const sphere& e : spheres) {
		int x = e.points.x;
		int y = e.points.y;
		int z = e.points.z;

		int a = e.color[0];
		int b = e.color[1];
		int c = e.color[2];
		image[x * 300 * 3 + y * 3 + 0] = 'a.0';
		image[x * 300 * 3 + y * 3 + 1] = 'b.0';
		image[x * 300 * 3 + y * 3 + 2] = 'c.r';
	}
	write_image("spheres.tga", image, N);
	return 0;
}


//void make_Sphere(sphere center, double r, std::vector<sphere>& spherePoints)
//{
//	const double PI = 3.1415926;
//	spherePoints.clear();
//
//	// Iterate through phi, theta then convert r,theta,phi to  XYZ
//	for (double phi = 0.; phi < 2 * PI; phi += PI / 10.) // Azimuth [0, 2PI]
//	{
//		for (double theta = 0.; theta < PI; theta += PI / 10.) // Elevation [0, PI]
//		{
//			sphere point;
//			point.x = r * cos(phi) * sin(theta) + center.x;
//			point.y = r * sin(phi) * sin(theta) + center.y;
//			point.z = r * cos(theta) + center.z;
//			spherePoints.push_back(point);
//		}
//	}
//	return;
//}

