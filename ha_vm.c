#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hre.h"

#define BUF_SIZE		10000	// raw data를 읽을 버퍼 크기. 넉넉하게.
#define ARGC_MAX		100		// 최대 인자 개수. 넉넉하게.
char buf[BUF_SIZE];
BYTECODE code;

void error(char* msg);

void error(char* msg) {
	printf("ERROR: %s\n", msg);
	exit(-1);
}


int main(int argc, char* argv[]) {
	stack_test();

	int size = load(argv[1], buf);
	if (size <= 0)
		error("빈 파일이거나 파일을 열 수 없습니다.");


	if (! parse(buf, size, &code))
		error("명령어 해석에 실패했습니다.");

	

	// argv[1]: 파일이름(xxxx.hsm)
	// argv[2...(argc-1)]: application에 전달할 인자들
	int8 arg_int8[ARGC_MAX];
	for (int i = 2; i < argc; i++) {
		arg_int8[i - 2] = (int8)atoi(argv[i]);
	}

	HRE* hre = hre_init(&code, argc - 2, arg_int8);
	if (hre == NULL)   
		error("HRE 초기화를 실패했습니다.");
	
	
	for (int i = 1; i < hre->code->size; i++) {
		printf(" %d 번째행 명령어 %s\n", i, hre->code->instr[i].hsm_code);
	}


	int8 result = hre_execute(hre);
	hre_release(hre);
	printf("%d\n", result);
	
	return 0;
}
