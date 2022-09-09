#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>
#include<cmath>

struct sphere;
struct intersection;

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

int main(int argc, char** argv)
{
	
}