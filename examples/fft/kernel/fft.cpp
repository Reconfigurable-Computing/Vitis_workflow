

#include "fft.h"
#include<stdio.h>
#include"ap_int.h"
#include<string.h>
#define KNUM 8
const unsigned short loca[] = {
0,512,256,768,128,640,384,896,64,576,320,832,192,704,448,960,32,544,288,800,160,672,416,928,96,608,352,864,224,736,480,992,16,528,272,784,144,656,400,912,80,592,336,848,208,720,464,976,48,560,304,816,176,688,432,944,112,624,368,880,240,752,496,1008,8,520,264,776,136,648,392,904,72,584,328,840,200,712,456,968,40,552,296,808,168,680,424,936,104,616,360,872,232,744,488,1000,24,536,280,792,152,664,408,920,88,600,344,856,216,728,472,984,56,568,312,824,184,696,440,952,120,632,376,888,248,760,504,1016,4,516,260,772,132,644,388,900,68,580,324,836,196,708,452,964,36,548,292,804,164,676,420,932,100,612,356,868,228,740,484,996,20,532,276,788,148,660,404,916,84,596,340,852,212,724,468,980,52,564,308,820,180,692,436,948,116,628,372,884,244,756,500,1012,12,524,268,780,140,652,396,908,76,588,332,844,204,716,460,972,44,556,300,812,172,684,428,940,108,620,364,876,236,748,492,1004,28,540,284,796,156,668,412,924,92,604,348,860,220,732,476,988,60,572,316,828,188,700,444,956,124,636,380,892,252,764,508,1020,2,514,258,770,130,642,386,898,66,578,322,834,194,706,450,962,34,546,290,802,162,674,418,930,98,610,354,866,226,738,482,994,18,530,274,786,146,658,402,914,82,594,338,850,210,722,466,978,50,562,306,818,178,690,434,946,114,626,370,882,242,754,498,1010,10,522,266,778,138,650,394,906,74,586,330,842,202,714,458,970,42,554,298,810,170,682,426,938,106,618,362,874,234,746,490,1002,26,538,282,794,154,666,410,922,90,602,346,858,218,730,474,986,58,570,314,826,186,698,442,954,122,634,378,890,250,762,506,1018,6,518,262,774,134,646,390,902,70,582,326,838,198,710,454,966,38,550,294,806,166,678,422,934,102,614,358,870,230,742,486,998,22,534,278,790,150,662,406,918,86,598,342,854,214,726,470,982,54,566,310,822,182,694,438,950,118,630,374,886,246,758,502,1014,14,526,270,782,142,654,398,910,78,590,334,846,206,718,462,974,46,558,302,814,174,686,430,942,110,622,366,878,238,750,494,1006,30,542,286,798,158,670,414,926,94,606,350,862,222,734,478,990,62,574,318,830,190,702,446,958,126,638,382,894,254,766,510,1022,1,513,257,769,129,641,385,897,65,577,321,833,193,705,449,961,33,545,289,801,161,673,417,929,97,609,353,865,225,737,481,993,17,529,273,785,145,657,401,913,81,593,337,849,209,721,465,977,49,561,305,817,177,689,433,945,113,625,369,881,241,753,497,1009,9,521,265,777,137,649,393,905,73,585,329,841,201,713,457,969,41,553,297,809,169,681,425,937,105,617,361,873,233,745,489,1001,25,537,281,793,153,665,409,921,89,601,345,857,217,729,473,985,57,569,313,825,185,697,441,953,121,633,377,889,249,761,505,1017,5,517,261,773,133,645,389,901,69,581,325,837,197,709,453,965,37,549,293,805,165,677,421,933,101,613,357,869,229,741,485,997,21,533,277,789,149,661,405,917,85,597,341,853,213,725,469,981,53,565,309,821,181,693,437,949,117,629,373,885,245,757,501,1013,13,525,269,781,141,653,397,909,77,589,333,845,205,717,461,973,45,557,301,813,173,685,429,941,109,621,365,877,237,749,493,1005,29,541,285,797,157,669,413,925,93,605,349,861,221,733,477,989,61,573,317,829,189,701,445,957,125,637,381,893,253,765,509,1021,3,515,259,771,131,643,387,899,67,579,323,835,195,707,451,963,35,547,291,803,163,675,419,931,99,611,355,867,227,739,483,995,19,531,275,787,147,659,403,915,83,595,339,851,211,723,467,979,51,563,307,819,179,691,435,947,115,627,371,883,243,755,499,1011,11,523,267,779,139,651,395,907,75,587,331,843,203,715,459,971,43,555,299,811,171,683,427,939,107,619,363,875,235,747,491,1003,27,539,283,795,155,667,411,923,91,603,347,859,219,731,475,987,59,571,315,827,187,699,443,955,123,635,379,891,251,763,507,1019,7,519,263,775,135,647,391,903,71,583,327,839,199,711,455,967,39,551,295,807,167,679,423,935,103,615,359,871,231,743,487,999,23,535,279,791,151,663,407,919,87,599,343,855,215,727,471,983,55,567,311,823,183,695,439,951,119,631,375,887,247,759,503,1015,15,527,271,783,143,655,399,911,79,591,335,847,207,719,463,975,47,559,303,815,175,687,431,943,111,623,367,879,239,751,495,1007,31,543,287,799,159,671,415,927,95,607,351,863,223,735,479,991,63,575,319,831,191,703,447,959,127,639,383,895,255,767,511,1023
};
typedef union{
	unsigned ival;
	float oval;
} converter;



DTYPE fadd(DTYPE a, DTYPE b){
#pragma HLS INLINE off
#pragma HLS LATENCY max=3 min=3

	DTYPE res = a+b;
	return res;
}

DTYPE fsub(DTYPE a, DTYPE b){
#pragma HLS INLINE off
#pragma HLS LATENCY max=3 min=3


	DTYPE res = a-b;
	return res;
}

DTYPE fmult(DTYPE a, DTYPE b){
#pragma HLS INLINE off
#pragma HLS LATENCY max=6 min=6

	DTYPE res = a*b;
	return res;
}


//stages
template<unsigned short l,unsigned short m,unsigned short loop_cnt>
void fft_stages_First(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]) {

unsigned short k =0;
    fft1:
    for(k; k < loop_cnt; k++ ){
        fft2:
        for( unsigned short i =0; i < m; i++){
#pragma HLS DEPENDENCE false inter variable=OUT_R
#pragma HLS DEPENDENCE false inter variable=OUT_I
#pragma HLS LOOP_FLATTEN
#pragma HLS PIPELINE II=1

            DTYPE t_r,t_i, scale_r, scale_i;
            unsigned short loc  = loca[( loop_cnt ) * i];
            unsigned short loc2 = k*l+i;
            unsigned short index = k*l+i+m;

            DTYPE tmp1,tmp2,tmp4,tmp5;

            scale_r =  W_real[loc];
            scale_i =  W_imag[loc];
            //
            tmp1 = fmult( X_R[ loca[index] ], scale_r);
            tmp2 = fmult( X_I[ loca[index] ], scale_i);
            t_r  = fsub(tmp1,tmp2);

            tmp4 = fmult( X_R[ loca[index] ], scale_i);
            tmp5 = fmult( X_I[ loca[index] ], scale_r);
            t_i  = fadd(tmp4,tmp5);

            OUT_R[ index ]  =  fsub(X_R[ loca[loc2] ] , t_r);
            OUT_I[ index ]  =  fsub(X_I[ loca[loc2] ] , t_i);

            OUT_R[ loc2 ]   =  fadd(X_R[ loca[loc2] ] , t_r);
            OUT_I[ loc2 ]   =  fadd( X_I[ loca[loc2] ] , t_i);

//          t_r = X_R[ loca[index] ] *  scale_r -  X_I[ loca[index] ] *  scale_i;
//          t_i = X_R[ loca[index] ] *  scale_i +  X_I[ loca[index]]  *  scale_r;
//
//          OUT_R[ index ]  =  X_R[ loca[loc2] ] - t_r;
//          OUT_I[ index ]  =  X_I[ loca[loc2] ] - t_i;
//
//          OUT_R[ loc2 ]     =   X_R[ loca[loc2] ] + t_r;
//          OUT_I[ loc2 ]     =   X_I[ loca[loc2] ] + t_i;

        }
    }
}

//stages
template<unsigned short l,unsigned short m,unsigned short loop_cnt>
void fft_stages(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]) {

    fft1:
    for(unsigned short k=0; k < loop_cnt; k++ ){
        fft2:
        for( unsigned short i =0; i < m; i++){
#pragma HLS DEPENDENCE false inter variable=OUT_R
#pragma HLS DEPENDENCE false inter variable=OUT_I
#pragma HLS LOOP_FLATTEN
#pragma HLS PIPELINE II=1

            DTYPE t_r,t_i, scale_r, scale_i;
            unsigned short loc  = ( loop_cnt ) * i;
            unsigned short loc2 = k*l + i;
            DTYPE tmp1,tmp2,tmp4,tmp5;

            scale_r =  W_real[loc];
            scale_i =  W_imag[loc];
            tmp1 = fmult( X_R[ loc2 + m ], scale_r);
            tmp2 = fmult( X_I[ loc2 + m ], scale_i);
            t_r  = fsub(tmp1,tmp2);

            tmp4 = fmult( X_R[ loc2 + m ], scale_i);
            tmp5 = fmult( X_I[ loc2 + m ], scale_r);
            t_i  = fadd(tmp4,tmp5);

            OUT_R[ loc2 + m  ]  =  fsub(X_R[ loc2 ] , t_r);
            OUT_I[ loc2 + m  ]  =  fsub(X_I[ loc2 ] , t_i);

            OUT_R[ loc2 ]   =  fadd(X_R[ loc2 ] , t_r);
            OUT_I[ loc2 ]   =  fadd(X_I[ loc2 ] , t_i);
//
//            t_r = X_R[ loc2 + m] *  scale_r -  X_I[ loc2 + m ] *  scale_i;
//            t_i = X_R[ loc2 + m] *  scale_i +  X_I[ loc2 + m]  *  scale_r;
//
//            OUT_R[ loc2 + m ]  =  X_R[ loc2 ] - t_r;
//            OUT_I[ loc2 + m ]  =  X_I[ loc2 ] - t_i;
//
//            OUT_R[ loc2 ]     =   X_R[ loc2 ] + t_r;
//            OUT_I[ loc2 ]     =   X_I[ loc2 ] + t_i;

        }
    }
}


void get_in(DTYPE X_R[SIZE], DTYPE X_I[SIZE],DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]){
	GIN:
	for(unsigned short i =0;i< SIZE/KNUM;i++){
#pragma HLS PIPELINE II=1
		ap_uint<256> TMP_R,TMP_I;
		memcpy(&TMP_R,(ap_uint<256>*)X_R+i,KNUM*sizeof(DTYPE));
		memcpy(&TMP_I,(ap_uint<256>*)X_I+i,KNUM*sizeof(DTYPE));
		for(unsigned short k=0;k<KNUM;k++){
#pragma HLS UNRLL
			unsigned short index = i*KNUM+k;
			converter tmp1,tmp2;
			tmp1.ival = TMP_R.range(32*(k+1)-1,32*k);
			tmp2.ival = TMP_I.range(32*(k+1)-1,32*k);
			OUT_R[index] = tmp1.oval;
			OUT_I[index] = tmp2.oval;
		}
	}
}





void get_out(DTYPE X_R[SIZE], DTYPE X_I[SIZE],DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]){
	GOU:
	for(unsigned short i =0;i< SIZE/KNUM;i++){
#pragma HLS PIPELINE II=1
		ap_uint<256> TMP_R,TMP_I;
		for(unsigned short k=0;k<KNUM;k++){
#pragma HLS UNROLL
			converter tmp1,tmp2;
			tmp1.oval =  X_R[i*KNUM+k];
			tmp2.oval =  X_I[i*KNUM+k];
			TMP_R.range(32*(k+1)-1,32*k) = tmp1.ival;
			TMP_I.range(32*(k+1)-1,32*k) = tmp2.ival;
		}
		memcpy((ap_uint<256>*)OUT_R+i,&TMP_R,KNUM*sizeof(DTYPE));
		memcpy((ap_uint<256>*)OUT_I+i,&TMP_I,KNUM*sizeof(DTYPE));
	}
}

void fft_kernel(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]){
#pragma HLS INLINE off
#pragma HLS DATAFLOW
	// the first and last stage may be processed specifically, due to the dynamically memory access.
	//Call fft
	DTYPE tmp_R[SIZE], tmp_I[SIZE];

	DTYPE Stage1_R[SIZE], Stage1_I[SIZE];
	DTYPE Stage2_R[SIZE], Stage2_I[SIZE];
	DTYPE Stage3_R[SIZE], Stage3_I[SIZE];
	DTYPE Stage4_R[SIZE], Stage4_I[SIZE];
	DTYPE Stage5_R[SIZE], Stage5_I[SIZE];
	DTYPE Stage6_R[SIZE], Stage6_I[SIZE];
	DTYPE Stage7_R[SIZE], Stage7_I[SIZE];
	DTYPE Stage8_R[SIZE], Stage8_I[SIZE];
	DTYPE Stage9_R[SIZE], Stage9_I[SIZE];
	DTYPE tmp_R2[SIZE], tmp_I2[SIZE];

#pragma HLS ARRAY_PARTITION variable=tmp_R dim=1 factor=8 cyclic
#pragma HLS ARRAY_PARTITION variable=tmp_I dim=1 factor=8 cyclic


#pragma HLS ARRAY_PARTITION variable=tmp_R2 dim=1 factor=8 cyclic
#pragma HLS ARRAY_PARTITION variable=tmp_I2 dim=1 factor=8 cyclic
	get_in(X_R,X_I,tmp_R,tmp_I);
	// l,m,loo_cnt
    fft_stages_First<2,1,512>(tmp_R,tmp_I,  Stage1_R, Stage1_I);
	fft_stages<4,2,256>(Stage1_R, Stage1_I,  Stage2_R, Stage2_I);
	fft_stages<8,4,128>(Stage2_R, Stage2_I,  Stage3_R, Stage3_I);
	fft_stages<16,8,64>(Stage3_R, Stage3_I,  Stage4_R, Stage4_I);
	fft_stages<32,16,32>(Stage4_R, Stage4_I,  Stage5_R, Stage5_I);
	fft_stages<64,32,16>(Stage5_R, Stage5_I,  Stage6_R, Stage6_I);
	fft_stages<128,64,8>(Stage6_R, Stage6_I,  Stage7_R, Stage7_I);
	fft_stages<256,128,4>(Stage7_R, Stage7_I,  Stage8_R, Stage8_I);
	fft_stages<512,256,2>(Stage8_R, Stage8_I,  Stage9_R, Stage9_I);
	//fft_stage_last(Stage9_R, Stage9_I, OUT_R, OUT_I);
    fft_stages<1024,512,1>(Stage9_R, Stage9_I, tmp_R2, tmp_I2);
    get_out(tmp_R2, tmp_I2,OUT_R, OUT_I);
}

extern "C" void fft(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE])
{
#pragma HLS INTERFACE s_axilite port=return

#pragma HLS INTERFACE m_axi max_widen_bitwidth=256 max_write_burst_length=16 latency=16 bundle=RE_IOUT port=OUT_I
#pragma HLS INTERFACE m_axi max_widen_bitwidth=256 max_write_burst_length=16 latency=16 bundle=RE_ROUT port=OUT_R
#pragma HLS INTERFACE m_axi max_widen_bitwidth=256 max_read_burst_length=16 latency=16 bundle=DATA_RIN port=X_R
#pragma HLS INTERFACE m_axi max_read_burst_length=16 max_widen_bitwidth=256 latency=16 bundle=DATA_IIN port=X_I


	// for(int i=0 ; i < 10000;i++){
	// 	fft_kernel(X_R,X_I,OUT_R,OUT_I);
	// }

        fft_kernel(X_R, X_I, OUT_R, OUT_I);
}





