#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <iostream>
#include <fstream>

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

struct PixelCoord {
	int x, y;
};

std::vector<char> blackPixels;
UINT sampledWidth = 0;
UINT sampledHeight = 0;

void ProcessImage(const wchar_t* filename, UINT xStep, UINT yStep)
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	Bitmap* bitmap = new Bitmap(filename);
	if (bitmap->GetLastStatus() == Ok)
	{
		UINT width = bitmap->GetWidth();
		UINT height = bitmap->GetHeight();

		sampledWidth = (width + xStep - 1) / xStep;
		sampledHeight = (height + yStep - 1) / yStep;

		blackPixels.clear();
		blackPixels.reserve(sampledWidth * sampledHeight);

		for (UINT y = 0; y < height; y += yStep)
		{
			for (UINT x = 0; x < width; x += xStep)
			{
				Color color;
				bitmap->GetPixel(x, y, &color);
				if (color.GetRed() < 10 &&
					color.GetGreen() < 10 &&
					color.GetBlue() < 10)
				{
					blackPixels.emplace_back('T');  // Black = Tile
				}
				else
				{
					blackPixels.emplace_back('D');  // Anything else = Dirt
				}
			}
		}
	}
	else
	{
		std::cerr << "Failed to load image." << std::endl;
	}

	delete bitmap;
	GdiplusShutdown(gdiplusToken);
}

void PrintGrid(std::ostream& out = std::cout)
{

	if (blackPixels.size() != sampledWidth * sampledHeight)
	{
		out << "Error: pixel vector size does not match grid size!\n";
		return;
	}

	for (UINT row = 0; row < sampledHeight; ++row)
	{
		out << "    { ";
		for (UINT col = 0; col < sampledWidth; ++col)
		{
			char c = blackPixels[row * sampledWidth + col];
			out << c;

			if (col != sampledWidth - 1)
				out << ", ";
		}
		out << " }";

		if (row != sampledHeight - 1)
			out << ",";
		out << std::endl;
	}
}

int main()
{
	const wchar_t* filename = L"Level1.png";

	UINT xStep = 20;
	UINT yStep = 4;

	ProcessImage(filename, xStep, yStep);

	std::cout << sampledWidth << " x " << sampledHeight << std::endl;
	PrintGrid();


	std::ofstream fileOut("Level1_Grid.txt");
	if (fileOut.is_open())
	{
		PrintGrid(fileOut);
		fileOut.close();
		std::cout << "Grid saved to Level1_Grid.txt" << std::endl;
	}
	else
	{
		std::cerr << "Failed to write output file!" << std::endl;
	}

	return 0;
}
