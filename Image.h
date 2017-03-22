//*****************************************************************************
//
// Image.h : Defines the class operations on images
//
// Author - Parag Havaldar
// Main Image class structure 
//
//*****************************************************************************

#ifndef IMAGE_DISPLAY
#define IMAGE_DISPLAY
#define M_PI 3.14159265


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "afxwin.h"

// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// Class structure of Image 
// Use to encapsulate an RGB image
class MyImage
{

private:
	int		Width;					// Width of Image
	int		Height;					// Height of Image
	char	ImagePath[_MAX_PATH];	// Image location
	char*	Data;					// RGB data of the image
	float** convert_data;				// 2D Matrix of YPbPr data of the image
	float** dct_coefficient;				// 8*8 block of DCT Coeffecients
	float** idct_coefficient;				// 8*8 block of iDCT Coeffecient
	float** quantizedMatrix;		// 8*8 block that holds quantized values based on input parameter n
	float**	final_matrix;				// Quantized Data of the image
	char*	out_data;				// Output 1D YPbPr Data values
	char*	rgb_output_vals;		// Output 1D RGB Data values
	int		QuantValue;
	int		Diagonals;

public:
	// Constructor
	MyImage();
	// Copy Constructor
	MyImage::MyImage(MyImage *otherImage);

	void generateZigZag(int dim);

	void convert_to_1D(float ** convData);
	void convert_YPbPr_to_RGB();
	void convert_RGB_to_YPbPr();
	float** convert_matrix_2d(float* matrix, int height, int width);

	void Direct_Cosine_Transform(float ** resBlock);
	void computeFunctions(float** resBlock, float** quantizedMatrix);
	void Inverse_DCT(float ** DCTMatrix);

	float** fetch_diagonals(float ** dctCoeff, int diagonal);
	void moreValue(char* rbgOutputValues);

	// Destructor
	~MyImage();

	void compute_DCT();

	void fill_matrix_values(int offsetX, int offsetY, int xVal, int yVal);

	void matrix_construction(float ** convData, float ** result, int offsetX, int offsetY, int xVal, int yVal);

	// operator overload
	MyImage & operator= (const MyImage & otherImage);

	// Reader & Writer functions
	void	setWidth(const int w) { Width = w; };
	void	setHeight(const int h) { Height = h; };
	void	setQuantValue(const int q) { QuantValue = q; };
	void	setDiagonals(const int d) { Diagonals = d; };
	void	setImageData(const char *img) { Data = (char *)img; };
	void	setImagePath(const char *path) { strcpy(ImagePath, path); }
	int		getWidth() { return Width; };
	int		getHeight() { return Height; };
	int		getQuantValue() { return QuantValue; };
	int		getDiagonals() { return Diagonals; }
	char*	getImageData() { return Data; };
	char*	getImagePath() {
		return ImagePath;
	}

	// Input Output operations
	bool	ReadImage();
	bool	WriteImage();

	// Modifications
	bool	Modify();

};

#endif //IMAGE_DISPLAY