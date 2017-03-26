#include "Arduino.h"
#include "PruebasKalman.h"

PruebasKalman::PruebasKalman(int tam)
{
	_tam = tam;
}

	//--------------------------------------------------------------
	// Valores pequeños en R
	//--------------------------------------------------------------

	/*
	 * Valores por defecto en Q (1), R00 valor muy pequeño (0.05)
	 */
	 void PruebasKalman::prueba1(float* R, float* Q) {
	 	R[0] = 0.05;
	 }

	/*
	 * Valores por defecto en Q (1), R11 valor muy pequeño (0.05)
	 */
	 void PruebasKalman::prueba2(float* R, float* Q) {
	 	R[7] = 0.05;
	 }

    /*
	 * Valores por defecto en Q (1), R22 valor muy pequeño (0.05)
	 */
	 void PruebasKalman::prueba3(float* R, float* Q) {
	 	R[14] = 0.05;
	 }

    /*
	 * Valores por defecto en Q (1), R33 valor muy pequeño (0.05)
	 */
	 void PruebasKalman::prueba4(float* R, float* Q) {
	 	R[21] = 0.05;
	 }

    	/*
	 * Valores por defecto en Q (1), R44 valor muy pequeño (0.05)
	 */
	 void PruebasKalman::prueba5(float* R, float* Q) {
	 	R[28] = 0.05;
	 }

    /*
	 * Valores por defecto en Q (1), R55 valor muy pequeño (0.05)
	 */
	 void PruebasKalman::prueba6(float* R, float* Q) {
	 	R[35] = 0.05;
	 }

	/*
	 * Valores por defecto en Q (1), valores muy pequeños (0.05) en las componentes de la posición (R00 y R11)
	 */
	 void PruebasKalman::prueba7(float* R, float* Q) {
	 	R[0] = 0.05;
	 	R[7] = 0.05;
	 }

	/*
	 * Valores por defecto en Q (1), valores muy pequeños (0.05) en las componentes de la velocidad (R22 y R33)
	 */
	 void PruebasKalman::prueba8(float* R, float* Q) {
	 	R[14] = 0.05;
	 	R[21] = 0.05;
	 }

	/*
	 * Valores por defecto en Q (1), valores muy pequeños (0.05) en las componentes de la aceleración (R44 y R55)
	 */
	 void PruebasKalman::prueba9(float* R, float* Q) {
	 	R[28] = 0.05;
	 	R[35] = 0.05;
	 }

	 //--------------------------------------------------------------
	 // Valores grandes en R
	 //--------------------------------------------------------------

	 /*
	 * Valores por defecto en Q (1), R00 valor muy grande (200)
	 */
	 void PruebasKalman::prueba10(float* R, float* Q) {
	 	R[0] = 200;
	 }

	/*
	 * Valores por defecto en Q (1), R11 valor muy grande (200)
	 */
	 void PruebasKalman::prueba11(float* R, float* Q) {
	 	R[7] = 200;
	 }

    /*
	 * Valores por defecto en Q (1), R22 valor muy grande (200)
	 */
	 void PruebasKalman::prueba12(float* R, float* Q) {
	 	R[14] = 200;
	 }

    /*
	 * Valores por defecto en Q (1), R33 valor muy grande (200)
	 */
	 void PruebasKalman::prueba13(float* R, float* Q) {
	 	R[21] = 200;
	 }

    	/*
	 * Valores por defecto en Q (1), R44 valor muy grande (200)
	 */
	 void PruebasKalman::prueba14(float* R, float* Q) {
	 	R[28] = 200;
	 }

    /*
	 * Valores por defecto en Q (1), R55 valor muy grande (200)
	 */
	 void PruebasKalman::prueba15(float* R, float* Q) {
	 	R[35] = 200;
	 }

	/*
	 * Valores por defecto en Q (1), valores muy grandes (200) en las componentes de la posición (R00 y R11)
	 */
	 void PruebasKalman::prueba16(float* R, float* Q) {
	 	R[0] = 200;
	 	R[7] = 200;
	 }

	/*
	 * Valores por defecto en Q (1), valores muy grandes (200) en las componentes de la velocidad (R22 y R33)
	 */
	 void PruebasKalman::prueba17(float* R, float* Q) {
	 	R[14] = 200;
	 	R[21] = 200;
	 }

	/*
	 * Valores por defecto en Q (1), valores muy grandes (200) en las componentes de la aceleración (R44 y R55)
	 */
	 void PruebasKalman::prueba18(float* R, float* Q) {
	 	R[28] = 200;
	 	R[35] = 200;
	 }

	//--------------------------------------------------------------
	// Matriz Q
	//--------------------------------------------------------------

	//--------------------------------------------------------------
	// Valores pequeños en Q
	//--------------------------------------------------------------

	/*
	 * Valores por defecto en R (2), Q00 valor muy pequeño (0.05)
	 */
	 void PruebasKalman::prueba19(float* R, float* Q) {
	 	Q[0] = 0.05;
	 }

	/*
	 * Valores por defecto en R (2), Q11 valor muy pequeño (0.05)
	 */
	 void PruebasKalman::prueba20(float* R, float* Q) {
	 	Q[7] = 0.05;
	 }

	/*
	 * Valores por defecto en R (2), Q22 valor muy pequeño (0.05)
	 */
	 void PruebasKalman::prueba21(float* R, float* Q) {
	 	Q[14] = 0.05;
	 }

	/*
	 * Valores por defecto en R (2), Q33 valor muy pequeño (0.05)
	 */
	 void PruebasKalman::prueba22(float* R, float* Q) {
	 	Q[21] = 0.05;
	 }

	/*
	 * Valores por defecto en R (2), Q44 valor muy pequeño (0.05)
	 */
	 void PruebasKalman::prueba23(float* R, float* Q) {
	 	Q[28] = 0.05;
	 }

	/*
	 * Valores por defecto en R (2), Q55 valor muy pequeño (0.05)
	 */
	 void PruebasKalman::prueba24(float* R, float* Q) {
	 	Q[35] = 0.05;
	 }

	/*
	 * Valores por defecto en R (2), valores muy pequeños (0.05) en las componentes de la posición (Q00 y Q11)
	 */
	 void PruebasKalman::prueba25(float* R, float* Q) {
	 	Q[0] = 0.05;
	 	Q[7] = 0.05;
	 }

		/*
	 * Valores por defecto en R (2), valores muy pequeños (0.05) en las componentes de la velocidad (Q22 y Q33)
	 */
	 void PruebasKalman::prueba26(float* R, float* Q) {
	 	Q[14] = 0.05;
	 	Q[21] = 0.05;
	 }

	 	/*
	 * Valores por defecto en R (2), valores muy pequeños (0.05) en las componentes de la aceleración (Q44 y Q55)
	 */
	 void PruebasKalman::prueba27(float* R, float* Q) {
	 	Q[28] = 0.05;
	 	Q[35] = 0.05;
	 }

	 //--------------------------------------------------------------
	// Valores grandes en Q
	//--------------------------------------------------------------

	/*
	 * Valores por defecto en R (2), Q00 valor muy grande (200)
	 */
	 void PruebasKalman::prueba28(float* R, float* Q) {
	 	Q[0] = 200;
	 }

	/*
	 * Valores por defecto en R (2), Q11 valor muy grande (200)
	 */
	 void PruebasKalman::prueba29(float* R, float* Q) {
	 	Q[7] = 200;
	 }

	/*
	 * Valores por defecto en R (2), Q22 valor muy grande (200)
	 */
	 void PruebasKalman::prueba30(float* R, float* Q) {
	 	Q[14] = 200;
	 }

	/*
	 * Valores por defecto en R (2), Q33 valor muy grande (200)
	 */
	 void PruebasKalman::prueba31(float* R, float* Q) {
	 	Q[21] = 200;
	 }

	/*
	 * Valores por defecto en R (2), Q44 valor muy grande (200)
	 */
	 void PruebasKalman::prueba32(float* R, float* Q) {
	 	Q[28] = 200;
	 }

	/*
	 * Valores por defecto en R (2), Q55 valor muy grande (200)
	 */
	 void PruebasKalman::prueba33(float* R, float* Q) {
	 	Q[35] = 200;
	 }

	/*
	 * Valores por defecto en R (2), valores muy pequeños (200) en las componentes de la posición (Q00 y Q11)
	 */
	 void PruebasKalman::prueba34(float* R, float* Q) {
	 	Q[0] = 200;
	 	Q[7] = 200;
	 }

	/*
	 * Valores por defecto en R (2), valores muy pequeños (200) en las componentes de la velocidad (Q22 y Q33)
	 */
	 void PruebasKalman::prueba35(float* R, float* Q) {
	 	Q[14] = 200;
	 	Q[21] = 200;
	 }

	/*
	 * Valores por defecto en R (2), valores muy pequeños (200) en las componentes de la aceleración (Q44 y Q55)
	 */
	 void PruebasKalman::prueba36(float* R, float* Q) {
	 	Q[28] = 200;
	 	Q[35] = 200;
	 }