#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct input   Input;
typedef enum   action  Action;
typedef struct item    Item;

enum action {
             ACTION_ADD = 0,
             ACTION_LIST,
             ACTION_COMPLETE,
             ACTION_CLEAR,
             ACTION_COUNT
};

const unsigned int action_buffer_max = 8;
char* const action_labels[] = {
                         "add",
                         "list",
                         "complete",
                         "clear",
                         "unknown"
};

Action action_from_label(char *label) {
  Action action = ACTION_COUNT;
  
  for (int i = 0; i < ACTION_COUNT; i++) {
    if (strcmp(action_labels[i], label) == 0) {
      action = (Action)i;
      break;
    }
  }

  return action;
}

struct input {
  Action action;
  char* data;
};

void input_print(Input input) {
  printf(
         "Input:\n\tAction: %s\n\tData: not-yet\n",
         action_labels[(unsigned int)input.action]
  );
}

struct item {
  char* name;
  int complete;
};

Input input_parse_from_buffer(char* buffer) {
  Input input = {
                 ACTION_COUNT,
                 action_labels[ACTION_COUNT]
  };
  unsigned int i = 0;
  char action_buffer[action_buffer_max];
  

  while(buffer[i] != EOF && !isspace(buffer[i])) {
    action_buffer[i] = buffer[i];
    i++;
  }

  action_buffer[i] = '\0';
  printf("DEBUG: action is: %s\n", action_buffer);
  input.action = action_from_label(action_buffer);

  return input;
}

#define PRINT_INPUT_READY() printf("> ");

int main(int argc, char** argv) {
  printf("Hello, World!\n");

  PRINT_INPUT_READY();

  const unsigned int line_buffer_max = 256;
  char line_buffer[line_buffer_max];
  while(fgets((char*)line_buffer, line_buffer_max, stdin)) {
    printf(":: %s\n", line_buffer);
    Input input = input_parse_from_buffer(line_buffer);
    input_print(input);

    PRINT_INPUT_READY();
  }
}
