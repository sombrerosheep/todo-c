#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define INPUT_BUFFER_MAX 64
#define WORD_BUFFER_MAX 64

typedef struct input Input;

typedef enum action_type ActionType;

typedef struct todo_item TodoItem;

void get_word(char *buffer, char * out_buffer, unsigned int len) {
  unsigned int i = 0;
  unsigned int j = 0;

  if (i < len && isspace(buffer[i])) {
    i++;
  }

  while(i < len && !isspace(buffer[i]) && buffer[i] != EOF) {
    out_buffer[j] = buffer[i];

    j++;
    i++;
  }

  out_buffer[j] = '\0';
}

struct todo_item {
  char name[WORD_BUFFER_MAX];
  int complete;
  int vacant;
};

void todo_item_print(TodoItem item) {
    printf("Item: %s, complete? %s\n", item.name, item.complete > 0 ? "yes" : "no");
}

#define TODO_ITEMS_MAX 16

TodoItem items[TODO_ITEMS_MAX];

void items_init() {
  for (int i = 0; i < TODO_ITEMS_MAX; i++) {
    items[i].name[0] = '\0';
    items[i].complete = 0;
    items[i].vacant = 1;
  }
}

void items_print() {
  for (int i = 0; i < TODO_ITEMS_MAX; i++) {
    if (items[i].vacant == 0) {
      printf("\t");
      todo_item_print(items[i]);
    }
  }
}

void items_add(char *name, unsigned int len) {
  for (int i = 0; i <  TODO_ITEMS_MAX; i++) {
    if (items[i].vacant == 0) {
      continue;
    }

    items[i].vacant = 0;
    items[i].complete = 0;
    strncpy(items[i].name, name, len);

    return;
  }
}

void items_complete(char *name, unsigned int len) {
  for (int i = 0; i <  TODO_ITEMS_MAX; i++) {
    if (items[i].vacant != 0) {
      continue;
    }

    // printf("comparing: '%s' with '%s'\n", name, items[i].name);
    if(strncmp(name, items[i].name, len) == 0) {
      items[i].complete = 1;

      return;
    }
  }

  printf("item '%s' not found in list\n", name);
}

enum action_type {
  ACTION_TYPE_ADD = 0,
  ACTION_TYPE_LIST,
  ACTION_TYPE_DONE,
  ACTION_TYPE_QUIT,
  ACTION_TYPE_COUNT
};

char *action_type_get_label(ActionType type) {
  switch (type) {
    case ACTION_TYPE_ADD: return "add";
    case ACTION_TYPE_LIST: return "list";
    case ACTION_TYPE_DONE: return "done";
    case ACTION_TYPE_QUIT: return "quit";
    default: return "unknown";
  }
}

ActionType action_type_parse(char *buffer, unsigned int len) {
  ActionType type = ACTION_TYPE_COUNT;

  for (int i = 0; i < (int)ACTION_TYPE_COUNT; i++) {
    if (strncmp(buffer, action_type_get_label((ActionType)i), len) == 0) {
      type = (ActionType)i;

      break;
    }
  }

  return type;
}

struct input {
  ActionType type;
  char item[WORD_BUFFER_MAX];
};

Input input_parse(char *buffer, unsigned int len) {
  Input input;
  input.type = ACTION_TYPE_COUNT;
  char action_buffer[WORD_BUFFER_MAX];

  get_word(buffer, action_buffer, WORD_BUFFER_MAX);

  input.type = action_type_parse(action_buffer, WORD_BUFFER_MAX);

  get_word(buffer + strlen(action_buffer), input.item, WORD_BUFFER_MAX);

  return input;
}

void input_print(Input input) {
  printf("Input: %s (%s)\n", action_type_get_label(input.type), input.item);
}

void input_act(Input input) {
  switch (input.type) {
    case ACTION_TYPE_ADD: {
      // printf("Adding\n");
      items_add(input.item, WORD_BUFFER_MAX);
      
      break;
    }
    case ACTION_TYPE_LIST: {
      // printf("Listing\n");
      items_print();

      break;
    }
    case ACTION_TYPE_DONE: {
      // printf("Completion\n");
      items_complete(input.item, WORD_BUFFER_MAX);

      break;
    }
      default: printf("Unknown action\n");
  }
}

#define PROMPT() printf("todo> ")

void main() {
  char input_buffer[INPUT_BUFFER_MAX];

  items_init();

  PROMPT();
  while(fgets(input_buffer, INPUT_BUFFER_MAX, stdin)) {
    Input input = input_parse(input_buffer, INPUT_BUFFER_MAX);
    // input_print(input);

    if (input.type == ACTION_TYPE_QUIT) {
      printf("bye bye\n");

      break;
    }
    
    input_act(input);

    PROMPT();
  }
}