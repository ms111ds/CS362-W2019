#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
    int     charVal;
    int     spread;

    // test all characters.
    spread = 0x80; // 128 ascii characters
    charVal = rand() % spread;

    return (char)charVal;
}

char *inputString()
{
    int     i;
    int     size;
    int     charSelector;
    int     maxSize = 10;
    int     charSpread = 7;
    char *  s;

    // make random size of size 1 to maxSize
    size = 1 + rand() % maxSize;
    s = malloc(sizeof(char) * size);
    if (s == NULL)
        return NULL;

    // fill array with characters
    for (i = 0; i < size - 1; i++)
    {
        charSelector = rand() % charSpread;
        switch(charSelector)
        {
            case 0:
                s[i] = 'r';
                break;
            case 1:
                s[i] = 'e';
                break;
            case 2:
                s[i] = 's';
                break;
            case 3:
                s[i] = 't';
                break;
            case 4:
                s[i] = '\0';
                break;
            case 5:
                s[i] = (char)0x7F; // final ascii character
                break;
            default:
                s[i] = (char)0x1F; // another ascii char
                break;
        }
    }

    return s;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
