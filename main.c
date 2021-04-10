#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define PROMPT() printf("todo> ");

#define LINE_BUFFER_SIZE 128
#define ACTION_BUFFER_SIZE 16
#define ITEM_BUFFER_SIZE 64
#define TODO_LIST_SIZE 64

int running = 1;

typedef struct item Item;

struct item {
  char name[ITEM_BUFFER_SIZE];
  int complete;
  int vacant;
};

Item list[TODO_LIST_SIZE];

void list_add(char *item, unsigned int len) {
  for (unsigned int i = 0; i < TODO_LIST_SIZE; i++) {
    if (list[i].vacant == 1) {
      list[i].vacant = 0;
      list[i].complete = 0;
      strncpy(list[i].name, item, len);

      return;
    }
  }

  printf("list is full!\n");
}

void list_list() {
  for (unsigned int i = 0; i < TODO_LIST_SIZE; i++) {
    if (list[i].vacant == 0) {
      printf("\tItem: %s, complete: %s\n", list[i].name, list[i].complete ? "yes" : "no");
    }
  }
}

void list_clear() {
  for (unsigned int i = 0; i < TODO_LIST_SIZE; i++) {
    list[i].vacant = 1;
    list[i].complete = 0;
    list[i].name[0] = '\0';
  }
}

void list_done(char *item, unsigned int len) {
  for (unsigned int i = 0; i < TODO_LIST_SIZE; i++) {
    // printf("comparing: '%s' with '%s' ?? %d\n", list[i].name, item, strncmp(list[i].name, item, len));
    if (list[i].vacant == 0 && strncmp(list[i].name, item, len) == 0) {
      list[i].complete = 1;

      return;
    }
  }

  printf("'%s' was not in the list!\n", item);
}

void print_help() {
  printf("todo> [action] [item]\n");
  printf("\n");
  printf("actions:\n");
  printf("  add: adds item to the list\n");
  printf("  list: list each item in list\n");
  printf("  clear: clears the list\n");
  printf("  done: marks item complete\n");
  printf("  quit: exit\n");
  printf("\n");
}

void input_act(char* action, char *item) {
  if (strncmp(action, "add", strlen(action)) == 0) {
    // printf("adding: '%s' to list\n", item);
    list_add(item, strlen(item));
  } else if (strncmp(action, "list", strlen(action)) == 0) {
    // printf("listing the list\n");
    list_list();
  } else if (strncmp(action, "clear", strlen(action)) == 0) {
    // printf("clearing the list\n");
    list_clear();
  } else if (strncmp(action, "done", strlen(action)) == 0) {
    // printf("marking '%s' as done list\n", item);
    list_done(item, strlen(item));
  } else if (strncmp(action, "quit", strlen(action)) == 0) {
    // printf("quitting\n");
    running = 0;
  } else {
    printf("Unknown input: %s\n", action);
    print_help();
  }
}

void get_word(char *buffer, char *out, unsigned int len) {
  unsigned int i = 0,
               n = 0;

  while (i < len && buffer[i] != EOF && isspace(buffer[i])) {
    i++;
  }

  while (i < len && buffer[i] != EOF && !isspace(buffer[i])) {
    out[n++] = buffer[i++];
  }

  if (n >= len) {
    out[len - 1] = '\0';
  } else {
    out[n] = '\0';
  }

  return;
}

void main() {
  printf("Hello, World!\n");

  char buffer[LINE_BUFFER_SIZE];

  list_clear();

  PROMPT();
  while(running && fgets(buffer, LINE_BUFFER_SIZE, stdin)) {
    char action[ACTION_BUFFER_SIZE];
    char item[ITEM_BUFFER_SIZE];
    // printf("you entered: %s\n", buffer);
    get_word(buffer, (char*)action, ACTION_BUFFER_SIZE);
    get_word(buffer + strlen(action), (char*)item, ITEM_BUFFER_SIZE);

    // printf("Action: %s, Item: %s\n", action, item);
    // if (strlen(action) > )
    if (strlen(action) > 0) {
      input_act(action, item);
    }

    if(running) {
      PROMPT();
    }
  }
}