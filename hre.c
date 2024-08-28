#include <stdio.h>
#include "hre.h"
#include <string.h>
#include <stdlib.h>

/*for (int j = 0; j < tokenCount; j++) {
                memset(tokens[j], 0, sizeof(tokens[j]));
            }
            tokenCount = 0;
            hre->pc++;
*/  //��ū�ʱ�ȭ�ϴ� �ڵ�

/*
* �������� ����ã�� �ڵ�
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
} //�������˸´� value���� üũ!


//�����ؾ��� �ڵ�
//load �Ϸ�
int load(char* filename, char* buf) {
    bool chk = false;  //�ϴ� chk false�� �س���  ���� �б� ��� �дٰ� 
    FILE* file;
    if (fopen_s(&file, filename, "r") != 0) {
        return -1; // ���� ���� ���� �� -1 ��ȯ
    }

    int byteCount = 0; // ���� ����Ʈ ���� ������ ����

    while (!feof(file)) {
        int ch = fgetc(file); // ���Ͽ��� ���� �ϳ��� �о�ɴϴ�.

        if (ch == EOF) {
            break; // ������ ���� �����ϸ� �ݺ��� ����
        }
        if ((char)ch == '!') {
            chk = true;
        }
        if (chk) {
            *buf++ = (char)ch; // ���� ���ڸ� ���ۿ� �����ϰ�, ���� �����͸� ���� ��ġ�� �̵��մϴ�.
            byteCount++; // ����Ʈ �� ����
        }
    }

    fclose(file); // ������ �ݽ��ϴ�.
    return byteCount; // ���� ����Ʈ ���� ��ȯ�մϴ�.
}

// buf�� �ؽ�Ʈ�� �ؼ��ؼ� code�� ��ɾ� ������ ����
// ������ ����� false, �ƴϸ� true ���� �Ϸ�
bool parse(char* buf, int bufsize, BYTECODE* code) {
    code->size = 0;
    char temp_str[INSTR_LEN_MAX] = "";  // �� ���ڿ��� �ʱ�ȭ
    
    for (int i = 0; i < bufsize; i++) {

        if (buf[i] == '\n') {
            if (strcpy_s(code->instr[code->size].raw_code, INSTR_LEN_MAX, temp_str) != 0) {
                return false;  // ���� ���� �� false ��ȯ
            }
            code->size++;
            temp_str[0] = '\0';  // �� ���ڿ��� �ʱ�ȭ
        }
        else {
            strncat_s(temp_str, INSTR_LEN_MAX, &buf[i], 1);  // ���� �ϳ��� ������
        }
    }
    if (strcpy_s(code->instr[code->size].raw_code, INSTR_LEN_MAX, temp_str) != 0) {
        return false;  // ���� ���� �� false ��ȯ
    }
    code->size++;
    temp_str[0] = '\0';


    
    char tokens[100][100];
    int tokenCount = 0;
    for (int i = 0; i < code->size; i++) {
        // �ڵ带 �� �پ� �����ͼ� ���� �������� ���� �� �ؼ�
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
        // tokens �迭 �ʱ�ȭ
        for (int j = 0; j < tokenCount; j++) {
            memset(tokens[j], 0, sizeof(tokens[j]));
        }
        tokenCount = 0;
    }
    return true;
}



// HRE �ʱ�ȭ(���� �Ҵ�)
// ����Ʈ�ڵ�, ����, ��������, ���α׷� ī���͸� �ʱ�ȭ�Ѵ�.
// �����ϸ� NULL ����
// ���ÿ��� argc, argv�� �Ѱܹ��� ���� argv[0], argv[1], ..., argc ������ Ǫ��
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

// bytecode ����
// ���������� �ݵ�� ���ÿ��� pop�� ���� ����
// �̻��� ������ -1�� ����
char hre_execute(HRE* hre) {

    char tokens[100][100];
    int tokenCount = 0;
    bool chk = false;
    int ret_cnt = 0;
    while (1) {
        // �ڵ带 �� �پ� �����ͼ� ���� �������� ���� �� �ؼ�

        char* token;
        char* nextToken;
        if (chk) {
            if (hre->pc < 1 || hre->pc > hre->code->size)
                return -1;   //pc�� ��ȿ���� ������� ����-1
        } //pc�� ��ȿ���� ���� ���

        char* codeCopy = _strdup(hre->code->instr[hre->pc].hsm_code);
        token = strtok_s(codeCopy, " ", &nextToken); //�̺κп��� ���� ������ �ڵ� �Ѽյ�
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
                // �߸��� ������ alias ��ɾ� ó��
                printf("�߸��� �μ��� ����\n");
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
                // ��ȿ���� ���� �������� �̸��� ��� ó��
                printf("��ȿ���� ���� ��������: %s\n", registerName);
                return -1;
            }

            // ������ �ش� �������Ϳ� �Ҵ�
            strcpy_s(hre->register_alias[registerIndex], sizeof(hre->register_alias[registerIndex]), alias);
            for (int j = 0; j < tokenCount; j++) {
                memset(tokens[j], 0, sizeof(tokens[j]));
            }
            tokenCount = 0;
            hre->pc++;
        }
        else if (strcmp(tokens[0], "HSM_POP") == 0) {
            //�������� ã��
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
                printf("�߸��� �μ��� ����\n");
                return -1;
            }
            if (tokens[1][0] == '0' && tokens[1][1] == '\0') { // 0�ϰ��
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
            }//��ū1�� �Էµ� ������ ������ �����ϴ��� üũ �ƴϸ� ����-1
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
            }//��ū1�� �Էµ� ������ ������ �����ϴ��� üũ �ƴϸ� ����-1  //���� ���� ���� ������ ����-1
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
            }//��ū1�� �Էµ� ������ ������ �����ϴ��� üũ �ƴϸ� ����-1
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
            }//��ū1�� �Էµ� ������ ������ �����ϴ��� üũ �ƴϸ� ����-1
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
            }//��ū1�� �Էµ� ������ ������ �����ϴ��� üũ �ƴϸ� ����-1
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
        return -1; //ret������ -1���� 
    }
	// �Ʒ� �ڵ�� �������� �� ��
	char return_value;
	if (stack_pop(hre->stack, &return_value))
		return return_value;
	else
		return -1;
}

// HRE �� HRE ������ ���� �Ҵ� �޸� ����
void hre_release(HRE* hre) {
    stack_release(hre->stack);
    free(hre);
}
