#ifndef _STACK_INT8_H_
#define _STACK_INT8_H_

#include <stdbool.h>


typedef char int8;

// 8��Ʈ �������� �����ϴ� ����
typedef struct {
	int8* slot;
	int size;	// ���� ũ��
	int cap;	// �ִ� ũ��(capacity)
} STACK;

STACK* stack_init(int cap);
bool stack_push(STACK* stack, int8 value);
bool stack_pop(STACK* stack, int8* result);
bool stack_empty(STACK* stack);
void stack_release(STACK* stack);
int8 stack_test(void);
#endif
