#ifndef _STACK_INT8_H_
#define _STACK_INT8_H_

#include <stdbool.h>


typedef char int8;

// 8비트 정수들을 저장하는 스택
typedef struct {
	int8* slot;
	int size;	// 현재 크기
	int cap;	// 최대 크기(capacity)
} STACK;

STACK* stack_init(int cap);
bool stack_push(STACK* stack, int8 value);
bool stack_pop(STACK* stack, int8* result);
bool stack_empty(STACK* stack);
void stack_release(STACK* stack);
int8 stack_test(void);
#endif
