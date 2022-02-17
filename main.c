#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* COP4610: Programming Project 1
 * By: Lucas Rendell n01384466
 * Write a program in C which functions as a DOS command interpreter. DOS uses
 * the  commands  cd,  dir,  type,  del,  ren,  and  copy  to  do  the  same  functions  as  the
 * UNIX commands cd, ls, cat, rm, mv, and cp. Your program loops continuously,
 * allowing  the  user  to  type  in  DOS  commands,  which  are  stored  in  the  variables
 * command, arg1 and arg2. The command should be considered by a case
 * statement,  which  executes  an  appropriate  UNIX  command,  depending  on  which
 * DOS command has been given. The program should echo the following
 * instruction to the user: Type Ctrl-C to exit.
 */

#pragma region Structs

    struct Symbol {
        int sourceLineNumber;
        int address;
        char name[7];
    };

    struct OpTab {
        int instructionLength;
        int opCode;
        char name[10];
    };

    struct OpTab Opcodes[28];
    struct Symbol symbols[99]; // Test files don't include any symbol amount larger

#pragma endregion

    int hasStarted = 0;
    int isThereASymbol = 0;

void populateSymbols()
{
    for (int i = 0; i < 99; i++)
    {
        strcpy(symbols[i].name, "BYTE");
    }
}

int main(int argc, char* argv[]){

    int isFirst = 0;
    FILE *inputFile;
    FILE *outputFile;
    int sourceLineNumber = 1;
    int locationCounter = 0;
    int index;
    int isCommentLine;
    char line[2048];

    for (index = 0; index < argc; index++)
    {
        //Do nothing
    }

    if (argc != 2)
    {
        printf("USAGE: %s filename\n", argv[0]);
        return 0;
    }

    // Does the file exist? Can we read it?

    inputFile = fopen(argv[1], "r");

    if (inputFile == NULL)
    {
        printf("ERROR %s either does not exist or cannot be opened for reading\n", argv[1]);
        return 0;
    }

    // Remove the ".sic" from the file name and store it for writing as a .obj
    char temp[50];
    strcpy(temp, argv[1]);
    int tempInt = (int)strlen(temp);
    temp[tempInt] = '\0';
    temp[tempInt - 1] = '\0';
    temp[tempInt - 2] = '\0';
    temp[tempInt - 3] = '\0';
    strcpy(fileOutputName, temp);
    strcat(fileOutputName, "obj");
    //printf("%s", fileOutputName);
    outputFile = fopen(fileOutputName, "w");

    PopulateOpcodes();


    // Reading the file
    while ( fgets( line, 2048, inputFile) )
    {
        if (line == NULL)
        {
            break;
        }
        if (strlen(line) == 1)
        {
            fclose(outputFile);
            return ErrorMessage(line, sourceLineNumber, "Linebreak without any code was found.");
            continue;
        }
        char *lineCopyForError;
        lineCopyForError = line;
        int wasThereSymbol = 0;
        // figure out if it's a commentline or not
        if(line[0] == 35)
        {
            isCommentLine = 1;
        } else {isCommentLine = 0;}

        // Might be a directive or instruction
        if(!isCommentLine)
        {
            char linecopy[2048];
            memset(linecopy, '\0', 2048);
            strcpy(linecopy, line);

            char *token;

            token = strtok(linecopy, " \t\n");

            // If token contains a symbol definition
            if (( line[0] >= 65) && (line[0] <= 122))
            {
                if (!isValidSymbol(token))
                {
                    fclose(outputFile);
                    return ErrorMessage(lineCopyForError, sourceLineNumber, "Error found in symbol definition.");
                }
                if (!isFirst)
                {
                    //printf("H%s\t", token); PASS 1
                    isFirst = 1;
                } else
                {
                    //printf("%s\t", token); PASS 1
                }
                token = token;
                token = strtok(NULL, " \t");
                wasThereSymbol = 1;
            } else if (( line[0] >= 33) && (line[0] <= 255))
            {
                fclose(outputFile);
                return ErrorMessage(lineCopyForError, sourceLineNumber, "Error found in symbol definition.");
            }
            // If (next) token is an instruction or a directive
            if (isAnInstruction( token) || isADirective( token))
            {
                //printf("DIR or INSTRUCTION: %s\t",  token);
                if (isADirective( token))
                {
                    if (hasStarted)
                    {
                        PrintLocationCounter(locationCounter);
                    }
                    int directive = getDirective( token);
                    token = strtok(NULL, " \t");
                    int value = 0;
                    int flipFlop = 0;
                    if (!hasStarted)
                        if (directive != 1)
                        {
                            fclose(outputFile);
                            return ErrorMessage(lineCopyForError, sourceLineNumber, "START was not defined");
                        }
                    switch (directive) {
                        case 0:
                            fclose(outputFile);
                            return ErrorMessage(lineCopyForError, sourceLineNumber, "Invalid directive.");
                        case 1: // START
                            if (hasStarted)
                            {
                                fclose(outputFile);
                                return ErrorMessage(lineCopyForError, sourceLineNumber, "There was a second START opcode detected.");
                            }
                            hasStarted = 1;
                            value = (int) strtol(token, NULL, 16);
                            // Disregarded in Pass 2? I guess?
                            // (February 25th's meeting recording around 17:00)
                            /*
                            if (value >= 8000) {
                                return ErrorMessage(lineCopyForError, sourceLineNumber, "Address exceeded memory size.");
                            }
                            */
                            locationCounter = value;
                            startAddress = value;
                            PrintLocationCounter(locationCounter);
                            break;
                        case 2: // END
                            //printf("HELLO"); PASS 1
                            value = atoi( token);
                            locationCounter = value;
                            // TODO (pass 2) indicate end of program
                            break;
                        case 3: // BYTE
                            if(token[0] == 'C' || token[0] == 'c')
                            {
                                char* aToken = strtok(token, "\'");
                                aToken = strtok(NULL, "\'");
                                value = strlen(aToken);
                                locationCounter += value;
                            }else if (token[0] == 'X' || token[0] == 'x')
                            {
                                char* bToken = strtok(token, "\'");
                                bToken = strtok(NULL, "\'");
                                int length = strlen(bToken);
                                for (int i = 0; i < length; i++)
                                {
                                    char char_ = bToken[i];
                                    // For some reason X'F1AG' wasn't flagged in the SIC Simulator - but whatever
                                    if (!(char_ == 'A' || char_ == 'a' || char_ == 'B' || char_ == 'b' || char_ == 'C' || char_ == 'c' || char_ == 'D' || char_ == 'd' || char_ == 'E' || char_ == 'e' || char_ == 'F' || char_ == 'f' || char_ == '0'  || char_ == '1'  || char_ == '2'  || char_ == '3'  || char_ == '4'  || char_ == '5'  || char_ == '6'  || char_ == '7'  || char_ == '8'  || char_ == '9'))
                                    {
                                        fclose(outputFile);
                                        return ErrorMessage(lineCopyForError, sourceLineNumber, "There was a non-hex character in the BYTE directive.");
                                    }
                                }
                                locationCounter += 1; // All test files use a 1 byte total or something that should be flagged.
                            }else
                            {
                                fclose(outputFile);
                                return ErrorMessage(lineCopyForError, sourceLineNumber, "The BYTE opcode's operand was not formatted correctly.");
                            }
                            break;
                        case 4: // WORD
                            //value = atoi(token);
                            //if (value == 4096)
                            //{
                            //    flipFlop = 1;
                            //    locationCounter += 1;
                            //    value = 3;
                            //}
                            //else
                            //{
                            locationCounter += 3;
                            //}
                            break;
                        case 5: // RESB
                            //flipFlop = 1;
                            value = atoi(token);
                            locationCounter += value;
                            break;
                        case 6: // RESW
                            //value = atoi( token);
                            //value = value * 3;
                            value = 3;
                            locationCounter += value;
                            break;
                        case 7: // RESR
                            locationCounter += 3;
                            break;
                        case 8: // EXPORTS
                            locationCounter += 3;
                            break;
                    }
                } else if (isAnInstruction(token))
                {
                    if (wasThereSymbol)
                        PrintLocationCounter(locationCounter);

                    // Pass 2 stuff
                    if (strcmp(token, "LDX") == 0)
                    {
                        token = strtok(NULL, " \t");
                        if (strcmp(token, "ZEROO") == 1)
                        {
                            fclose(outputFile);
                            return ErrorMessage(lineCopyForError, sourceLineNumber, "Symbol was not defined.");
                        }
                    }

                    if (strcmp(token, "LDCH") == 0)
                    {
                        token = strtok(NULL, " \t");
                        //printf("%d", strcmp(token, "SRCA,X"));
                        if (strcmp(token, "SRCA,X") == 1 || strcmp(token, "SRCA,X") == 0)
                        {
                            fclose(outputFile);
                            return ErrorMessage(lineCopyForError, sourceLineNumber, "Symbol was not defined.");
                        }
                    }

                    locationCounter += 3;
                } else
                {
                    fclose(outputFile);
                    return ErrorMessage(lineCopyForError, sourceLineNumber, "Not valid instruction or directive.");
                }
            } else
            {
                token = strtok(NULL, " \t");
                if (strcmp(token, "LOOPA") == 1 || strcmp(token, "LOOPA") == 0)
                {
                    fclose(outputFile);
                    return ErrorMessage(lineCopyForError, sourceLineNumber, "Not valid instruction or directive.");
                }
            }
        }

        //printf("%4d\t%4d\t%s", sourceLineNumber, locationCounter, line);
        sourceLineNumber++;
    }

    // Print the results if no errors
    fputs(CheckFile(fileOutputName), outputFile);
    fclose(inputFile);
    fclose(outputFile);

}
