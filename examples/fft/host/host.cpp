#include <math.h>
#include <iostream>
#include "fft_wrapper.hpp"
#include "utils.hpp"
using namespace std;

struct Rmse {
    int num_sq;
    float sum_sq;
    float error;

    Rmse() {
        num_sq = 0;
        sum_sq = 0;
        error = 0;
    }

    float add_value(float d_n) {
        num_sq++;
        sum_sq += (d_n * d_n);
        error = sqrtf(sum_sq / num_sq);
        return error;
    }
};

Rmse rmse_R, rmse_I;

// Data must be aligned
vec_t In_R(SIZE), In_I(SIZE);
vec_t Out_R(SIZE), Out_I(SIZE);

int main(int argc, const char* argv[]) {
    std::cout << "\n-------------------START----------------\n";
    ArgParser parser(argc, argv);
    std::string tmpStr;
    if (!parser.getCmdOption("--xclbin", tmpStr)) {
        std::cout << "ERROR: xclbin is not set!\n";
        return 1;
    }

    int index;
    float gold_R, gold_I;

    FILE* fp = fopen("kernel/out.gold.dat", "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: cannot open the golden output file\n");
        return 1;
    }

    // Generate input data
    for (int i = 0; i < SIZE; i++) {
        In_R[i] = i;
        In_I[i] = 0.0;
        Out_R[i] = i;
        Out_I[i] = 1;
    }

    // Perform FFT
    fft_wrapper(In_R, In_I, Out_R, Out_I, tmpStr);

    // comparing with golden output
    for (int i = 0; i < SIZE; i++) {
        fscanf(fp, "%d %f %f", &index, &gold_R, &gold_I);
        // printf("%f %f\n", Out_R[i], gold_R);
        rmse_R.add_value(Out_R[i] - gold_R);
        rmse_I.add_value(Out_I[i] - gold_I);
    }
    fclose(fp);

    // printing error results
    printf("----------------------------------------------\n");
    printf("   RMSE(R)           RMSE(I)\n");
    printf("%0.15f %0.15f\n", rmse_R.error, rmse_I.error);
    printf("----------------------------------------------\n");

    if (rmse_R.error > 1 || rmse_I.error > 1) {
        fprintf(stdout, "*******************************************\n");
        fprintf(stdout, "FAIL: Output DOES NOT match the golden output\n");
        fprintf(stdout, "*******************************************\n");
        return 1;
    } else {
        fprintf(stdout, "*******************************************\n");
        fprintf(stdout, "PASS: The output matches the golden output!\n");
        fprintf(stdout, "*******************************************\n");
        return 0;
    }
}
