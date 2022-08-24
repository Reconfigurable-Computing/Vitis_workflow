#include "vadd.hpp"



extern "C"
void vadd_kernel(dt *in1, dt *in2, dt *res){
#pragma HLS TOP name=vadd_kernel

#pragma HLS INTERFACE m_axi depth=1000 port=in1 offset=slave bundle=inBUS1
#pragma HLS INTERFACE m_axi depth=1000 port=in2 offset=slave bundle=inBUS2
#pragma HLS INTERFACE m_axi depth=1000 port=res offset=slave bundle=resBUS


	for(int i=0; i< N; i++){
		*(res+i) = *(in1+i)+*(in2+i);
	}

}
