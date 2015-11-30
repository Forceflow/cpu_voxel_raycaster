#include "morton.h"

using namespace std;

uint64_t mortonEncode(uint64_t x, uint64_t y, uint64_t z){
	uint64_t answer = 0;
	for (uint64_t i = 0; i < (sizeof(uint64_t) * CHAR_BIT); ++i) {
		answer |= ((x & ((uint64_t)1 << i)) << 2*i) | ((y & ((uint64_t)1 << i)) << (2*i + 1)) | ((z & ((uint64_t)1 << i)) << (2*i + 2));
	}
	return answer;
}

vector<uint64_t> mortonDecode(uint64_t morton){
	vector<uint64_t> answer(3,(uint64_t) 0);

	for (uint64_t i = 0; i < (sizeof(uint64_t) * CHAR_BIT)/3; ++i) {
		answer[0] |= ((morton & (uint64_t( 1ull ) << uint64_t((3ull * i) + 0ull))) >> uint64_t(((3ull * i) + 0ull)-i));
		answer[1] |= ((morton & (uint64_t( 1ull ) << uint64_t((3ull * i) + 1ull))) >> uint64_t(((3ull * i) + 1ull)-i));
		answer[2] |= ((morton & (uint64_t( 1ull ) << uint64_t((3ull * i) + 2ull))) >> uint64_t(((3ull * i) + 2ull)-i));
	}
	return answer;
}
