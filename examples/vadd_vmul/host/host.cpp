#include <iostream>
#include "global.h"
#include "utils.hpp"
#include "vadd_vmul_wrapper.h"
using namespace std;

int main(int argc, const char* argv[]) {
    std::cout << "\n-------------------START----------------\n";
    ArgParser parser(argc, argv);
    std::string tmpStr;
    if (!parser.getCmdOption("--xclbin", tmpStr)) {
        std::cout << "ERROR: xclbin is not set!\n";
        return 1;
    }
    vec_t in1(N);
    vec_t in2(N);
    vec_t res1(N);
    vec_t res2(N);
    vec_t golden_res1(N);
    vec_t golden_res2(N);

    std::cout << "data prepared" << std::endl;
    for (int i = 0; i < N; i++) {
        in1[i] = i;
        in2[i] = i;
        golden_res1[i] = in1[i] + in2[i];
        golden_res2[i] = in1[i] * in2[i];
    }

    vadd_vmul_wrapper(in1, in2, res1, res2, N, tmpStr);

    unsigned err = 0;
    for (int i = 0; i < N; i++) {
        if (res1[i] != golden_res1[i] || res2[i] != golden_res2[i]) {
            err++;
            std::cout << "result1: " << res1[i] << ":" << golden_res1[i];
            std::cout << "   result2: " << res2[i] << ":" << golden_res2[i] << std::endl;
        }
    }

    if (err == 0) {
        std::cout << "TEST PASS" << std::endl;
    } else {
        std::cout << "TEST FAIL" << std::endl;
    }
    return err;
}