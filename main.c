#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct input   Input;
typedef enum   action  Action;
typedef struct item    Item;

enum action {
             ACTION_ADD,
             ACTION_LIST,
             ACTION_COMPLETE,
             ACTION_CLEAR,
             ACTION_COUNT
};

struct input {
  Action action;
  char* data;
};

struct item {
  char* name;
  int complete;
};


#define PRINT_INPUT_READY() printf("> ");

int main(int argc, char** argv) {
  printf("Hello, World!\n");

  PRINT_INPUT_READY();

  const unsigned int line_buffer_max = 256;
  char line_buffer[line_buffer_max];
  while(fgets((char*)line_buffer, line_buffer_max, stdin)) {
    printf(":: %s\n", line_buffer);

    PRINT_INPUT_READY();
  }
}
