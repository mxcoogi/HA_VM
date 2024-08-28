#ifndef _HRE_H_
#define _HRE_H_
//�����ؾߵ� �ڵ�
// HRE(HA-VM Runtime Environment)

#include <stdbool.h>
#include "stack_int8.h"
#include "hsm.h"

#define INSTR_LEN_MAX	100		// ��ɾ� �� ��(���ڿ�)�� �ִ� ����
#define INSTR_CNT_MAX	1000	// �ִ� ��ɾ� ����
#define N_REGISTER		4
#define INT8_MIN		-128
#define INT8_MAX		127



#ifndef _STACK_INT8_H_
typedef char int8;
#endif 

// HSM ��ɾ� �� ��
typedef struct {	
	char raw_code[INSTR_LEN_MAX];	
	// ���ڿ��� ���� ����(ó���Ǳ� ��)�� ��ɾ�
	// �߰��� �ʿ��� ����� ������ �����ؼ� ���

	//hsm�ڵ�� �����Ѱ�
	int8 hsm_code[INSTR_LEN_MAX];
	 
} HSM_INSTR;

typedef struct {
	HSM_INSTR instr[INSTR_CNT_MAX];
	int size;	// ��ɾ� ����
	
	// �߰��� �ʿ��� ����� ������ �����ؼ� ���

} BYTECODE;





typedef struct {
	BYTECODE* code;
	STACK* stack;
	int8 registers[N_REGISTER];
	//�⺻������ ���� �������� ������  A = 0 B = 1 C = 2 D = 3 
	int pc;	// program counter
	// �߰��� �ʿ��� ����� ������ �����ؼ� ���
	char register_alias[N_REGISTER][100];  // �������� ������ ������ �迭

} HRE;

// �Լ� �� ���� ���� ������ �� ������ ä���� ���Ǹ� ���ؼ� HRE�� ����

// hsm ������ �ؽ�Ʈ�� �о buf�� ����
// ������ ����� -1, �ƴϸ� ���� byte�� ����
int load(char* filename, char* buf);

// buf�� �ؽ�Ʈ�� �ؼ��ؼ� code�� ��ɾ� ������ ����
// ������ ����� false, �ƴϸ� true ����
bool parse(char* buf, int bufsize, BYTECODE* code);

// HRE �ʱ�ȭ(���� �Ҵ�)
// ����Ʈ�ڵ�, ����, ��������, ���α׷� ī���͸� �ʱ�ȭ�Ѵ�.
// �����ϸ� NULL ����
// ���ÿ��� argc, argv�� �Ѱܹ��� ���� argv[0], argv[1], ..., argc ������ Ǫ��
HRE* hre_init(BYTECODE* code, int argc, int8* argv);

// bytecode ����
// ���������� �ݵ�� ���ÿ��� pop�� ���� ����
// �̻��� ������ -1�� ����
char hre_execute(HRE* hre);

// HRE �� HRE ������ ���� �Ҵ� �޸� ����
void hre_release(HRE* hre);

#endif
