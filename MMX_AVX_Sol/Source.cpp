#include <iostream>
#include <random>
#include <immintrin.h>
#include <intrin.h>
#include <iomanip>

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
		ymmA = _mm256_load_ps(A+i);
		ymmB = _mm256_load_ps(B+i);
		ymmC = _mm256_fmadd_ps(ymmA, ymmB, ymmA);

		_mm256_store_ps(C3+i, ymmC);
	}
	std::cout << std::setprecision(15) << std::fixed;
	for (int i = 0; i < VECTOR_SIZE; i++)
	{
		if (C1[i] != C2[i])
		{
			std::cout << "C1[" << i << "]: " << C1[i] << "	but C2[" << i << "]: " << C2[i] << std::endl;
		}
		if (C2[i] != C3[i])
		{
			std::cout << "C2[" << i << "]: " << C2[i] << "	but C3[" << i << "]: " << C3[i] << std::endl;
		}
		if (C3[i] != C1[i])
		{
			std::cout << "C3[" << i << "]: " << C3[i] << "	but C1[" << i << "]: " << C1[i] << std::endl;
		}
	}

	system("pause");
}