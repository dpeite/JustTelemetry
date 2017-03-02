#include "Arduino.h"
#include "Matrices.h"

Matrices::Matrices(int tam)
{
  _tam = tam;
}

void Matrices::mulMatrizVector(float* matriz, float* vector, float* res)
{
	res[0] = matriz[0] * vector[0] + matriz[1] * vector [1] + matriz[2] * vector[2];
	res[1] = matriz[3] * vector[0] + matriz[4] * vector [1] + matriz[5] * vector[2];
	res[2] = matriz[6] * vector[0] + matriz[7] * vector [1] + matriz[8] * vector[2];
}

void Matrices::mulMatrizMatriz(float* matriz_izq, float* matriz_der, float* res) {
	res[0] = matriz_izq[0] * matriz_der[0] + matriz_izq[1] * matriz_der[3] + matriz_izq[2] * matriz_der[6];
	res[1] = matriz_izq[0] * matriz_der[1] + matriz_izq[1] * matriz_der[4] + matriz_izq[2] * matriz_der[7];
	res[2] = matriz_izq[0] * matriz_der[2] + matriz_izq[1] * matriz_der[5] + matriz_izq[2] * matriz_der[12];

	res[3] = matriz_izq[3] * matriz_der[0] + matriz_izq[4] * matriz_der[3] + matriz_izq[5] * matriz_der[6];
	res[4] = matriz_izq[3] * matriz_der[1] + matriz_izq[4] * matriz_der[4] + matriz_izq[5] * matriz_der[7];
	res[5] = matriz_izq[3] * matriz_der[2] + matriz_izq[4] * matriz_der[5] + matriz_izq[5] * matriz_der[8];

	res[6] = matriz_izq[6] * matriz_der[0] + matriz_izq[7] * matriz_der[3] + matriz_izq[8] * matriz_der[6];
	res[7] = matriz_izq[6] * matriz_der[1] + matriz_izq[7] * matriz_der[4] + matriz_izq[8] * matriz_der[7];
	res[8] = matriz_izq[6] * matriz_der[2] + matriz_izq[7] * matriz_der[5] + matriz_izq[8] * matriz_der[8];
}

void Matrices::sumaMatrizMatriz(float* matriz_izq, float* matriz_der, float* res) {

	res[0] = matriz_izq[0] + matriz_der[0];
	res[1] = matriz_izq[1] + matriz_der[1];
	res[2] = matriz_izq[2] + matriz_der[2];

	res[3] = matriz_izq[3] + matriz_der[3];
	res[4] = matriz_izq[4] + matriz_der[4];
	res[5] = matriz_izq[5] + matriz_der[5];

	res[6] = matriz_izq[6] + matriz_der[6];
	res[7] = matriz_izq[7] + matriz_der[7];
	res[8] = matriz_izq[8] + matriz_der[8];
}

void Matrices::restaMatrizMatriz(float* matriz_izq, float* matriz_der, float* res) {

  res[0] = matriz_izq[0] - matriz_der[0];
  res[1] = matriz_izq[1] - matriz_der[1];
  res[2] = matriz_izq[2] - matriz_der[2];

  res[3] = matriz_izq[3] - matriz_der[3];
  res[4] = matriz_izq[4] - matriz_der[4];
  res[5] = matriz_izq[5] - matriz_der[5];

  res[6] = matriz_izq[6] - matriz_der[6];
  res[7] = matriz_izq[7] - matriz_der[7];
  res[8] = matriz_izq[8] - matriz_der[8];
}

void Matrices::trasponerMatriz(float* matriz, float* res) {

	res[1] = matriz[3];
	res[2] = matriz[6];
	res[5] = matriz[7];

	res[3] = matriz[1];
	res[6] = matriz[2];
	res[7] = matriz[5];

	res[0] = matriz[0];
	res[4] = matriz[4];
	res[8] = matriz[8];
}

int Matrices::invertirMatriz(float* A) {
 	// A = input matrix AND result matrix
    // n = number of rows = number of columns in A (n x n)
    int n = _tam;
    int pivrow;     // keeps track of current pivot row
    int k,i,j;      // k: overall index along diagonal; i: row index; j: col index
    int pivrows[n]; // keeps track of rows swaps to undo at end
    float tmp;      // used for finding max value and making column swaps

    for (k = 0; k < n; k++)
    {
        // find pivot row, the row with biggest entry in current column
    	tmp = 0;
    	for (i = k; i < n; i++)
    	{
            if (abs(A[i*n+k]) >= tmp)   // 'Avoid using other functions inside abs()?'
            {
            	tmp = abs(A[i*n+k]);
            	pivrow = i;
            }
        }

        // check for singular matrix
        if (A[pivrow*n+k] == 0.0f)
        {
        	Serial.println("Inversion failed due to singular matrix");
        	return 0;
        }

        // Execute pivot (row swap) if needed
        if (pivrow != k)
        {
            // swap row k with pivrow
        	for (j = 0; j < n; j++)
        	{
        		tmp = A[k*n+j];
        		A[k*n+j] = A[pivrow*n+j];
        		A[pivrow*n+j] = tmp;
        	}
        }
        pivrows[k] = pivrow;    // record row swap (even if no swap happened)

        tmp = 1.0f/A[k*n+k];    // invert pivot element
        A[k*n+k] = 1.0f;        // This element of input matrix becomes result matrix

        // Perform row reduction (divide every element by pivot)
        for (j = 0; j < n; j++)
        {
        	A[k*n+j] = A[k*n+j]*tmp;
        }

        // Now eliminate all other entries in this column
        for (i = 0; i < n; i++)
        {
        	if (i != k)
        	{
        		tmp = A[i*n+k];
                A[i*n+k] = 0.0f;  // The other place where in matrix becomes result mat
                for (j = 0; j < n; j++)
                {
                	A[i*n+j] = A[i*n+j] - A[k*n+j]*tmp;
                }
            }
        }
    }

    // Done, now need to undo pivot row swaps by doing column swaps in reverse order
    for (k = n-1; k >= 0; k--)
    {
    	if (pivrows[k] != k)
    	{
    		for (i = 0; i < n; i++)
    		{
    			tmp = A[i*n+k];
    			A[i*n+k] = A[i*n+pivrows[k]];
    			A[i*n+pivrows[k]] = tmp;
    		}
    	}
    }
    return 1;
}

void Matrices::sumaVectorVector(float * A, float * B, float * res) {
  res[0] = A[0] + B[0];
  res[1] = A[1] + B[1];
  res[2] = A[2] + B[2];
}

void Matrices::restaVectorVector(float * A, float * B, float * res) {
  res[0] = A[0] - B[0];
  res[1] = A[1] - B[1];
  res[2] = A[2] - B[2];
}

void Matrices::imprimirMatriz(float* A) {
  Serial.print(A[0]);
  Serial.print(" | ");
  Serial.print(A[1]);
  Serial.print(" | ");
  Serial.println(A[2]);
  Serial.print(A[3]);
  Serial.print(" | ");
  Serial.print(A[4]);
  Serial.print(" | ");
  Serial.println(A[5]);
  Serial.print(A[6]);
  Serial.print(" | ");
  Serial.print(A[7]);
  Serial.print(" | ");
  Serial.println(A[8]);
  Serial.print("\n");
}

void Matrices::imprimirVector(float* res) {
  Serial.print(res[0]);
  Serial.print(" | ");
  Serial.print(res[1]);
  Serial.print(" | ");
  Serial.println(res[2]);
  Serial.print("\n");
}