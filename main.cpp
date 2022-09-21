#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>
#include<cmath>

using namespace std;

struct sphere {
	// c, r, color
	float radius;
	float points[3];
	float color[3];

};

struct intersection {
	std::vector<float> point;
	std::vector<float> color;

	intersection(std::vector<float> pos, std::vector<float> col) {
		point = pos;
		color = col;
	}
};
vector<sphere> ReadSphere(string s);

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
	char* image = (char*)malloc(100 * 100 * 3);
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			image[j * 100*3 + i * 3 + 0] = '0';
			image[j * 100*3 + i * 3 + 1] = 'g';
			image[j * 100*3 + i * 3 + 2] = 'r';
		}
	}
	write_image("spheres.tga", image, 100);

	vector<sphere> spheres;
	spheres = ReadSphere("spheres.txt");


	return 0;
}

vector<sphere> ReadSphere(string s) {
	ifstream rfile(s);
	vector<sphere> spheres;

	while (rfile) {
		sphere s;
		rfile >> s.points[0] >> s.points[1] >> s.points[2];
		rfile >> s.radius;
		rfile >> s.color[0] >> s.color[1] >> s.color[2];
		spheres.push_back(s);
	}
	return spheres;
}