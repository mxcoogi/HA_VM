#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hre.h"

#define BUF_SIZE		10000	// raw data�� ���� ���� ũ��. �˳��ϰ�.
#define ARGC_MAX		100		// �ִ� ���� ����. �˳��ϰ�.
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
		error("�� �����̰ų� ������ �� �� �����ϴ�.");


	if (! parse(buf, size, &code))
		error("��ɾ� �ؼ��� �����߽��ϴ�.");

	

	// argv[1]: �����̸�(xxxx.hsm)
	// argv[2...(argc-1)]: application�� ������ ���ڵ�
	int8 arg_int8[ARGC_MAX];
	for (int i = 2; i < argc; i++) {
		arg_int8[i - 2] = (int8)atoi(argv[i]);
	}

	HRE* hre = hre_init(&code, argc - 2, arg_int8);
	if (hre == NULL)   
		error("HRE �ʱ�ȭ�� �����߽��ϴ�.");
	
	
	for (int i = 1; i < hre->code->size; i++) {
		printf(" %d ��°�� ��ɾ� %s\n", i, hre->code->instr[i].hsm_code);
	}


	int8 result = hre_execute(hre);
	hre_release(hre);
	printf("%d\n", result);
	
	return 0;
}
