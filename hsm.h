#ifndef _HSM_H_
#define _HSM_H_

// HSM(HA-VM aSseMbly) instructions

// 특수 명령어
#define HSM_ALIAS	10
#define HSM_SEP		11	// '!'

// 스택, 리턴
#define	HSM_PUSH	00
#define HSM_POP		01
#define HSM_RET		02

// 산술 연산
#define HSM_ADD		20
#define HSM_SUB		21
#define HSM_NEG		22
#define HSM_ACC		23

// 비교해서 점프
#define HSM_JE		30		// jump if equal (==)
#define HSM_JNE		31		// jump if not equal (!=)
#define HSM_JL		32		// jump if less than (<)
#define HSM_JLE		33		// jump if less than or equal (<=)
#define HSM_JG		34		// jump if greater than (>)
#define HSM_JGE		35		// jump if grater than or equal (>=)

#endif
