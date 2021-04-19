#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define LINE_BUFFER_MAX 256
#define ACTION_BUFFER_MAX 32
#define ITEM_BUFFER_MAX LINE_BUFFER_MAX - ACTION_BUFFER_MAX

#define TODO_ITEMS_MAX 128

typedef enum action_type ActionType;

enum action_type {
  ACTION_TYPE_ADD,
  ACTION_TYPE_DONE,
  ACTION_TYPE_LIST,
  ACTION_TYPE_CLEAR,
  ACTION_TYPE_QUIT,
  ACTION_TYPE_UNKNOWN,
  ACTION_TYPE_COUNT
};

typedef struct todo_item TodoItem;

struct todo_item {
  char name[ITEM_BUFFER_MAX];
  int complete;
  int free;
};

TodoItem items[TODO_ITEMS_MAX];

void list_clear() {
  for (unsigned int i = 0; i < TODO_ITEMS_MAX; i++) {
    items[i].name[0] = '\0';
    items[i].complete = 0;
    items[i].free = 1;
  }
}

void list_add(char *name, unsigned int len) {
  for (unsigned int i = 0; i < TODO_ITEMS_MAX; i++) {
    if (items[i].free == 0) {
      continue;
    }

    strncpy(items[i].name, name, len);
    items[i].complete = 0;
    items[i].free = 0;

    break;
  }

  return;
}

void list_list() {
  for (unsigned int i = 0; i < TODO_ITEMS_MAX; i++) {
    if (items[i].free != 0) {
      continue;
    }

    printf("\t:> %s (%s)\n", items[i].name, items[i].complete > 0 ? "complete" : "incomplete");
  }
}

void list_done(char *name, unsigned int len) {
  for (unsigned int i = 0; i < TODO_ITEMS_MAX; i++) {
    if (items[i].free != 0) {
      continue;
    }

    if (strncmp(name, items[i].name, len) == 0) {
      items[i].complete = 1;

      break;
    }
  }
}

void get_word(char* buffer, unsigned int len, char* out_buffer, unsigned int out_len) {
  unsigned int i = 0;
  unsigned int n = 0;

  // skip leading spaces
  while (buffer[i] != EOF && i < len && isspace(buffer[i]) != 0) {
    i++;
  }

  while (buffer[i] != EOF && n < out_len && isspace(buffer[i]) == 0) {
    out_buffer[n++] = buffer[i++];
  }

  if (n >= out_len) {
    n = out_len - 1;
  }

  out_buffer[n] = '\0';

  return;
}

void get_string(char *buffer, unsigned int len, char* out_buffer, unsigned int out_len) {
  unsigned int i = 0;
  unsigned int n = 0;

  // skip leading spaces
  while (buffer[i] != EOF && i < len && isspace(buffer[i]) != 0) {
    i++;
  }

  while (buffer[i] != EOF && n < out_len && buffer[i] != '\n') {
    out_buffer[n++] = buffer[i++];
  }

  if (n >= out_len) {
    n = out_len - 1;
  }

  out_buffer[n] = '\0';

  return;
}

ActionType parse_action(char *buffer, unsigned int len) {
  if (len < 1) {
    return ACTION_TYPE_UNKNOWN;
  }

  if (strncmp(buffer, "add", len) == 0) {
    return ACTION_TYPE_ADD;
  }
  if (strncmp(buffer, "done", len) == 0) {
    return ACTION_TYPE_DONE;
  }
  if (strncmp(buffer, "list", len) == 0) {
    return ACTION_TYPE_LIST;
  }
  if (strncmp(buffer, "clear", len) == 0) {
    return ACTION_TYPE_CLEAR;
  }
  if (
    strncmp(buffer, "quit", len) == 0 ||
    strncmp(buffer, "exit", len) == 0
  ) {
    return ACTION_TYPE_QUIT;
  }

  return ACTION_TYPE_UNKNOWN;
}

void main() {
  printf("Hello, World!\n");

  int running = 1;
  #define PROMPT() printf("todo> ");

  char line_buffer[LINE_BUFFER_MAX];
  char action_buffer[ACTION_BUFFER_MAX];
  char item_buffer[ITEM_BUFFER_MAX];

  list_clear();
  PROMPT();
  while(running > 0 && fgets(line_buffer, LINE_BUFFER_MAX, stdin)) {
    // printf("\t> %s\n", line_buffer);

    get_word(line_buffer, LINE_BUFFER_MAX, action_buffer, ACTION_BUFFER_MAX);
    unsigned int action_len = strlen(action_buffer);

    get_string(line_buffer + action_len, LINE_BUFFER_MAX - action_len, item_buffer, ITEM_BUFFER_MAX);

    // printf("Action: (%s), Item: (%s)\n", action_buffer, item_buffer);

    ActionType action_type = parse_action(action_buffer, ACTION_BUFFER_MAX);

    switch (action_type) {
      case ACTION_TYPE_ADD: {
        // printf("adding...\n");
        list_add(item_buffer, ITEM_BUFFER_MAX);
        break;
      }
      case ACTION_TYPE_DONE: {
        // printf("completing...\n");
        list_done(item_buffer, ITEM_BUFFER_MAX);
        break;
      }
      case ACTION_TYPE_LIST: {
        // printf("listing...\n");
        list_list();
        break;
      }
      case ACTION_TYPE_CLEAR: {
        // printf("clearing...\n");
        list_clear();

        break;
      }
      case ACTION_TYPE_QUIT: {
        // printf("quiting...\n");
        running = 0;
        break;
      }
      case ACTION_TYPE_UNKNOWN:
      default: {
        printf("Unknown Command: %s\n", action_buffer);
        printf("defaulted...\n");

      }
    }

    if (running > 0) {
      PROMPT();
    }
  }
}