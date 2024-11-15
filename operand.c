#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool isValidOperand(char *operand) {
    for (int i = 0; i < strlen(operand); i++) {
        if (!isdigit(operand[i]) && operand[i] != 'R') {
            return false;
        }
    }
    return true;
}

int main() {
    FILE *inputFile;
    char line[20];
    char opcode[10], operand[10];

    inputFile = fopen("job_instructions.txt", "r");
    if (inputFile == NULL) {
        printf("Error: Could not open file.\n");
        return 1;
    }

    int lineNumber = 0;
    while (fgets(line, sizeof(line), inputFile)) {
        sscanf(line, "%s %s", opcode, operand);
        lineNumber++;

        if (!isValidOperand(operand)) {
            printf("Operand Error: Invalid operand '%s' at line %d. Raising interrupt.\n", operand, lineNumber);
            fclose(inputFile);
            return 1;
        }
    }

    printf("All operands are valid.\n");
    fclose(inputFile);
    return 0;
}
