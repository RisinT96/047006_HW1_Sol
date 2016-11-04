#include <iostream>
#include <random>
#include <immintrin.h>
#include <intrin.h>

#define VECTOR_SIZE 1024

int main() {

	__declspec(align(32))float A[VECTOR_SIZE];
	__declspec(align(32))float B[VECTOR_SIZE];
	__declspec(align(32))float C1[VECTOR_SIZE];
	__declspec(align(32))float C2[VECTOR_SIZE];
	__declspec(align(32))float C3[VECTOR_SIZE];

	std::random_device rand;
	std::mt19937 e2(rand());

	std::uniform_real_distribution<float> dist(0, 1);
	for (int i = 0; i < VECTOR_SIZE; i++)
	{
		A[i] = dist(e2);
	}

	dist = std::uniform_real_distribution<float>(10, 50);
	for (int i = 0; i < VECTOR_SIZE; i++)
	{
		B[i] = dist(e2);
	}

	//Scalar
	for (int i = 0; i < VECTOR_SIZE; i++)
	{
		C1[i] = A[i] + A[i]*B[i];
	}

	//Vector AVX2
	__m256 ymmA, ymmB, ymmC;
	for (int i = 0; i < VECTOR_SIZE; i += 8)
	{
		ymmA = _mm256_load_ps(A+i);
		ymmB = _mm256_load_ps(B+i);
		ymmC = _mm256_mul_ps(ymmA, ymmB);
		ymmC = _mm256_add_ps(ymmA, ymmC);
		
		_mm256_store_ps(C2+i, ymmC);
	}

	for (int i = 0; i < VECTOR_SIZE; i += 8)
	{
		ymmA = _mm256_load_ps(A);
		ymmB = _mm256_load_ps(B);
		ymmC = _mm256_fmadd_ps(ymmA, ymmB, ymmA);

		_mm256_store_ps(C3, ymmC);
	}

	for (int i = 0; i < VECTOR_SIZE; i++)
	{
		std::cout << C1[i] << " " << C2[i] << " " << C3[i] << std::endl;

	}

	system("pause");
}