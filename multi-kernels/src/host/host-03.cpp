#include "host.hpp"



int main(int argc, const char* argv[]) {
    std::cout << "\n-------------------START----------------\n";
    ArgParser parser(argc, argv);
    std::string tmpStr;
    if (!parser.getCmdOption("--mode", tmpStr)) {
        std::cout << "ERROR: mode is not set!\n";
        return 1;
    }

    int mode;
    if(tmpStr == "0"){
    	mode = 0;
        std::cout << "INFO: VADD is selected\n";

    }
    else if(tmpStr == "1"){
    	mode = 1;
        std::cout << "INFO: VMUL is selected\n";
    }
    else{
    	std::cout << "ERROR: mode error\n";
    }


    dt in1[N];
    dt in2[N];
    dt res[N];
    dt res_golden[N];

    std::cout << "data prepared" << std::endl;
    for (int i = 0; i < N; i++) {
        in1[i] = i;
        in2[i] = i;
        if(mode == 0){
        	res_golden[i] = i+i;
//        	std::cout << "Here is add\n";
        }
        else
            if(mode == 1){
            	res_golden[i] = i*i;
//            	std::cout << "Here is mul\n";
            }

    }

    struct timeval start_time, end_time;
    gettimeofday(&end_time, 0);
    switch (mode)
    {
    case 0:
        vadd_op(in1,in2,res,N);
        break;
    case 1:
        vmul_op(in1,in2,res,N);
        break;
    }
    gettimeofday(&end_time, 0);
//    std::cout << "Kernel ed execution time is: " <<end_time.tv_sec<<"."<< end_time.tv_usec << " s" << std::endl;

    unsigned err = 0;
    for (int i = 0; i < N; i++) {
        if(res[i]!=res_golden[i]){
        	err++;
        	std::cout << "result: " <<res[i]<<":"<<res_golden[i]<< std::endl;
        }
     }


    if (err == 0) {
        std::cout << "TEST PASS" << std::endl;
    } else {
        std::cout << "TEST FAIL" << std::endl;
    }
    return err;
}
