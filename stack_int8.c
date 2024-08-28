#include <stdio.h>
#include "stack_int8.h"
#include <stdlib.h>
//�����ؾߵ� �ڵ�
STACK* stack_init(int cap) {
	// �ʱ�ȭ(���� �Ҵ�)
	STACK* stack = (STACK*)malloc(sizeof(STACK));
	if (stack == NULL) {
			return NULL;
	}
	

	stack->slot = (int8*)malloc(cap * sizeof(int8));

	if (stack->slot == NULL) {
		free(stack);
		return NULL;
	}

	stack -> size = 0;
	stack -> cap = cap;
	// �����ϸ� NULL ����
	return stack;
}

bool stack_push(STACK* stack, int8 value) {
	// value�� stack�� �ִ´�.
	if (stack->size >= stack->cap)
		return false; //�� �����¸� false
	stack->slot[stack->size] = value;
	stack->size++;
	return true;
}

bool stack_pop(STACK* stack, int8* result) {
	if (stack->size <= 0)
		return false;
	int8 res = stack->slot[stack->size - 1];
	*result = res;
	stack->size--;
	
	
	// stack���� ����pop���� result�� �ְ� ����
	// stack�� ��� ������ false ����
	return true;
}


bool stack_empty(STACK* stack) {
	// stack�� ��� ������ true, �ƴϸ� false ����
	return stack->size == 0;

}

void stack_release(STACK* stack) {
	// ���� �Ҵ� ����
	free(stack->slot);
	free(stack);
}

// �˾Ƽ� ���. ä�� ��� �ƴ�
int8 stack_test(void) {
	return 0;
}

