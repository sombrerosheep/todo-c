#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define LINE_BUFFER_SIZE 256
#define ACTION_BUFFER_SIZE 16
#define ITEM_BUFFER_SIZE ((LINE_BUFFER_SIZE) - (ACTION_BUFFER_SIZE))
#define ITEMS_MAX 256

typedef unsigned int uint;
typedef struct item_t Item;

struct item_t {
  char name[ITEM_BUFFER_SIZE];
  uint complete;
  uint in_use;
};

Item items[ITEMS_MAX];

void Items_Init() {
  for (uint i = 0; i < ITEMS_MAX; i++) {
    items[i].name[0] = '\0';
    items[i].complete = 0;
    items[i].in_use = 0;
  }

  return;
}

void Items_Add(char *name, uint len) {
  for (uint i = 0; i < ITEMS_MAX; i++) {
    if (items[i].in_use == 0) {
      strncpy(items[i].name, name, len);
      items[i].complete = 0;
      items[i].in_use = 1;

      return;
    }
  }

  printf("No available slots. List is full.\n");
  return;
}

void Items_Done(char *name, uint len) {
  for (uint i = 0; i < ITEMS_MAX; i++) {
    if (items[i].in_use == 1 && strncmp(items[i].name, name, len) == 0) {
      items[i].complete = 1;

      return;
    }
  }

  printf("Item '%s' was not on the list\n", name);
  return;
}

void Items_List() {
  for (uint i = 0; i < ITEMS_MAX; i++) {
    if (items[i].in_use == 1) {
      printf("\t%s (%s)\n", items[i].name, items[i].complete == 1 ? "complete" : "incomplete");
    }
  }
}

void get_word(char *buffer, char *out, uint len) {
  uint i = 0;
  uint n = 0;

  // remove any leading spaces
  while (buffer[i] != EOF && i < len && isspace(buffer[i]) != 0) {
    i++;
  }

  while (buffer[i] != EOF && i < len && isspace(buffer[i]) == 0) {
    out[n++] = buffer[i++];
  }

  out[n] = '\0';

  return;
}

#define PROMPT() printf("todo:> ")

void main() {
  char line_buffer[LINE_BUFFER_SIZE];
  char action_buffer[ACTION_BUFFER_SIZE];
  char item_buffer[ITEM_BUFFER_SIZE];

  Items_Init();
  PROMPT();
  while (fgets(line_buffer, LINE_BUFFER_SIZE, stdin)) {
    //    printf("::%s\n", line_buffer);
    get_word(line_buffer, action_buffer, ACTION_BUFFER_SIZE);
    get_word(line_buffer + strlen(action_buffer), item_buffer, ITEM_BUFFER_SIZE);

    printf("action: %s\nitem: %s\n", action_buffer, item_buffer);

    if (strncmp(action_buffer, "add", ACTION_BUFFER_SIZE) == 0) {
      Items_Add(item_buffer, ITEM_BUFFER_SIZE);
    }
    else if (strncmp(action_buffer, "done", ACTION_BUFFER_SIZE) == 0) {
      Items_Done(item_buffer, ITEM_BUFFER_SIZE);
    }
    else if (strncmp(action_buffer, "list", ACTION_BUFFER_SIZE) == 0) {
      Items_List();
    }
    else if (strncmp(action_buffer, "clear", ACTION_BUFFER_SIZE) == 0) {
      Items_Init();
    }
    else if (strncmp(action_buffer, "quit", ACTION_BUFFER_SIZE) == 0) {
      printf("Goodbye!\n");
      break;
    } else {
      printf("command not recognized: %s\n", action_buffer);
    }

    PROMPT();
  }
}
