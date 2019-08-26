// raw2tga -- converts .raw format files to 24 bit uncompressed Targa files.

#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
	int     xres, yres;             /* image size */
	std::ifstream infile;
	std::ofstream outfile;

	int     i, y, total;
	int     iCurRow = 0;
	int     iSkip = 1;
	char red[1024], green[1024], blue[1024];

	if (argc != 2)
	{
		std::cout << "Usage: raw2tga <file.raw>" << std::endl;
		exit(0);
	}

	std::string infilename(argv[1]);
	infilename.append(".raw");
	infile.open(infilename, std::ios::binary);
	if (!infile.is_open())
	{
		std::cerr << "Error opening file " << infilename << "for input." << std::endl;
		exit(1);
	}

	std::string outfilename(argv[1]);
	outfilename.append(".tga");
	outfile.open(outfilename, std::ios::binary);
	if (!outfile.is_open())
	{
		std::cerr << "Error opening file " << outfilename << "for output." << std::endl;
		exit(1);
	}

	/* Read .img header.  Get resolution and toss the rest. */
	xres = 512;
	yres = 484;

	// Write .tga header
	outfile.put(0);
	outfile.put(0);
	outfile.put(2);				// Type 2 Targa file
	for (i = 3; i < 12; i++)
		outfile.put(0);
	outfile.put(xres & 255);
	outfile.put(xres / 256);
	outfile.put(yres & 255);
	outfile.put(yres / 256);
	outfile.put(24);			// bits per pixel
	outfile.put(32);			// image descriptor

	std::cout << "Image size : " << xres << ", " << yres << std::endl;

	// Read the current row number
	infile.read((char*)& iCurRow, sizeof(short));
	std::cout << "Starting with row :" << iCurRow << std::endl;

	// Write blank lines until we have some data
	for (y = 0; y < iCurRow; y++)
	{
		if (y % 10 == 0)
			std::cout << std::endl << y;

		total = xres;
		while (total > 0)
		{
			outfile.put(0);	// Red
			outfile.put(0); // Green
			outfile.put(0); // Blue
			total--;
		}
	}

	// Now go through our scanlines
	for (; y < yres; y++)
	{
		// let user know we're awake
		if (y % 10 == 0)
		{
			std::cout << std::endl << y;
		}

		if (iSkip || infile.read((char*)& iCurRow, sizeof(short)))
		{
			iSkip = 0;
			infile.read(red, xres);
			infile.read(green, xres);
			infile.read(blue, xres);

			total = 0;
			while (total < xres)
			{
				outfile.put(blue[total]);
				outfile.put(green[total]);
				outfile.put(red[total]);
				total++;
			}
		}
		else
		{
			total = xres;
			while (total > 0)
			{
				outfile.put(0);		// Red
				outfile.put(0);		// Green
				outfile.put(0);		// Blue
				total--;
			}
		}
	}

	// Close everything out and exit
	infile.close();
	outfile.close();

	exit(0);

}
