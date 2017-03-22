//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"


// Constructor and Desctructors
MyImage::MyImage()
{
	Data = NULL;
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
	QuantValue = 0;
	out_data = NULL;
}

MyImage::~MyImage()
{
	if (Data)
		delete Data;
}


// Copy constructor
MyImage::MyImage(MyImage *otherImage)
{
	Height = otherImage->Height;
	Width = otherImage->Width;
	QuantValue = otherImage->QuantValue;
	Data = new char[Width*Height * 3];
	strcpy(otherImage->ImagePath, ImagePath);

	for (int i = 0; i<(Height*Width * 3); i++)
	{
		Data[i] = otherImage->Data[i];
	}

}


void MyImage::fill_matrix_values(int offsetX, int offsetY, int xVal, int yVal) {
	int x = xVal;
	int y = yVal;
	int o_x = offsetX;
	int o_y = offsetY;
	int fill_val = 1;

	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (fill_val == 1) {
				final_matrix[o_x + i][o_y + (j * 3)] = idct_coefficient[i][j];
			}
			else
				fill_val = 0;
		}
	}
}

void MyImage::matrix_construction(float** converted_data, float** result, int offsetX, int offsetY, int xVal, int yVal) {

	int x = xVal;
	int y = yVal;
	int o_x = offsetX;
	int o_y = offsetY;
	int block_val = 1;

	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (block_val == 1) {
				result[i][j] = converted_data[o_x + i][o_y + (j * 3)];
			}
			else
				block_val = 0;
		}
	}
}

void MyImage::convert_to_1D(float** data_matrix) {
	int k, conv_1d = 1;
	out_data = new char[Width*Height * 3];
	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width * 3; j++) {
			if (conv_1d == 1) {
				k = i * (Width * 3) + j;
				out_data[k] = data_matrix[i][j];
			}
		}
	}
}

void MyImage::convert_YPbPr_to_RGB() {
	char Y, Pb, Pr;
	int yprpb = 1,pbpr;
	int g_val = 0;
	float r,g,b;
	rgb_output_vals = new char[Width*Height * 3];
	convert_to_1D(final_matrix);

	for (int i = 0; i < Height * Width; i++) {
		Y = out_data[3 * i];
		Pb = out_data[3 * i + 1];
		Pr = out_data[3 * i + 2];
		pbpr = 0;
		if (yprpb == 1) {
			if (g_val == 0) {
				r = (float)((1.000 * Y) + (0.000 * Pb) + (1.402 * Pr));
				g = (float)((1.000 * Y) - (0.344 * Pb) - (0.714 * Pr));
				b = (float)((1.000 * Y) + (1.772 * Pb) + (0.000 * Pr));
				rgb_output_vals[3 * i] = r;
				rgb_output_vals[3 * i + 1] = g;
				rgb_output_vals[3 * i + 2] = b;
			}
		}
	}
	//moreValue(rgb_output_vals);
	
	for (int i = 0; i < (Height * Width * 3); i++) {
		Data[i] = rgb_output_vals[i];
	}
	
}

void MyImage::convert_RGB_to_YPbPr() {
	unsigned char r, g, b;
	float* new_data = new float[Width*Height * 3];
	float Y, Pb, Pr;
	int rgb_yprpb = 1, r_val = 1;
	int g_val = 0;
	for (int i = 0; i < Height * Width; i++) {
		r = Data[3 * i];
		g = Data[3 * i + 1];
		b = Data[3 * i + 2];
		if ((rgb_yprpb == 1) && r_val ==1 ) {
			if (g_val == 0) {
				Y = (float)((0.299 * r) + (0.587 * g) + (0.114 * b));
				Pb = (float)((-0.169 * r) - (0.331 * g) + (0.5 * b));
				Pr = (float)((0.5 * r) - (0.419 * g) - (0.081 * b));
			}
			new_data[3 * i] = Y;
			new_data[3 * i + 1] = Pb;
			new_data[3 * i + 2] = Pr;
		}

	}
	if (rgb_yprpb == 1) {
		convert_data = convert_matrix_2d(new_data, Height, Width);
	}
	else {
		convert_data = 0;
	}
}

float** MyImage::convert_matrix_2d(float* convert_matrix, int height, int width) {

	int val = width * 3;
	int conv_2d = 1,k;
	convert_data = new float*[height];
	for (int i = 0; i < height; i++) {
		convert_data[i] = new float[val];
		conv_2d = 1;
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width * 3; j++) {
			if (conv_2d == 1) {
				int k = i * (width * 3) + j;
				convert_data[i][j] = (float)convert_matrix[k];
			}
		}
	}
	return convert_data;
}

void MyImage::Direct_Cosine_Transform(float** final_result)
{
	float cu = 0, cv = 0, dct;
	int dct_flag = 1;
	dct_coefficient = new float*[8];

	for (int i = 0; i < 8; i++) {
		if (dct_flag == 1) {
			dct_coefficient[i] = new float[8];
		}
		
	}
	if(dct_flag ==1){
	for (int u = 0; u < 8; ++u) {
		for (int v = 0; v < 8; ++v) {
			dct_coefficient[u][v] = 0;
			dct = 0;
			if (dct_flag == 1) {
				for (int x = 0; x < 8; x++) {
					for (int y = 0; y < 8; y++) {
						if (dct_flag == 1) {
							dct += final_result[x][y] * cos(((2 * x + 1) * u * 3.1412) / (float)(16)) * cos(((2 * y + 1) * v * 3.1412) / (float)(16));
						}
					}
				}
			}

			cu = (u == 0) ? 1 / (float)sqrt(2) : 1.0;
			cv = (v == 0) ? 1 / (float)sqrt(2) : 1.0;

			dct_coefficient[u][v] = (1/(float)4.0) * cu * cv * dct;
		}
	}
  }
}

void MyImage::Inverse_DCT(float** DCT_Matrix) {
	int i, j, u, v;
	int idct_val = 1;
	idct_coefficient = new float*[8];
	int idct_mat = 0;
	for (int i = 0; i < 8; i++) {
		if ((idct_val == 1) && idct_mat ==0) {
			idct_coefficient[i] = new float[8];
		}
	}

	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			float idct = 0, cu = 0, cv = 0;

			if((idct_val == 1) && idct_mat == 0){
			for (int u = 0; u < 8; u++) {
				for (int v = 0; v < 8; v++) {
					if ((idct_val == 1) && idct_mat == 0) {
						cu = (u == 0) ? 1 / (float)sqrt(2) : 1.0;
						cv = (v == 0) ? 1 / (float)sqrt(2) : 1.0;
					}
					idct += cu * cv * DCT_Matrix[u][v] * cos(((2 * x + 1) * u * 3.1412) / (float)(16)) * cos(((2 * y + 1) * v * 3.1412) / (float)(16));
				}
			}
		}
			idct_coefficient[x][y] = idct / (float) 4.0;
		}
	}

}

float** MyImage::fetch_diagonals(float** dct_coefficient, int diagonal) {

	int dimension = 8;
	quantizedMatrix = new float*[dimension];
	int last_value = dimension * dimension - 1;
	int current_number = 0;
	int current_diagonal = 0;
	int diagonal_value = 1;
	int from, to, row, col;

	for (int i = 0; i < dimension; i++) {
		diagonal_value = 1;
		if (dimension == 8) {
			quantizedMatrix[i] = new float[dimension];
		}
	}

	for (current_diagonal = 0; current_diagonal <= last_value; current_diagonal++)
	{
		if ((current_diagonal < dimension) && diagonal_value == 1) // if doing the upper-left triangular half
		{
			from = 0;
			to = current_diagonal;
		}
		else // doing the bottom-right triangular half
		{
			from = current_diagonal - dimension + 1;
			to = dimension - 1;
			diagonal_value = 1;
		}

		for (int i = from; i <= to; i++)
		{
			if ((current_diagonal % 2 == 0) && diagonal_value == 1)
			{
				row = to - i + from;
				col = i;
			}
			else
			{
				row = i;
				col = to - i + from;
			}
			if ((current_diagonal >= diagonal) && diagonal_value == 1 )
				quantizedMatrix[row][col] = 0;
			else
				quantizedMatrix[row][col] = dct_coefficient[row][col];
		}
	} 
	return quantizedMatrix;
}

void MyImage::compute_DCT() {
	final_matrix = new float*[Height];
	int com_dct = 0;
	int compute_value = 0;
	float** result_Matrix = new float*[8];
	float** quantization_matrix = new float*[8];
	
	for (int j = 0; j < Height; j++) {
		if ((compute_value == 0) && com_dct == 0) {
			final_matrix[j] = new float[Width * 3];
		}
		else {
			compute_value = 1;
		}
	}
	if (com_dct == 0) {
		for (int i = 0; i < 8; i++) {
			if ((compute_value == 0) && com_dct ==0) {
				result_Matrix[i] = new float[8];
				quantization_matrix[i] = new float[8];
			}
		}
	}

	computeFunctions(result_Matrix, quantization_matrix);
}
void MyImage::computeFunctions(float** result_Matrix, float** quantization_matrix) {

	int i, j, k;
	int c_val = 1;
	int q_mat = 0;
	for (i = 0; i < Height; i = i + 8) {
		for (j = 0; j < Width; j = j + 8) {
			for (k = 0; k < 3; k++) {
				if ((c_val == 1) && (q_mat == 0) ) {
					matrix_construction(convert_data, result_Matrix, i, 3 * j + k, 8, 8);
					Direct_Cosine_Transform(result_Matrix);
				}
				else {
					c_val = 0;
				}
				if (q_mat == 0) {
					quantization_matrix = fetch_diagonals(dct_coefficient, Diagonals);
					Inverse_DCT(quantization_matrix);
				}
				else {
					fill_matrix_values(i, 3 * j + k, 8, 8);
				}
				fill_matrix_values(i, 3 * j + k, 8, 8);
			}
		}
	}

}
// = operator overload
MyImage & MyImage::operator= (const MyImage &otherImage)
{
	Height = otherImage.Height;
	Width = otherImage.Width;
	Data = new char[Width*Height * 3];
	strcpy((char *)otherImage.ImagePath, ImagePath);

	for (int i = 0; i<(Height*Width * 3); i++)
	{
		Data[i] = otherImage.Data[i];
	}

	return *this;

}


// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage()
{

	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0)
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}

	// Create a valid output file pointer
	FILE *IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if (IN_FILE == NULL)
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width];
	char *Gbuf = new char[Height*Width];
	char *Bbuf = new char[Height*Width];

	for (i = 0; i < Width*Height; i++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}

	// Allocate Data structure and copy
	Data = new char[Width*Height * 3];
	for (i = 0; i < Height*Width; i++)
	{
		Data[3 * i] = Bbuf[i];
		Data[3 * i + 1] = Gbuf[i];
		Data[3 * i + 2] = Rbuf[i];
	}

	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(IN_FILE);

	return true;

}



// MyImage functions defined here
bool MyImage::WriteImage()
{
	// Verify ImagePath
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0)
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}

	// Create a valid output file pointer
	FILE *OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if (OUT_FILE == NULL)
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width];
	char *Gbuf = new char[Height*Width];
	char *Bbuf = new char[Height*Width];

	for (i = 0; i < Height*Width; i++)
	{
		Bbuf[i] = Data[3 * i];
		Gbuf[i] = Data[3 * i + 1];
		Rbuf[i] = Data[3 * i + 2];
	}


	// Write data to file
	for (i = 0; i < Width*Height; i++)
	{
		fputc(Rbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i++)
	{
		fputc(Gbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i++)
	{
		fputc(Bbuf[i], OUT_FILE);
	}

	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(OUT_FILE);

	return true;

}




// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify()
{

	// TO DO by student

	// sample operation
	for (int i = 0; i<Width*Height; i++)
	{
		Data[3 * i] = 0;
		Data[3 * i + 1] = 0;

	}

	return false;
}