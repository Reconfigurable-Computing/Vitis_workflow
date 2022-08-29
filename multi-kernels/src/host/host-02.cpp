#include "host-02.hpp"




int main(int argc, const char* argv[]) {
    std::cout << "\n-------------------START----------------\n";
    ArgParser parser(argc, argv);
    std::string tmpStr;
    if (!parser.getCmdOption("--xclbin", tmpStr)) {
        std::cout << "ERROR: xclbin is not set!\n";
        return 1;
    }
    std::string xclbin_path = tmpStr;

    dt in1[N];
    dt in2[N];
    dt res1[N];
    dt res2[N];
    dt res_golden1[N];
    dt res_golden2[N];

    std::cout << "data prepared" << std::endl;
    for (int i = 0; i < N; i++) {
        in1[i] = i;
        in2[i] = i;

        res_golden1[i] = i+i;
        res_golden2[i] = i*i;
//        	std::cout << "Here is add\n";

    }

    struct timeval start_time, end_time;
    gettimeofday(&end_time, 0);

    op(in1,in2,res1,res2,N,xclbin_path);
    gettimeofday(&end_time, 0);
//    std::cout << "Kernel ed execution time is: " <<end_time.tv_sec<<"."<< end_time.tv_usec << " s" << std::endl;

    unsigned err = 0;
    for (int i = 0; i < N; i++) {
        if(res1[i]!=res_golden1[i]||res2[i]!=res_golden2[i]){
        	err++;
        	std::cout << "result1: " <<res1[i]<<":"<<res_golden1[i];
        	std::cout << "   result2: " <<res2[i]<<":"<<res_golden2[i]<< std::endl;
        }
     }


    if (err == 0) {
        std::cout << "TEST PASS" << std::endl;
    } else {
        std::cout << "TEST FAIL" << std::endl;
    }
    return err;
}
