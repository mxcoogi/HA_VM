#include <stdio.h>
#include "stack_int8.h"
#include <stdlib.h>
//수정해야될 코드
STACK* stack_init(int cap) {
	// 초기화(동적 할당)
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
	// 실패하면 NULL 리턴
	return stack;
}

bool stack_push(STACK* stack, int8 value) {
	// value를 stack에 넣는다.
	if (stack->size >= stack->cap)
		return false; //꽉 찬상태면 false
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
	
	
	// stack에서 뽑은pop값을 result에 넣고 리턴
	// stack이 비어 있으면 false 리턴
	return true;
}


bool stack_empty(STACK* stack) {
	// stack이 비어 있으면 true, 아니면 false 리턴
	return stack->size == 0;

}

void stack_release(STACK* stack) {
	// 동적 할당 해제
	free(stack->slot);
	free(stack);
}

// 알아서 사용. 채점 대상 아님
int8 stack_test(void) {
	return 0;
}

