#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 100000
#define PATTERN_LENGTH 44

int main(int argc, char *argv[])
{
    FILE *file;
    FILE *output;
    int length, difficulty = 0, arrow_number = 0;
    int(*chart)[6]; // chart[difficulty][row]
    char *buffer = (char *)malloc(BUF_SIZE);

    //Yeah.
    char* valkyrie[PATTERN_LENGTH] = {
 "1000\n", "0100\n", "1000\n", "0100\n", "0001\n", "0100\n", "0001\n", 
    "0100\n", "1000\n", "0100\n", "1000\n", "0100\n", "1000\n", "0010\n", "1000\n", "0010\n", 
    "0001\n", "0010\n", "0001\n", "0010\n", "1000\n", "0010\n", "1000\n", "0100\n", "1000\n",
    "0100\n", "1000\n", "0010\n", "0100\n", "0001\n", "1000\n", "0100\n", "0020\n", "0000\n",
    "0000\n", "0000\n", "0000\n", "0000\n", "0000\n", "0000\n", "0000\n", "0000\n", "0000\n", 
    "0030\n"};


    if (argc < 2)
    {
        printf("Usage : %s stepfile\n", argv[0]);
        exit(1);
    }
    else
    {

        // Open the original .sm file
        file = fopen(argv[1], "r");
        fseek(file, 0, SEEK_END);
        length = (int)ftell(file);
        fseek(file, 0, SEEK_SET);
        fgets(buffer, BUF_SIZE, file);

        // Open the converted chart
        output = fopen("output.sm", "w");

        // Copy the description lines
        while (buffer[0] == '#')
        {
            fputs(buffer, output);
            fgets(buffer, BUF_SIZE, file);
        }
        // More description lines
        if (buffer[0] == '/')
        {
            for (int i = 0; i < 7; i++)
            {
                fputs(buffer, output);
                fgets(buffer, BUF_SIZE, file);
            }
        }

        // Parse the charted notes until EOF
        fseek(file, -strlen(buffer), SEEK_CUR);
        while (fgets(buffer, BUF_SIZE, file) != NULL)
        {
            if (strstr(buffer, "dance-double") != NULL) break;
            //Replace any note with one from valkyrie dimension
            if ((strchr(buffer,'M') != NULL || strchr(buffer,'K') != NULL || strchr(buffer,'L') != NULL  || strchr(buffer,'F') != NULL  ||
                 strchr(buffer,'0') != NULL  || strchr(buffer,'1') != NULL || strchr(buffer,'2') != NULL || strchr(buffer,'3') != NULL ||
                 strchr(buffer,'4') != NULL ) && strlen(buffer) == 6 && strncmp(buffer, "0000", 4) != 0)
            {
                printf("%s", valkyrie[arrow_number % PATTERN_LENGTH]);
                fputs(valkyrie[arrow_number % PATTERN_LENGTH], output);
                ++arrow_number;
            }
            else
            //If it isn't a note, or an empty line, output to file directly
            {
                 printf("%s", buffer);
                fputs(buffer, output);
                //If it's a new difficulty, reset the arrow counter
                if (buffer[0] == ';')
                    arrow_number = 0;
                        }
        }
        fclose(output);
        fclose(file);
    }

    return EXIT_SUCCESS;
}