#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool isValidOpcode(char *opcode) {
    char *validOpcodes[] = {"LOAD", "STORE", "ADD", "SUB", "MUL", "DIV", "JUMP"};
    for (int i = 0; i < 7; i++) {
        if (strcmp(opcode, validOpcodes[i]) == 0) {
            return true;
        }
    }
    return false;
}

int main() {
    FILE *inputFile;
    char line[20];
    char opcode[10];

    inputFile = fopen("job_instructions.txt", "r");
    if (inputFile == NULL) {
        printf("Error: Could not open file.\n");
        return 1;
    }

    int lineNumber = 0;
    while (fgets(line, sizeof(line), inputFile)) {
        sscanf(line, "%s", opcode);
        lineNumber++;

        if (!isValidOpcode(opcode)) {
            printf("Opcode Error: Invalid opcode '%s' at line %d. Raising interrupt.\n", opcode, lineNumber);
            fclose(inputFile);
            return 1;
        }
    }

    printf("All opcodes are valid.\n");
    fclose(inputFile);
    return 0;
}
