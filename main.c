#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define LINE_BUFFER_SIZE 256
#define ACTION_BUFFER_SIZE 8
#define ITEM_BUFFER_SIZE (LINE_BUFFER_SIZE - ACTION_BUFFER_SIZE)

#define ITEMS_SIZE 128

typedef struct todo_item Item;

struct todo_item {
  int in_use;
  int complete;
  char name[ITEM_BUFFER_SIZE];
};

Item items[ITEMS_SIZE];

void items_init() {
  for (unsigned int i = 0; i < ITEMS_SIZE; i++) {
    items[i].in_use = 0;
    items[i].complete = 0;
    items[i].name[0] = '\0';
  }

  return;
}

void items_add(char *name) {
  for (unsigned int i = 0; i < ITEMS_SIZE; i++) {
    if (items[i].in_use == 0) {
      strncpy(items[i].name, name, ITEM_BUFFER_SIZE);
      items[i].in_use = 1;
      items[i].complete = 0;

      return;
    }
  }

  printf("Unable to add %s to the list. is it full?", name);
}

void items_list() {
  for (unsigned int i = 0; i < ITEMS_SIZE; i++) {
    if (items[i].in_use != 0) {
      printf("\t%s (%s)\n", items[i].name, items[i].complete == 0 ? "incomplete" : "complete");
    }
  }
}

void items_clear() {
  return items_init();
}

void items_done(char *name) {
  for (unsigned int i = 0; i < ITEMS_SIZE; i++) {
    if (
      items[i].in_use != 0 &&
      strncmp(items[i].name, name, ITEM_BUFFER_SIZE) == 0
    ) {
      items[i].complete = 1;
      printf("%s is complete!\n", name);

      return;
    }
  }

  printf("%s, not present in the list\n", name);
}

void get_word(char *src, char *out, unsigned int len) {
  unsigned int i = 0;
  unsigned int n = 0;

  // skip any leading spaces
  while (i < len && src[i] != EOF && isspace(src[i]) != 0) {
    i++;
  }

  while (i < len && src[i] != EOF && isspace(src[i]) == 0) {
    out[n++] = src[i++];
  }

  out[n] = '\0';

  return;
}

#define PROMPT() printf("todo> ");

void main() {
  printf("Hello, World!\n");
  char line_buffer[LINE_BUFFER_SIZE];
  char action_buffer[ACTION_BUFFER_SIZE];
  char item_buffer[ITEM_BUFFER_SIZE];

  PROMPT();
  while(fgets(line_buffer, LINE_BUFFER_SIZE, stdin)) {
    // printf("you said: %s\n", line_buffer);
    
    get_word(line_buffer, action_buffer, ACTION_BUFFER_SIZE);
    // printf("Action: %s\n", action_buffer);
    
    // get_word(line_buffer + strlen(action_buffer), item_buffer, ITEM_BUFFER_SIZE);
    strncpy(item_buffer, line_buffer + strlen(action_buffer) + 1, ITEM_BUFFER_SIZE);
    item_buffer[strlen(item_buffer) - 1] = '\0';
    // printf("Item is: %s\n", item_buffer);

    if (strncmp(action_buffer, "add", ACTION_BUFFER_SIZE) == 0) {
      // printf("adding (%s) to the list\n", item_buffer);
      items_add(item_buffer);
    } else if (strncmp(action_buffer, "list", ACTION_BUFFER_SIZE) == 0) {
      // printf("listing the list\n");
      items_list();
    } else if (strncmp(action_buffer, "clear", ACTION_BUFFER_SIZE) == 0) {
      // printf("clearing list\n");
      items_clear();
    } else if (strncmp(action_buffer, "done", ACTION_BUFFER_SIZE) == 0) {
      // printf("marking (%s) as done\n", item_buffer);
      items_done(item_buffer);
    } else if (strncmp(action_buffer, "quit", ACTION_BUFFER_SIZE) == 0) {
      printf("buh bye!\n");
      break;
    } else {
      printf("Unknown command: %s\n", action_buffer);
    }

    PROMPT();
  }
}