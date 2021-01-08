#include <cmath>
#include <iostream>
#include <iterator>
#include <fstream>
#include <stdio.h>

#include <vector>
#include <C:/eigen-3.3.7/Eigen/Dense>

using namespace std;
using namespace Eigen;

// For the example, can can these
#define ORDER 4 //Order of the polynomial to be fit "n"
#define FRAMELEN 11 //Number of points to fit over "m", size of frame
#define NUM_POINTS 2050 //Number of data points in input signal
#define WRITE_COEFFS true //Write the computed coefficients to a file (true or false)

/* Global variables */
int g_numPoints = 0; //Number of data points in input signal

/* Functions */
double* read_in_data(void)
{
    double point;
    double* data = new double[NUM_POINTS];
    ifstream file("INPUT");
    if(file.is_open())
    {
        while(g_numPoints < NUM_POINTS && file >> point)
        {
            data[g_numPoints++] = point;
        }
        file.close();
    }
    return data;
}

void write_to_file(MatrixXd Out, int num_zeros)
{
    IOFormat CommaInitFmt(StreamPrecision, DontAlignCols, "\n", "\n", "", "", " ", "");
    ofstream file("smooth");
    if(file.is_open())
    {
        //0's at front
        for(int j = 0; j < num_zeros; ++j)
            file << 0 << "\n";
        file << Out.format(CommaInitFmt);
        file.close();   
    }
    
}

double* comp_coeff(int order, int framelen)
{
    //Initialize the even Vandermonde matrices
    int even_col = ceil((float)(order + 1) / 2);
    MatrixXd Je = MatrixXd::Zero(framelen, even_col);

    //Find even matrix
    int i = (1 - framelen) / 2;
    int j = (framelen - 1) / 2;
    for(; i <= j; ++i)
        for(int k = 0; k < even_col; ++k)
            Je(i + j, k) = pow(i, 2*k);
    
    //Solve for the co-efficients
    MatrixXd JeT = Je.transpose();
    MatrixXd C = ((JeT*Je).inverse()) * JeT;

    //Get coefficients and return
    double* coeff = new double[framelen];
    for(int c = 0; c < framelen; ++c)
    {
        coeff[c] = C(0, c);
        cout << coeff[c] << " ";
    }

    //Write to ACSII file
    if(WRITE_COEFFS)
    {
        ofstream file("coeffs");
        if(file.is_open())
        {   
            for(int c = 0; c < framelen; ++c)
                file << coeff[c] << "\n"; 
            file.close();   
        }
    }
    return coeff;
}

double* coeff_filter(double *data, int order, int framelen)
{
    /* Error check */
    if(order >= framelen)
    {
        cout << "Framlen must be greater than the order!" << endl;
        return NULL;
    }
    else if(framelen % 2 == 0)
    {
        cout << "Framelen must be odd!" << endl;
        return NULL;
    }

    /* Find the coefficients for the matrix (framlen coeff's to find),
    then initialize the matrices. Out is the smoothed output, C is the
    co-efficients and In is the data to be filtered */

    double *coeff = comp_coeff(order, framelen);
    int cols = g_numPoints;
    int rows = cols - framelen + 1;
    MatrixXd C = MatrixXd::Zero(rows, cols);
    MatrixXd In = MatrixXd::Zero(cols, 1);
    MatrixXd Out = MatrixXd::Zero(rows, 1);

    for(int i = 0; i < rows; ++i)
        for(int j = 0; j < framelen; ++j)
            C(i, j + i) = coeff[j];
    
    /* Read in the data, perform convolution and write to file*/
    int i = 0;
    while(i < g_numPoints)
    {
        In(i, 0) = data[i];
        i++;
    }
    Out = C * In;
    write_to_file(Out, (framelen - 1) / 2);

    /* Return the filtered points (with 0's at front)*/
    int j;
    double* filtered = new double[rows + ((framelen-1) / 2)];
    for(j = 0; j < (framelen - 1) / 2; ++j)
        filtered[j] = 0;
    for(i = 0; i < rows; ++i)
        filtered[i + j] = Out(i, 0);
    
    return filtered;
}

/* Read in the data from the file and smooth */
int main(void)
{
    double* data = read_in_data();
    double* smooth = coeff_filter(data, ORDER, FRAMELEN);
    free(data);
    free(smooth);
    cout << endl << "----------------------- Done ----------------------" << endl;
}


