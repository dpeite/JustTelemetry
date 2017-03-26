#include "Arduino.h"
#include "Matrices.h"

Matrices::Matrices(int tam)
{
  _tam = tam;
}

// Corregida
void Matrices::mulMatrizVector(float* matriz, float* vector, float* res)
{
	res[0] = matriz[0] * vector[0] + matriz[1] * vector [1] + matriz[2] * vector[2] + matriz[3] * vector[3] + matriz[4] * vector [4] + matriz[5] * vector[5];
	res[1] = matriz[6] * vector[0] + matriz[7] * vector [1] + matriz[8] * vector[2] + matriz[9] * vector[3] + matriz[10] * vector [4] + matriz[11] * vector[5];
	res[2] = matriz[12] * vector[0] + matriz[13] * vector [1] + matriz[14] * vector[2] + matriz[15] * vector[3] + matriz[16] * vector [4] + matriz[17] * vector[5];
  res[3] = matriz[18] * vector[0] + matriz[19] * vector [1] + matriz[20] * vector[2] + matriz[21] * vector[3] + matriz[22] * vector [4] + matriz[23] * vector[5];
  res[4] = matriz[24] * vector[0] + matriz[25] * vector [1] + matriz[26] * vector[2] + matriz[27] * vector[3] + matriz[28] * vector [4] + matriz[29] * vector[5];
  res[5] = matriz[30] * vector[0] + matriz[31] * vector [1] + matriz[32] * vector[2] + matriz[33] * vector[3] + matriz[34] * vector [4] + matriz[35] * vector[5];
}

// Corregida
void Matrices::mulMatrizMatriz(float* matriz_izq, float* matriz_der, float* res) {
  /*
	res[0] = matriz_izq[0] * matriz_der[0] + matriz_izq[1] * matriz_der[6] + matriz_izq[2] * matriz_der[12] + matriz_izq[3] * matriz_der[18] + matriz_izq[4] * matriz_der[24] + matriz_izq[5] * matriz_der[30];
	res[1] = matriz_izq[0] * matriz_der[1] + matriz_izq[1] * matriz_der[7] + matriz_izq[2] * matriz_der[13] + matriz_izq[3] * matriz_der[19] + matriz_izq[4] * matriz_der[25] + matriz_izq[5] * matriz_der[31];
  res[2] = matriz_izq[0] * matriz_der[2] + matriz_izq[1] * matriz_der[8] + matriz_izq[2] * matriz_der[14] + matriz_izq[3] * matriz_der[20] + matriz_izq[4] * matriz_der[26] + matriz_izq[5] * matriz_der[32];
  res[3] = matriz_izq[0] * matriz_der[3] + matriz_izq[1] * matriz_der[9] + matriz_izq[2] * matriz_der[15] + matriz_izq[3] * matriz_der[21] + matriz_izq[4] * matriz_der[27] + matriz_izq[5] * matriz_der[33];
  res[4] = matriz_izq[0] * matriz_der[4] + matriz_izq[1] * matriz_der[10] + matriz_izq[2] * matriz_der[16] + matriz_izq[3] * matriz_der[22] + matriz_izq[4] * matriz_der[28] + matriz_izq[5] * matriz_der[34];
  res[5] = matriz_izq[0] * matriz_der[5] + matriz_izq[1] * matriz_der[11] + matriz_izq[2] * matriz_der[17] + matriz_izq[3] * matriz_der[23] + matriz_izq[4] * matriz_der[29] + matriz_izq[5] * matriz_der[35];
  
  res[6] = matriz_izq[6] * matriz_der[0] + matriz_izq[7] * matriz_der[6] + matriz_izq[8] * matriz_der[12] + matriz_izq[9] * matriz_der[18] + matriz_izq[10] * matriz_der[24] + matriz_izq[11] * matriz_der[30];
  res[7] = matriz_izq[6] * matriz_der[1] + matriz_izq[7] * matriz_der[7] + matriz_izq[8] * matriz_der[13] + matriz_izq[9] * matriz_der[19] + matriz_izq[10] * matriz_der[25] + matriz_izq[11] * matriz_der[31];
  res[8] = matriz_izq[6] * matriz_der[2] + matriz_izq[7] * matriz_der[8] + matriz_izq[8] * matriz_der[14] + matriz_izq[9] * matriz_der[20] + matriz_izq[10] * matriz_der[26] + matriz_izq[11] * matriz_der[32];
  res[9] = matriz_izq[6] * matriz_der[3] + matriz_izq[7] * matriz_der[9] + matriz_izq[8] * matriz_der[15] + matriz_izq[9] * matriz_der[21] + matriz_izq[10] * matriz_der[27] + matriz_izq[11] * matriz_der[33];
  res[10] = matriz_izq[6] * matriz_der[4] + matriz_izq[7] * matriz_der[10] + matriz_izq[8] * matriz_der[16] + matriz_izq[9] * matriz_der[22] + matriz_izq[10] * matriz_der[28] + matriz_izq[11] * matriz_der[34];
  res[11] = matriz_izq[6] * matriz_der[5] + matriz_izq[7] * matriz_der[11] + matriz_izq[8] * matriz_der[17] + matriz_izq[9] * matriz_der[23] + matriz_izq[10] * matriz_der[29] + matriz_izq[11] * matriz_der[35];

  res[12] = matriz_izq[12] * matriz_der[0] + matriz_izq[13] * matriz_der[6] + matriz_izq[14] * matriz_der[12] + matriz_izq[15] * matriz_der[18] + matriz_izq[16] * matriz_der[24] + matriz_izq[17] * matriz_der[30];
  res[13] = matriz_izq[12] * matriz_der[1] + matriz_izq[13] * matriz_der[7] + matriz_izq[14] * matriz_der[13] + matriz_izq[15] * matriz_der[19] + matriz_izq[16] * matriz_der[25] + matriz_izq[17] * matriz_der[31];
  res[14] = matriz_izq[12] * matriz_der[2] + matriz_izq[13] * matriz_der[8] + matriz_izq[14] * matriz_der[14] + matriz_izq[15] * matriz_der[20] + matriz_izq[16] * matriz_der[26] + matriz_izq[17] * matriz_der[32];
  res[15] = matriz_izq[12] * matriz_der[3] + matriz_izq[13] * matriz_der[9] + matriz_izq[14] * matriz_der[15] + matriz_izq[15] * matriz_der[21] + matriz_izq[16] * matriz_der[27] + matriz_izq[17] * matriz_der[33];
  res[16] = matriz_izq[12] * matriz_der[4] + matriz_izq[13] * matriz_der[10] + matriz_izq[14] * matriz_der[16] + matriz_izq[15] * matriz_der[22] + matriz_izq[16] * matriz_der[28] + matriz_izq[17] * matriz_der[34];
  res[17] = matriz_izq[12] * matriz_der[5] + matriz_izq[13] * matriz_der[11] + matriz_izq[14] * matriz_der[17] + matriz_izq[15] * matriz_der[23] + matriz_izq[16] * matriz_der[29] + matriz_izq[17] * matriz_der[35];

  res[18] = matriz_izq[18] * matriz_der[0] + matriz_izq[19] * matriz_der[6] + matriz_izq[20] * matriz_der[12] + matriz_izq[21] * matriz_der[18] + matriz_izq[22] * matriz_der[24] + matriz_izq[23] * matriz_der[30];
  res[19] = matriz_izq[18] * matriz_der[1] + matriz_izq[19] * matriz_der[7] + matriz_izq[20] * matriz_der[13] + matriz_izq[21] * matriz_der[19] + matriz_izq[22] * matriz_der[25] + matriz_izq[23] * matriz_der[31];
  res[20] = matriz_izq[18] * matriz_der[2] + matriz_izq[19] * matriz_der[8] + matriz_izq[20] * matriz_der[14] + matriz_izq[21] * matriz_der[20] + matriz_izq[22] * matriz_der[26] + matriz_izq[23] * matriz_der[32];
  res[21] = matriz_izq[18] * matriz_der[3] + matriz_izq[19] * matriz_der[9] + matriz_izq[20] * matriz_der[15] + matriz_izq[21] * matriz_der[21] + matriz_izq[22] * matriz_der[27] + matriz_izq[23] * matriz_der[33];
  res[22] = matriz_izq[18] * matriz_der[4] + matriz_izq[19] * matriz_der[10] + matriz_izq[20] * matriz_der[16] + matriz_izq[21] * matriz_der[22] + matriz_izq[22] * matriz_der[28] + matriz_izq[23] * matriz_der[34];
  res[23] = matriz_izq[18] * matriz_der[5] + matriz_izq[19] * matriz_der[11] + matriz_izq[20] * matriz_der[17] + matriz_izq[21] * matriz_der[23] + matriz_izq[22] * matriz_der[29] + matriz_izq[23] * matriz_der[35];

  res[24] = matriz_izq[24] * matriz_der[0] + matriz_izq[25] * matriz_der[6] + matriz_izq[26] * matriz_der[12] + matriz_izq[27] * matriz_der[18] + matriz_izq[28] * matriz_der[24] + matriz_izq[29] * matriz_der[30];
  res[25] = matriz_izq[24] * matriz_der[1] + matriz_izq[25] * matriz_der[7] + matriz_izq[26] * matriz_der[13] + matriz_izq[27] * matriz_der[19] + matriz_izq[28] * matriz_der[25] + matriz_izq[29] * matriz_der[31];
  res[26] = matriz_izq[24] * matriz_der[2] + matriz_izq[25] * matriz_der[8] + matriz_izq[26] * matriz_der[14] + matriz_izq[27] * matriz_der[20] + matriz_izq[28] * matriz_der[26] + matriz_izq[29] * matriz_der[32];
  res[27] = matriz_izq[24] * matriz_der[3] + matriz_izq[25] * matriz_der[9] + matriz_izq[26] * matriz_der[15] + matriz_izq[27] * matriz_der[21] + matriz_izq[28] * matriz_der[27] + matriz_izq[29] * matriz_der[33];
  res[28] = matriz_izq[24] * matriz_der[4] + matriz_izq[25] * matriz_der[10] + matriz_izq[26] * matriz_der[16] + matriz_izq[27] * matriz_der[22] + matriz_izq[28] * matriz_der[28] + matriz_izq[29] * matriz_der[34];
  res[29] = matriz_izq[24] * matriz_der[5] + matriz_izq[25] * matriz_der[11] + matriz_izq[26] * matriz_der[17] + matriz_izq[27] * matriz_der[23] + matriz_izq[28] * matriz_der[29] + matriz_izq[29] * matriz_der[35];

  res[30] = matriz_izq[30] * matriz_der[0] + matriz_izq[31] * matriz_der[6] + matriz_izq[32] * matriz_der[12] + matriz_izq[33] * matriz_der[18] + matriz_izq[34] * matriz_der[24] + matriz_izq[35] * matriz_der[30];
  res[31] = matriz_izq[30] * matriz_der[1] + matriz_izq[31] * matriz_der[7] + matriz_izq[32] * matriz_der[13] + matriz_izq[33] * matriz_der[19] + matriz_izq[34] * matriz_der[25] + matriz_izq[35] * matriz_der[31];
  res[32] = matriz_izq[30] * matriz_der[2] + matriz_izq[31] * matriz_der[8] + matriz_izq[32] * matriz_der[14] + matriz_izq[33] * matriz_der[20] + matriz_izq[34] * matriz_der[26] + matriz_izq[35] * matriz_der[32];
  res[33] = matriz_izq[30] * matriz_der[3] + matriz_izq[31] * matriz_der[9] + matriz_izq[32] * matriz_der[15] + matriz_izq[33] * matriz_der[21] + matriz_izq[34] * matriz_der[27] + matriz_izq[35] * matriz_der[33];
  res[34] = matriz_izq[30] * matriz_der[4] + matriz_izq[31] * matriz_der[10] + matriz_izq[32] * matriz_der[16] + matriz_izq[33] * matriz_der[22] + matriz_izq[34] * matriz_der[28] + matriz_izq[35] * matriz_der[34];
  res[35] = matriz_izq[30] * matriz_der[5] + matriz_izq[31] * matriz_der[11] + matriz_izq[32] * matriz_der[17] + matriz_izq[33] * matriz_der[23] + matriz_izq[34] * matriz_der[29] + matriz_izq[35] * matriz_der[35];
  */

  // A = input matrix (m x p)
  // B = input matrix (p x n)
  // m = number of rows in A
  // p = number of columns in A = number of rows in B
  // n = number of columns in B
  // C = output matrix = A*B (m x n)
  int i, j, k;
  for (i = 0; i < _tam; i++)
    for(j = 0; j < _tam; j++)
    {
      res[_tam * i + j] = 0;
      for (k = 0; k < _tam; k++)
        res[_tam * i + j] = res[_tam * i + j] + matriz_izq[_tam * i + k] * matriz_der[_tam * k + j];
    }
}

// Corregida
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
  res[9] = matriz_izq[9] + matriz_der[9];
  res[10] = matriz_izq[10] + matriz_der[10];
  res[11] = matriz_izq[11] + matriz_der[11];

  res[12] = matriz_izq[12] + matriz_der[12];
  res[13] = matriz_izq[13] + matriz_der[13];  
  res[14] = matriz_izq[14] + matriz_der[14];
  res[15] = matriz_izq[15] + matriz_der[15];  
  res[16] = matriz_izq[16] + matriz_der[16];
  res[17] = matriz_izq[17] + matriz_der[17]; 

  res[18] = matriz_izq[18] + matriz_der[18];
  res[19] = matriz_izq[19] + matriz_der[19];  
  res[20] = matriz_izq[20] + matriz_der[20];
  res[21] = matriz_izq[21] + matriz_der[21];  
  res[22] = matriz_izq[22] + matriz_der[22];
  res[23] = matriz_izq[23] + matriz_der[23];

  res[24] = matriz_izq[24] + matriz_der[24];
  res[25] = matriz_izq[25] + matriz_der[25];  
  res[26] = matriz_izq[26] + matriz_der[26];
  res[27] = matriz_izq[27] + matriz_der[27];  
  res[28] = matriz_izq[28] + matriz_der[28];
  res[29] = matriz_izq[29] + matriz_der[29];

  res[30] = matriz_izq[30] + matriz_der[30];
  res[31] = matriz_izq[31] + matriz_der[31];  
  res[32] = matriz_izq[32] + matriz_der[32];
  res[33] = matriz_izq[33] + matriz_der[33];  
  res[34] = matriz_izq[34] + matriz_der[34];
  res[35] = matriz_izq[35] + matriz_der[35];
}

// Corregida
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
  res[9] = matriz_izq[9] - matriz_der[9];
  res[10] = matriz_izq[10] - matriz_der[10];
  res[11] = matriz_izq[11] - matriz_der[11];

  res[12] = matriz_izq[12] - matriz_der[12];
  res[13] = matriz_izq[13] - matriz_der[13];  
  res[14] = matriz_izq[14] - matriz_der[14];
  res[15] = matriz_izq[15] - matriz_der[15];  
  res[16] = matriz_izq[16] - matriz_der[16];
  res[17] = matriz_izq[17] - matriz_der[17]; 

  res[18] = matriz_izq[18] - matriz_der[18];
  res[19] = matriz_izq[19] - matriz_der[19];  
  res[20] = matriz_izq[20] - matriz_der[20];
  res[21] = matriz_izq[21] - matriz_der[21];  
  res[22] = matriz_izq[22] - matriz_der[22];
  res[23] = matriz_izq[23] - matriz_der[23];

  res[24] = matriz_izq[24] - matriz_der[24];
  res[25] = matriz_izq[25] - matriz_der[25];  
  res[26] = matriz_izq[26] - matriz_der[26];
  res[27] = matriz_izq[27] - matriz_der[27];  
  res[28] = matriz_izq[28] - matriz_der[28];
  res[29] = matriz_izq[29] - matriz_der[29];

  res[30] = matriz_izq[30] - matriz_der[30];
  res[31] = matriz_izq[31] - matriz_der[31];  
  res[32] = matriz_izq[32] - matriz_der[32];
  res[33] = matriz_izq[33] - matriz_der[33];  
  res[34] = matriz_izq[34] - matriz_der[34];
  res[35] = matriz_izq[35] - matriz_der[35];
}

void Matrices::trasponerMatriz(float* matriz, float* res) {

  // A = input matrix (m x n)
  // m = number of rows in A
  // n = number of columns in A
  // C = output matrix = the transpose of A (n x m)
  int i, j;
  for (i = 0; i < _tam; i++)
    for(j = 0; j < _tam; j++)
      res[_tam * j + i] = matriz[_tam * i + j];
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

//Corregida
void Matrices::sumaVectorVector(float * A, float * B, float * res) {
  res[0] = A[0] + B[0];
  res[1] = A[1] + B[1];
  res[2] = A[2] + B[2];
  res[3] = A[3] + B[3];
  res[4] = A[4] + B[4];
  res[5] = A[5] + B[5];
}

//Corregida
void Matrices::restaVectorVector(float * A, float * B, float * res) {
  res[0] = A[0] - B[0];
  res[1] = A[1] - B[1];
  res[2] = A[2] - B[2];
  res[3] = A[3] - B[3];
  res[4] = A[4] - B[4];
  res[5] = A[5] - B[5];
}

void Matrices::imprimirMatriz(float* A) {
  Serial.print(A[0]);
  Serial.print(" | ");
  Serial.print(A[1]);
  Serial.print(" | ");
  Serial.print(A[2]);
  Serial.print(" | ");
  Serial.print(A[3]);
  Serial.print(" | ");
  Serial.print(A[4]);
  Serial.print(" | ");
  Serial.println(A[5]);

  Serial.print(A[6]);
  Serial.print(" | ");
  Serial.print(A[7]);
  Serial.print(" | ");
  Serial.print(A[8]);
  Serial.print(" | ");
  Serial.print(A[9]);
  Serial.print(" | ");
  Serial.print(A[10]);
  Serial.print(" | ");
  Serial.println(A[11]);

  Serial.print(A[12]);
  Serial.print(" | ");
  Serial.print(A[13]);
  Serial.print(" | ");
  Serial.print(A[14]);
  Serial.print(" | ");
  Serial.print(A[15]);
  Serial.print(" | ");
  Serial.print(A[16]);
  Serial.print(" | ");
  Serial.println(A[17]);

  Serial.print(A[18]);
  Serial.print(" | ");
  Serial.print(A[19]);
  Serial.print(" | ");
  Serial.print(A[20]);
  Serial.print(" | ");
  Serial.print(A[21]);
  Serial.print(" | ");
  Serial.print(A[22]);
  Serial.print(" | ");
  Serial.println(A[23]);

  Serial.print(A[24]);
  Serial.print(" | ");
  Serial.print(A[25]);
  Serial.print(" | ");
  Serial.print(A[26]);
  Serial.print(" | ");
  Serial.print(A[27]);
  Serial.print(" | ");
  Serial.print(A[28]);
  Serial.print(" | ");
  Serial.println(A[29]);

  Serial.print(A[30]);
  Serial.print(" | ");
  Serial.print(A[31]);
  Serial.print(" | ");
  Serial.print(A[32]);
  Serial.print(" | ");
  Serial.print(A[33]);
  Serial.print(" | ");
  Serial.print(A[34]);
  Serial.print(" | ");
  Serial.println(A[35]);
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