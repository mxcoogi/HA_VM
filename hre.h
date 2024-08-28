#ifndef _HRE_H_
#define _HRE_H_
//수정해야될 코드
// HRE(HA-VM Runtime Environment)

#include <stdbool.h>
#include "stack_int8.h"
#include "hsm.h"

#define INSTR_LEN_MAX	100		// 명령어 한 줄(문자열)의 최대 길이
#define INSTR_CNT_MAX	1000	// 최대 명령어 개수
#define N_REGISTER		4
#define INT8_MIN		-128
#define INT8_MAX		127



#ifndef _STACK_INT8_H_
typedef char int8;
#endif 

// HSM 명령어 한 개
typedef struct {	
	char raw_code[INSTR_LEN_MAX];	
	// 문자열로 읽은 상태(처리되기 전)의 명령어
	// 추가로 필요한 멤버가 있으면 정의해서 사용

	//hsm코드로 번역한거
	int8 hsm_code[INSTR_LEN_MAX];
	 
} HSM_INSTR;

typedef struct {
	HSM_INSTR instr[INSTR_CNT_MAX];
	int size;	// 명령어 개수
	
	// 추가로 필요한 멤버가 있으면 정의해서 사용

} BYTECODE;





typedef struct {
	BYTECODE* code;
	STACK* stack;
	int8 registers[N_REGISTER];
	//기본적으로 따로 지정하지 않으면  A = 0 B = 1 C = 2 D = 3 
	int pc;	// program counter
	// 추가로 필요한 멤버가 있으면 정의해서 사용
	char register_alias[N_REGISTER][100];  // 레지스터 별명을 저장할 배열

} HRE;

// 함수 몇 개는 여기 있으면 안 되지만 채점의 편의를 위해서 HRE에 넣음

// hsm 파일의 텍스트를 읽어서 buf에 저장
// 오류가 생기면 -1, 아니면 읽은 byte수 리턴
int load(char* filename, char* buf);

// buf의 텍스트를 해석해서 code에 명령어 단위로 저장
// 오류가 생기면 false, 아니면 true 리턴
bool parse(char* buf, int bufsize, BYTECODE* code);

// HRE 초기화(동적 할당)
// 바이트코드, 스택, 레지스터, 프로그램 카운터를 초기화한다.
// 실패하면 NULL 리턴
// 스택에는 argc, argv로 넘겨받은 값을 argv[0], argv[1], ..., argc 순서로 푸시
HRE* hre_init(BYTECODE* code, int argc, int8* argv);

// bytecode 실행
// 마지막에는 반드시 스택에서 pop한 값을 리턴
// 이상이 있으면 -1을 리턴
char hre_execute(HRE* hre);

// HRE 및 HRE 내부의 동적 할당 메모리 해제
void hre_release(HRE* hre);

#endif
