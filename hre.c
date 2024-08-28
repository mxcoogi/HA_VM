#include <stdio.h>
#include "hre.h"
#include <string.h>
#include <stdlib.h>

/*for (int j = 0; j < tokenCount; j++) {
                memset(tokens[j], 0, sizeof(tokens[j]));
            }
            tokenCount = 0;
            hre->pc++;
*/  //토큰초기화하는 코드

/*
* 레지스터 별명찾는 코드
const char* variableName = tokens[1];
            int registerIndex = -1;
            for (int i = 0; i < N_REGISTER; i++) {
                if (strcmp(hre->register_alias[i], variableName) == 0) {
                    registerIndex = i;
                    break;
                }
            }
*/

bool check_overflow(int value) {
    if (value < INT8_MIN || value > INT8_MAX) {
        return false;
    }
} //범위에알맞는 value인지 체크!


//수정해야할 코드
//load 완료
int load(char* filename, char* buf) {
    bool chk = false;  //일단 chk false로 해놓고  파일 읽기 계속 읽다가 
    FILE* file;
    if (fopen_s(&file, filename, "r") != 0) {
        return -1; // 파일 열기 실패 시 -1 반환
    }

    int byteCount = 0; // 읽은 바이트 수를 저장할 변수

    while (!feof(file)) {
        int ch = fgetc(file); // 파일에서 문자 하나씩 읽어옵니다.

        if (ch == EOF) {
            break; // 파일의 끝에 도달하면 반복문 종료
        }
        if ((char)ch == '!') {
            chk = true;
        }
        if (chk) {
            *buf++ = (char)ch; // 읽은 문자를 버퍼에 저장하고, 버퍼 포인터를 다음 위치로 이동합니다.
            byteCount++; // 바이트 수 증가
        }
    }

    fclose(file); // 파일을 닫습니다.
    return byteCount; // 읽은 바이트 수를 반환합니다.
}

// buf의 텍스트를 해석해서 code에 명령어 단위로 저장
// 오류가 생기면 false, 아니면 true 리턴 완료
bool parse(char* buf, int bufsize, BYTECODE* code) {
    code->size = 0;
    char temp_str[INSTR_LEN_MAX] = "";  // 빈 문자열로 초기화
    
    for (int i = 0; i < bufsize; i++) {

        if (buf[i] == '\n') {
            if (strcpy_s(code->instr[code->size].raw_code, INSTR_LEN_MAX, temp_str) != 0) {
                return false;  // 복사 실패 시 false 반환
            }
            code->size++;
            temp_str[0] = '\0';  // 빈 문자열로 초기화
        }
        else {
            strncat_s(temp_str, INSTR_LEN_MAX, &buf[i], 1);  // 문자 하나씩 덧붙임
        }
    }
    if (strcpy_s(code->instr[code->size].raw_code, INSTR_LEN_MAX, temp_str) != 0) {
        return false;  // 복사 실패 시 false 반환
    }
    code->size++;
    temp_str[0] = '\0';


    
    char tokens[100][100];
    int tokenCount = 0;
    for (int i = 0; i < code->size; i++) {
        // 코드를 한 줄씩 가져와서 공백 기준으로 분해 후 해석
        char* token;
        char* nextToken;
        token = strtok_s(code->instr[i].raw_code, " ", &nextToken);
        while (token != NULL && tokenCount < 100) {
            strcpy_s(tokens[tokenCount], sizeof(tokens[tokenCount]), token);
            tokenCount++;
            token = strtok_s(NULL, " ", &nextToken);
        }

        if (strcmp(tokens[0], "push") == 0) {
            char newCode[INSTR_LEN_MAX] = "";
            strcpy_s(newCode, sizeof(newCode), "HSM_PUSH");
            for (int j = 1; j < tokenCount; j++) {
                strcat_s(newCode, sizeof(newCode), " ");
                strcat_s(newCode, sizeof(newCode), tokens[j]);
            }
            strcpy_s(code->instr[i].hsm_code, INSTR_LEN_MAX, newCode);
        }
        else if (strcmp(tokens[0], "pop") == 0) {
            char newCode[INSTR_LEN_MAX] = "";
            strcpy_s(newCode, sizeof(newCode), "HSM_POP");
            for (int j = 1; j < tokenCount; j++) {
                strcat_s(newCode, sizeof(newCode), " ");
                strcat_s(newCode, sizeof(newCode), tokens[j]);
            }
            strcpy_s(code->instr[i].hsm_code, INSTR_LEN_MAX, newCode);
        }
        else if (strcmp(tokens[0], "ret") == 0) {
            char newCode[INSTR_LEN_MAX] = "";
            strcpy_s(newCode, sizeof(newCode), "HSM_RET");
            for (int j = 1; j < tokenCount; j++) {
                strcat_s(newCode, sizeof(newCode), " ");
                strcat_s(newCode, sizeof(newCode), tokens[j]);
            }
            strcpy_s(code->instr[i].hsm_code, INSTR_LEN_MAX, newCode);
        }
        else if (strcmp(tokens[0], "!") == 0) {
            char newCode[INSTR_LEN_MAX] = "";
            strcpy_s(newCode, sizeof(newCode), "HSM_SEP");
            for (int j = 1; j < tokenCount; j++) {
                strcat_s(newCode, sizeof(newCode), " ");
                strcat_s(newCode, sizeof(newCode), tokens[j]);
            }
            strcpy_s(code->instr[i].hsm_code, INSTR_LEN_MAX, newCode);
        }
        else if (strcmp(tokens[0], "alias") == 0) {
            char newCode[INSTR_LEN_MAX] = "";
            strcpy_s(newCode, sizeof(newCode), "HSM_ALIAS");
            for (int j = 1; j < tokenCount; j++) {
                strcat_s(newCode, sizeof(newCode), " ");
                strcat_s(newCode, sizeof(newCode), tokens[j]);
            }
            strcpy_s(code->instr[i].hsm_code, INSTR_LEN_MAX, newCode);
        }
        else if (strcmp(tokens[0], "add") == 0) {
            char newCode[INSTR_LEN_MAX] = "";
            strcpy_s(newCode, sizeof(newCode), "HSM_ADD");
            for (int j = 1; j < tokenCount; j++) {
                strcat_s(newCode, sizeof(newCode), " ");
                strcat_s(newCode, sizeof(newCode), tokens[j]);
            }
            strcpy_s(code->instr[i].hsm_code, INSTR_LEN_MAX, newCode);
        }
        else if (strcmp(tokens[0], "sub") == 0) {
            char newCode[INSTR_LEN_MAX] = "";
            strcpy_s(newCode, sizeof(newCode), "HSM_SUB");
            for (int j = 1; j < tokenCount; j++) {
                strcat_s(newCode, sizeof(newCode), " ");
                strcat_s(newCode, sizeof(newCode), tokens[j]);
            }
            strcpy_s(code->instr[i].hsm_code, INSTR_LEN_MAX, newCode);
        }
        else if (strcmp(tokens[0], "neg") == 0) {
            char newCode[INSTR_LEN_MAX] = "";
            strcpy_s(newCode, sizeof(newCode), "HSM_NEG");
            for (int j = 1; j < tokenCount; j++) {
                strcat_s(newCode, sizeof(newCode), " ");
                strcat_s(newCode, sizeof(newCode), tokens[j]);
            }
            strcpy_s(code->instr[i].hsm_code, INSTR_LEN_MAX, newCode);
        }
        else if (strcmp(tokens[0], "acc") == 0) {
            char newCode[INSTR_LEN_MAX] = "";
            strcpy_s(newCode, sizeof(newCode), "HSM_ACC");
            for (int j = 1; j < tokenCount; j++) {
                strcat_s(newCode, sizeof(newCode), " ");
                strcat_s(newCode, sizeof(newCode), tokens[j]);
            }
            strcpy_s(code->instr[i].hsm_code, INSTR_LEN_MAX, newCode);
        }
        else if (strcmp(tokens[0], "je") == 0) {
            char newCode[INSTR_LEN_MAX] = "";
            strcpy_s(newCode, sizeof(newCode), "HSM_JE");
            for (int j = 1; j < tokenCount; j++) {
                strcat_s(newCode, sizeof(newCode), " ");
                strcat_s(newCode, sizeof(newCode), tokens[j]);
            }
            strcpy_s(code->instr[i].hsm_code, INSTR_LEN_MAX, newCode);
        }
        else if (strcmp(tokens[0], "jne") == 0) {
            char newCode[INSTR_LEN_MAX] = "";
            strcpy_s(newCode, sizeof(newCode), "HSM_JNE");
            for (int j = 1; j < tokenCount; j++) {
                strcat_s(newCode, sizeof(newCode), " ");
                strcat_s(newCode, sizeof(newCode), tokens[j]);
            }
            strcpy_s(code->instr[i].hsm_code, INSTR_LEN_MAX, newCode);
        }
        else if (strcmp(tokens[0], "jl") == 0) {
            char newCode[INSTR_LEN_MAX] = "";
            strcpy_s(newCode, sizeof(newCode), "HSM_JL");
            for (int j = 1; j < tokenCount; j++) {
                strcat_s(newCode, sizeof(newCode), " ");
                strcat_s(newCode, sizeof(newCode), tokens[j]);
            }
            strcpy_s(code->instr[i].hsm_code, INSTR_LEN_MAX, newCode);
        }
        else if (strcmp(tokens[0], "jle") == 0) {
        char newCode[INSTR_LEN_MAX] = "";
        strcpy_s(newCode, sizeof(newCode), "HSM_JLE");
        for (int j = 1; j < tokenCount; j++) {
            strcat_s(newCode, sizeof(newCode), " ");
            strcat_s(newCode, sizeof(newCode), tokens[j]);
        }
        strcpy_s(code->instr[i].hsm_code, INSTR_LEN_MAX, newCode);
        }
        else if (strcmp(tokens[0], "jg") == 0) {
        char newCode[INSTR_LEN_MAX] = "";
        strcpy_s(newCode, sizeof(newCode), "HSM_JG");
        for (int j = 1; j < tokenCount; j++) {
            strcat_s(newCode, sizeof(newCode), " ");
            strcat_s(newCode, sizeof(newCode), tokens[j]);
        }
        strcpy_s(code->instr[i].hsm_code, INSTR_LEN_MAX, newCode);
        }
        else if (strcmp(tokens[0], "jge") == 0) {
        char newCode[INSTR_LEN_MAX] = "";
        strcpy_s(newCode, sizeof(newCode), "HSM_JGE");
        for (int j = 1; j < tokenCount; j++) {
            strcat_s(newCode, sizeof(newCode), " ");
            strcat_s(newCode, sizeof(newCode), tokens[j]);
        }
        strcpy_s(code->instr[i].hsm_code, INSTR_LEN_MAX, newCode);
        }
        else {
  
            for (int j = 0; j < tokenCount; j++) {
                memset(tokens[j], 0, sizeof(tokens[j]));
            }
            tokenCount = 0;
        }
        // tokens 배열 초기화
        for (int j = 0; j < tokenCount; j++) {
            memset(tokens[j], 0, sizeof(tokens[j]));
        }
        tokenCount = 0;
    }
    return true;
}



// HRE 초기화(동적 할당)
// 바이트코드, 스택, 레지스터, 프로그램 카운터를 초기화한다.
// 실패하면 NULL 리턴
// 스택에는 argc, argv로 넘겨받은 값을 argv[0], argv[1], ..., argc 순서로 푸시
HRE* hre_init(BYTECODE* code, int argc, int8* argv) {

    HRE* hre = (HRE*)malloc(sizeof(HRE));
    if (hre == NULL) {
        return NULL;
    }

    hre->code = code;
    hre->stack = stack_init(100);

    if (hre->stack == NULL) {
        free(hre);
        return NULL;
    }

    for (int i = 0; i < N_REGISTER; i++) {
        hre->registers[i] = 0;
        snprintf(hre->register_alias[i], sizeof(hre->register_alias[i]), "%c", 'A' + i);
    }
    
    hre->pc = 0;

    for (int i = 0; i < argc; i++) {

        stack_push(hre->stack, argv[i]);
    }
    stack_push(hre->stack, argc);




    return hre;


}

// bytecode 실행
// 마지막에는 반드시 스택에서 pop한 값을 리턴
// 이상이 있으면 -1을 리턴
char hre_execute(HRE* hre) {

    char tokens[100][100];
    int tokenCount = 0;
    bool chk = false;
    int ret_cnt = 0;
    while (1) {
        // 코드를 한 줄씩 가져와서 공백 기준으로 분해 후 해석

        char* token;
        char* nextToken;
        if (chk) {
            if (hre->pc < 1 || hre->pc > hre->code->size)
                return -1;   //pc가 유효하지 않을경우 리턴-1
        } //pc가 유효하지 않을 경우

        char* codeCopy = _strdup(hre->code->instr[hre->pc].hsm_code);
        token = strtok_s(codeCopy, " ", &nextToken); //이부분에서 원본 복제된 코드 훼손됨
        while (token != NULL && tokenCount < 100) {
            strcpy_s(tokens[tokenCount], sizeof(tokens[tokenCount]), token);
            tokenCount++;
            token = strtok_s(NULL, " ", &nextToken);
        }
        free(codeCopy);
        if (strcmp(tokens[0], "HSM_SEP") == 0) {
            chk = true;
            for (int j = 0; j < tokenCount; j++) {
                memset(tokens[j], 0, sizeof(tokens[j]));
            }
            tokenCount = 0;
            hre->pc++;
            continue;
        }
        else if (strcmp(tokens[0], "HSM_ALIAS") == 0){
            if (tokenCount < 3) {
                // 잘못된 형식의 alias 명령어 처리
                printf("잘못된 인수의 개수\n");
                return -1;
            }

            const char* registerName = tokens[1];
            const char* alias = tokens[2];

            int registerIndex = -1;
            for (int i = 0; i < N_REGISTER; i++) {
                if (strcmp(hre->register_alias[i], registerName) == 0) {
                    registerIndex = i;
                    break;
                }
            }

            if (registerIndex == -1) {
                // 유효하지 않은 레지스터 이름인 경우 처리
                printf("유효하지 않은 레지스터: %s\n", registerName);
                return -1;
            }

            // 별명을 해당 레지스터에 할당
            strcpy_s(hre->register_alias[registerIndex], sizeof(hre->register_alias[registerIndex]), alias);
            for (int j = 0; j < tokenCount; j++) {
                memset(tokens[j], 0, sizeof(tokens[j]));
            }
            tokenCount = 0;
            hre->pc++;
        }
        else if (strcmp(tokens[0], "HSM_POP") == 0) {
            //레지스터 찾기
            const char* variableName = tokens[1];
            int registerIndex = -1;
            for (int i = 0; i < N_REGISTER; i++) {
                if (strcmp(hre->register_alias[i], variableName) == 0) {
                    registerIndex = i;
                    break;
                }
            }
            int8 value;
            if (stack_pop(hre->stack, &value)) {
                hre->registers[registerIndex] = value; 
            }
            else {
                return -1;
            }
            for (int j = 0; j < tokenCount; j++) {
                memset(tokens[j], 0, sizeof(tokens[j]));
            }
            tokenCount = 0;
            hre->pc++;
        }
        else if (strcmp(tokens[0], "HSM_PUSH") == 0) {
            int8 value;
            if (tokens[1] == NULL) {
                printf("잘못된 인수의 개수\n");
                return -1;
            }
            if (tokens[1][0] == '0' && tokens[1][1] == '\0') { // 0일경우
                if (tokens[1] != NULL) {
                    value = (int8)atoi(tokens[1]);
                    stack_push(hre->stack, value);
                    for (int j = 0; j < tokenCount; j++) {
                        memset(tokens[j], 0, sizeof(tokens[j]));
                    }
                    tokenCount = 0;
                    hre->pc++;
                    continue;
                }
            }

            int num = atoi(tokens[1]);
            if (!check_overflow(num)) {
                return -1;
            }//토큰1에 입력된 정수가 범위에 만족하는지 체크 아니면 리턴-1
            if (num != 0) {
                value = (int8)num;
                stack_push(hre->stack, value);
            }
            else {
                const char* variableName = tokens[1];
                int registerIndex = -1;
                for (int i = 0; i < N_REGISTER; i++) {
                    if (strcmp(hre->register_alias[i], variableName) == 0) {
                        registerIndex = i;
                        break;
                    }
                }
                value = hre->registers[registerIndex];
                stack_push(hre->stack, value);
            }
            for (int j = 0; j < tokenCount; j++) {
                memset(tokens[j], 0, sizeof(tokens[j]));
            }
            tokenCount = 0;
            hre->pc++;
        }
        else if (strcmp(tokens[0], "HSM_RET") == 0) {
        ret_cnt++;
            for (int j = 0; j < tokenCount; j++) {
                memset(tokens[j], 0, sizeof(tokens[j]));
            }
            tokenCount = 0;
            break;
        }
        else if (strcmp(tokens[0], "HSM_ADD") == 0) {
            const char* variableName = tokens[1];
            int registerIndex = -1;
            for (int i = 0; i < N_REGISTER; i++) {
                if (strcmp(hre->register_alias[i], variableName) == 0) {
                    registerIndex = i;
                    break;
                }
            }
            int num = atoi(tokens[2]);
            int num2 = (int)(hre->registers[registerIndex]);
            num2 += num;
            if (!check_overflow(num)) {
                return -1;
            }//토큰1에 입력된 정수가 범위에 만족하는지 체크 아니면 리턴-1  //더한 수가 범위 넘으면 리턴-1
            hre->registers[registerIndex] = (int8)num2;
            for (int j = 0; j < tokenCount; j++) {
                memset(tokens[j], 0, sizeof(tokens[j]));
            }
            tokenCount = 0;
            hre->pc++;
            }
        else if (strcmp(tokens[0], "HSM_SUB") == 0) {
            const char* variableName = tokens[1];
            int registerIndex = -1;
            for (int i = 0; i < N_REGISTER; i++) {
                if (strcmp(hre->register_alias[i], variableName) == 0) {
                    registerIndex = i;
                    break;
                }
            }
            int num = atoi(tokens[2]);
            int num2 = (int)(hre->registers[registerIndex]);
            num2 -= num;
            if (!check_overflow(num)) {
                return -1;
            }//토큰1에 입력된 정수가 범위에 만족하는지 체크 아니면 리턴-1
            hre->registers[registerIndex] = (int8)num2;
            for (int j = 0; j < tokenCount; j++) {
                memset(tokens[j], 0, sizeof(tokens[j]));
            }
            tokenCount = 0;
            hre->pc++;
        }
        else if (strcmp(tokens[0], "HSM_NEG") == 0) {
            const char* variableName = tokens[1];
            int registerIndex = -1;
            for (int i = 0; i < N_REGISTER; i++) {
                if (strcmp(hre->register_alias[i], variableName) == 0) {
                    registerIndex = i;
                    break;
                }
            }
            int num;
            int num2 = (int)(hre->registers[registerIndex]);
            num = -num2;
            if (!check_overflow(num)) {
                return -1;
            }//토큰1에 입력된 정수가 범위에 만족하는지 체크 아니면 리턴-1
            hre->registers[registerIndex] = (int8)num;
            for (int j = 0; j < tokenCount; j++) {
                memset(tokens[j], 0, sizeof(tokens[j]));
            }
            tokenCount = 0;
            hre->pc++;
        }
        else if (strcmp(tokens[0], "HSM_ACC") == 0) {
            const char* variableName = tokens[1];
            int registerIndex = -1;
            const char* variableName2 = tokens[2];
            int registerIndex2 = -1;
            for (int i = 0; i < N_REGISTER; i++) {
                if (strcmp(hre->register_alias[i], variableName) == 0) {
                    registerIndex = i;
                    break;
                }
            }
            for (int i = 0; i < N_REGISTER; i++) {
                if (strcmp(hre->register_alias[i], variableName2) == 0) {
                    registerIndex2 = i;
                    break;
                }
            }

            int num = (int)(hre->registers[registerIndex]);
            int num2 = (int)(hre->registers[registerIndex2]);
            num += num2;
            if (!check_overflow(num)) {
                return -1;
            }//토큰1에 입력된 정수가 범위에 만족하는지 체크 아니면 리턴-1
            hre->registers[registerIndex] = (int8)num;
            for (int j = 0; j < tokenCount; j++) {
                memset(tokens[j], 0, sizeof(tokens[j]));
            }
            tokenCount = 0;
            hre->pc++;
        }
        else if (strcmp(tokens[0], "HSM_JE") == 0){
        int8 value;
        if (!stack_pop(hre->stack, &value)) {
            return -1;
        }
        const char* variableName = tokens[1];
        const char* variableName2 = tokens[2];
        int registerIndex = -1;
        int registerIndex2 = -1;
        for (int i = 0; i < N_REGISTER; i++) {
            if (strcmp(hre->register_alias[i], variableName) == 0) {
                registerIndex = i;
                break;
            }
        }
        for (int i = 0; i < N_REGISTER; i++) {
            if (strcmp(hre->register_alias[i], variableName2) == 0) {
                registerIndex2 = i;
                break;
            }
        }

        if ((int)(hre->registers[registerIndex] == (int)hre->registers[registerIndex2])) {
            hre->pc = (int)value + 1;
        }
        else {
            hre->pc++;
        }


        for (int j = 0; j < tokenCount; j++) {
            memset(tokens[j], 0, sizeof(tokens[j]));
        }
        tokenCount = 0;
}
        else if (strcmp(tokens[0], "HSM_JNE") == 0) {
            int8 value;
            if (!stack_pop(hre->stack, &value)) {
                return -1;
            }
            const char* variableName = tokens[1];
            const char* variableName2 = tokens[2];
            int registerIndex = -1;
            int registerIndex2 = -1;
            for (int i = 0; i < N_REGISTER; i++) {
                if (strcmp(hre->register_alias[i], variableName) == 0) {
                    registerIndex = i;
                    break;
                }
            }
            for (int i = 0; i < N_REGISTER; i++) {
                if (strcmp(hre->register_alias[i], variableName2) == 0) {
                    registerIndex2 = i;
                    break;
                }
            }

            if ((int)(hre->registers[registerIndex] != (int)hre->registers[registerIndex2])) {
                hre->pc = (int)value + 1;
            }
            else {
                hre->pc++;
            }


            for (int j = 0; j < tokenCount; j++) {
                memset(tokens[j], 0, sizeof(tokens[j]));
            }
            tokenCount = 0;
        }
        else if (strcmp(tokens[0], "HSM_JL") == 0) {
        int8 value;
        if (!stack_pop(hre->stack, &value)) {
            return -1;
        }
        const char* variableName = tokens[1];
        const char* variableName2 = tokens[2];
        int registerIndex = -1;
        int registerIndex2 = -1;
        for (int i = 0; i < N_REGISTER; i++) {
            if (strcmp(hre->register_alias[i], variableName) == 0) {
                registerIndex = i;
                break;
            }
        }
        for (int i = 0; i < N_REGISTER; i++) {
            if (strcmp(hre->register_alias[i], variableName2) == 0) {
                registerIndex2 = i;
                break;
            }
        }

        if ((int)(hre->registers[registerIndex] < (int)hre->registers[registerIndex2])) {
            hre->pc = (int)value + 1;
        }
        else {
            hre->pc++;
        }


        for (int j = 0; j < tokenCount; j++) {
            memset(tokens[j], 0, sizeof(tokens[j]));
        }
        tokenCount = 0;



}
        else if (strcmp(tokens[0], "HSM_JLE") == 0) {
        int8 value;
        if (!stack_pop(hre->stack, &value)) {
            return -1;
        }
        const char* variableName = tokens[1];
        const char* variableName2 = tokens[2];
        int registerIndex = -1;
        int registerIndex2 = -1;
        for (int i = 0; i < N_REGISTER; i++) {
            if (strcmp(hre->register_alias[i], variableName) == 0) {
                registerIndex = i;
                break;
            }
        }
        for (int i = 0; i < N_REGISTER; i++) {
            if (strcmp(hre->register_alias[i], variableName2) == 0) {
                registerIndex2 = i;
                break;
            }
        }

        if ((int)(hre->registers[registerIndex] <= (int)hre->registers[registerIndex2])) {
            hre->pc = (int)value + 1;
        }
        else {
            hre->pc++;
        }


        for (int j = 0; j < tokenCount; j++) {
            memset(tokens[j], 0, sizeof(tokens[j]));
        }
        tokenCount = 0;


}
        else if (strcmp(tokens[0], "HSM_JG") == 0) {
        int8 value;
        if (!stack_pop(hre->stack, &value)) {
            return -1;
        }
        const char* variableName = tokens[1];
        const char* variableName2 = tokens[2];
        int registerIndex = -1;
        int registerIndex2 = -1;
        for (int i = 0; i < N_REGISTER; i++) {
            if (strcmp(hre->register_alias[i], variableName) == 0) {
                registerIndex = i;
                break;
            }
        }
        for (int i = 0; i < N_REGISTER; i++) {
            if (strcmp(hre->register_alias[i], variableName2) == 0) {
                registerIndex2 = i;
                break;
            }
        }

        if ((int)(hre->registers[registerIndex] > (int)hre->registers[registerIndex2])) {
            hre->pc = (int)value + 1;
        }
        else {
            hre->pc++;
        }


        for (int j = 0; j < tokenCount; j++) {
            memset(tokens[j], 0, sizeof(tokens[j]));
        }
        tokenCount = 0;


}
        else if (strcmp(tokens[0], "HSM_JGE") == 0) {
        int8 value;
        if (!stack_pop(hre->stack, &value)) {
            return -1;
        }
        const char* variableName = tokens[1];
        const char* variableName2 = tokens[2];
        int registerIndex = -1;
        int registerIndex2 = -1;
        for (int i = 0; i < N_REGISTER; i++) {
            if (strcmp(hre->register_alias[i], variableName) == 0) {
                registerIndex = i;
                break;
            }
        }
        for (int i = 0; i < N_REGISTER; i++) {
            if (strcmp(hre->register_alias[i], variableName2) == 0) {
                registerIndex2 = i;
                break;
            }
        }

        if ((int)(hre->registers[registerIndex] >= (int)hre->registers[registerIndex2])) {
            hre->pc = (int)value + 1;
        }
        else {
            hre->pc++;
        }


        for (int j = 0; j < tokenCount; j++) {
            memset(tokens[j], 0, sizeof(tokens[j]));
        }
        tokenCount = 0;


}
        else {
        return -1;
}
    }
    if (ret_cnt == 0) {
        return -1; //ret없으면 -1리턴 
    }
	// 아래 코드는 수정하지 말 것
	char return_value;
	if (stack_pop(hre->stack, &return_value))
		return return_value;
	else
		return -1;
}

// HRE 및 HRE 내부의 동적 할당 메모리 해제
void hre_release(HRE* hre) {
    stack_release(hre->stack);
    free(hre);
}
