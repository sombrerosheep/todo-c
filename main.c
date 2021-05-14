#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define LINE_BUFFER_SIZE 256
#define ACTION_BUFFER_SIZE 8
#define ITEM_BUFFER_SIZE (LINE_BUFFER_SIZE - ACTION_BUFFER_SIZE)
#define ITEMS_MAX 64

typedef unsigned int uint;

typedef struct todo_item Item;
struct todo_item {
  char name[ITEM_BUFFER_SIZE];
  uint done;
  uint in_use;
};

Item items[ITEMS_MAX];

void get_word(char *buffer, char* out_buffer, uint len) {
  uint i = 0;
  uint n = 0;

  while (
         i < len &&
         buffer[i] != EOF &&
         isspace(buffer[i]) != 0
         ) {
    i++;
  }

  while (
         i < len &&
         buffer[i] != EOF &&
         isspace(buffer[i]) == 0
         ) {
    out_buffer[n++] = buffer[i++];
  }
  
  out_buffer[n] = '\0';

  return;
}

void items_clear() {
  for (uint i = 0; i < ITEMS_MAX; i++) {
    items[i].name[0] = '\0';
    items[i].done = 0;
    items[i].in_use = 0;
  }
}

void items_add(char* item) {
  for (uint i = 0; i < ITEMS_MAX; i++) {
    if (items[i].in_use == 0) {
      strncpy(items[i].name, item, ITEM_BUFFER_SIZE);
      items[i].done = 0;
      items[i].in_use = 1;

      break;
    }
  }
}

void items_done(char* item) {
  for (uint i = 0; i < ITEMS_MAX; i++) {
    if (
        items[i].in_use == 1 &&
        strncmp(items[i].name, item, ITEM_BUFFER_SIZE) == 0
    ) {
      items[i].done =1;
      break;
    }
  }
}

void items_list() {
  for (uint i = 0; i < ITEMS_MAX; i++) {
    if (items[i].in_use == 1) {
      printf("\tItem: %s\n\tDone: %s\n", items[i].name, items[i].done == 1 ? "yes" : "no");
    }
  }
}

#define PROMPT() printf("todo:> ");

void main() {
  printf("Hello, World!\n");
  char line_buffer[LINE_BUFFER_SIZE];
  char action_buffer[ACTION_BUFFER_SIZE];
  char item_buffer[ITEM_BUFFER_SIZE];

  PROMPT();
  while (fgets(line_buffer, LINE_BUFFER_SIZE, stdin)) {
    //printf(":: %s\n", line_buffer);
    get_word(line_buffer, action_buffer, ACTION_BUFFER_SIZE);
    get_word(line_buffer + strlen(action_buffer), item_buffer, ITEM_BUFFER_SIZE);

    //printf("action: %s\nitem: %s\n", action_buffer, item_buffer);

    if (strncmp(action_buffer, "add", ACTION_BUFFER_SIZE) == 0) {
      //      printf("adding %s to list\n", item_buffer);
      items_add(item_buffer);
    }
    else if (strncmp(action_buffer, "done", ACTION_BUFFER_SIZE) == 0) {
      //printf("marking %s as complete\n", item_buffer);
      items_done(item_buffer);
    }
    else if (strncmp(action_buffer, "list", ACTION_BUFFER_SIZE) == 0) {
      //      printf("listing the list\n");
      items_list();
    }
    else if (strncmp(action_buffer, "clear", ACTION_BUFFER_SIZE) == 0) {
      //      printf("clearing the list\n");
      items_clear();
    }
    else if (strncmp(action_buffer, "quit", ACTION_BUFFER_SIZE) == 0) {
      printf("bye bye\n");
      return;
    }
    else {
      printf("Unknown command: %s\n", item_buffer);
    }

    PROMPT();
  }
  
}
