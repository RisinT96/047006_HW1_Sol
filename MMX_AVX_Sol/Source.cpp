#include <iostream>
#include <random>
#include <iomanip>
#include <string>
#include <chrono>
#include <immintrin.h>

#include "Detector.h"

#define VECTOR_SIZE 1024
#define LOOPS 100000
int main() {
	bool flag = false;
	int Tot1 = 0, Tot2 = 0, Tot3 = 0;
	int runs = 0;


	__declspec(align(32))float A[VECTOR_SIZE];
	__declspec(align(32))float B[VECTOR_SIZE];
	__declspec(align(32))float C1[VECTOR_SIZE];
	__declspec(align(32))float C2[VECTOR_SIZE];
	__declspec(align(32))float C3[VECTOR_SIZE];

	std::random_device rand;
	std::mt19937 e2(rand());

	start_fresh:

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

	start_same:

	std::chrono::high_resolution_clock::time_point L1S = std::chrono::high_resolution_clock::now();
	//Scalar
	for (int j = 0; j < LOOPS; j++)
	{
		for (int i = 0; i < VECTOR_SIZE; i++)
		{
			C1[i] = A[i] + A[i] * B[i];
		}
	}
	std::chrono::high_resolution_clock::time_point L1E = std::chrono::high_resolution_clock::now();

	//Vector AVX2
	__m256 ymmA, ymmB, ymmC;

	std::chrono::high_resolution_clock::time_point L2S = std::chrono::high_resolution_clock::now();
	for (int j = 0; j < LOOPS; j++) {
		for (int i = 0; i < VECTOR_SIZE; i += 8)
		{
			ymmA = _mm256_load_ps(A + i);
			ymmB = _mm256_load_ps(B + i);
			ymmC = _mm256_mul_ps(ymmA, ymmB);
			ymmC = _mm256_add_ps(ymmA, ymmC);

			_mm256_store_ps(C2 + i, ymmC);
		}
	}
	std::chrono::high_resolution_clock::time_point L2E = std::chrono::high_resolution_clock::now();

	//Vector AVX2 + FMA
	std::chrono::high_resolution_clock::time_point L3S = std::chrono::high_resolution_clock::now();
	for (int j = 0; j < LOOPS; j++) {
		for (int i = 0; i < VECTOR_SIZE; i += 8)
		{
			ymmA = _mm256_load_ps(A + i);
			ymmB = _mm256_load_ps(B + i);
			ymmC = _mm256_fmadd_ps(ymmB, ymmA, ymmA);

			_mm256_store_ps(C3 + i, ymmC);
		}
	}
	std::chrono::high_resolution_clock::time_point L3E = std::chrono::high_resolution_clock::now();

	std::cout << std::setprecision(9) << std::fixed;
	std::cout << "All differences between outputs:" << std::endl;
	for (int i = 0; i < VECTOR_SIZE; i++)
	{
		if (C1[i] != C2[i] || C2[i] != C3[i] || C3[i] != C2[i])
		{
			std::cout << "C1[" << i << "]: " << C1[i] << "	C2[" << i << "]: " << C2[i] << "	C3[" << i << "]: " << C3[i] << std::endl;
		}
	}
	int Dur1, Dur2, Dur3;
	Dur1 = std::chrono::duration_cast<std::chrono::milliseconds>(L1E - L1S).count();
	Dur2 = std::chrono::duration_cast<std::chrono::milliseconds>(L2E - L2S).count();
	Dur3 = std::chrono::duration_cast<std::chrono::milliseconds>(L3E - L3S).count();
	Tot1 += Dur1;
	Tot2 += Dur2;
	Tot3 += Dur3;
	runs++;

	std::cout << "\n\n\n\nTotal execution time for " << LOOPS << " loops:" << std::endl;
	std::cout << "Normal " << Dur1 << " milliseconds" << std::endl;
	std::cout << "Vectoric (AVX2) " << Dur2 << " milliseconds" << std::endl;
	std::cout << "Vectoric (AVX2+FMA) " << Dur3 << " milliseconds\n\n\n\n" << std::endl;

	if (flag)
	{
		std::cout << "Total average execution time for " << LOOPS << " loops:" << std::endl;
		std::cout << "Normal " << Tot1/runs << " milliseconds" << std::endl;
		std::cout << "Vectoric (AVX2) " << Tot2/runs << " milliseconds" << std::endl;
		std::cout << "Vectoric (AVX2+FMA) " << Tot3/runs << " milliseconds\n\n\n\n" << std::endl;
	}

	std::cout << "To re-run with same arrays, select R" << std::endl;
	std::cout << "To re-run with new arrays, select N" << std::endl;
	std::cout << "To exit, just hit enter..." << std::endl;

	std::string input;
	std::getline(std::cin,input);

	if (input == "R") {
		flag = true;
		goto start_same;
	}
		
	if (input == "N") {
		flag = true;
		goto start_fresh;
	}
}