#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define LINE_BUFFER_SIZE 256
#define ACTION_BUFFER_SIZE 8
#define ITEM_BUFFER_SIZE ((LINE_BUFFER_SIZE) - (ACTION_BUFFER_SIZE))
#define ITEMS_MAX 128

typedef struct todo_item Item;

struct todo_item {
  char name[ITEM_BUFFER_SIZE];
  int complete;
  int in_use;
};

Item list[ITEMS_MAX];

void list_clear() {
  for (unsigned int i = 0; i < ITEMS_MAX; i++) {
    list[i].name[0] = '\0';
    list[i].complete = 0;
    list[i].in_use = 0;
  }
}

void list_add(char *item) {
  for (unsigned int i = 0; i < ITEMS_MAX; i++) {
    if (list[i].in_use == 0) {
      strncpy(list[i].name, item, ITEM_BUFFER_SIZE);
      list[i].complete = 0;
      list[i].in_use = 1;

      return;
    }
  }

  printf("No vacant spaces in list. it must be full\n");
}

void list_list() {
   for (unsigned int i = 0; i < ITEMS_MAX; i++) {
     if (list[i].in_use != 0) {
       printf("\t%s. complete: %s\n", list[i].name, list[i].complete == 1 ? "yes" : "no");
     }
   }
}

void list_done(char *item) {
  for (unsigned int i = 0; i < ITEMS_MAX; i++) {
    if (
        list[i].in_use == 1 &&
        strncmp(list[i].name, item, ITEM_BUFFER_SIZE) == 0
    ) {
      list[i].complete = 1;
      return;
    }
  }
  printf("%s was not found in the list\n", item);
}

void get_word(char* buffer, char* out_buffer, unsigned int len) {
  unsigned int i = 0;
  unsigned int n = 0;

  while(
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

#define PROMPT() printf("todo:> ");

void main() {
  //  printf("Hello, World!\n");

  char line_buffer[LINE_BUFFER_SIZE];
  char action_buffer[ACTION_BUFFER_SIZE];
  char item_buffer[ITEM_BUFFER_SIZE];

  list_clear();
  PROMPT();
  while (fgets(line_buffer, LINE_BUFFER_SIZE, stdin)) {
    //    printf("you said: %s\n", line_buffer);

    get_word(line_buffer, action_buffer, ACTION_BUFFER_SIZE);
    size_t action_len = strlen(action_buffer);
    get_word(line_buffer + action_len, item_buffer, ITEM_BUFFER_SIZE);

    //    printf("action: %s, item: %s\n", action_buffer, item_buffer);

    if (strncmp(action_buffer, "quit", ACTION_BUFFER_SIZE) == 0) {
      //      printf("goodbye\n");
      break;
    }
    else if (strncmp(action_buffer, "add", ACTION_BUFFER_SIZE) == 0) {
      //      printf("adding (%s) to the list\n", item_buffer);
      list_add(item_buffer);
    }
    else if (strncmp(action_buffer, "list", ACTION_BUFFER_SIZE) == 0) {
      //      printf("Listing the list\n");
      list_list();
    }
    else if (strncmp(action_buffer, "clear", ACTION_BUFFER_SIZE) == 0) {
      //      printf("clearing the list\n");
      list_clear();
    }
    else if (strncmp(action_buffer, "done", ACTION_BUFFER_SIZE) == 0) {
      //      printf("marking (%s) as complete\n", item_buffer);
      list_done(item_buffer);
    }
    else {
      printf("unknown command: %s\n", action_buffer);
    }
    
    PROMPT();
  }

}

