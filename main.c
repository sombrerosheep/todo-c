#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
  features:
    - synonyms: add->a, exit->quit etc...
    - multiple lists
    - multi word items
*/

#define MAX_TODO_ITEMS 10
#define MAX_ITEM_LEN 256

typedef struct item_t Item;

struct item_t {
  char name[MAX_ITEM_LEN];
  int complete;
  int vacant;
};

Item items[MAX_TODO_ITEMS];

void items_init() {
  for (int i = 0; i < MAX_TODO_ITEMS; i++) {
      items[i].vacant = 1;
      items[i].complete = 0;
      items[i].name[0] = '\0';
    }
}

typedef struct input_t Input;
typedef enum   action_t Action;

enum action_t {
  ACTION_ADD = 0,
  ACTION_DONE,
  ACTION_LIST,
  ACTION_CLEAR,
  ACTION_HELP,
  ACTION_QUIT,
  ACTION_COUNT
};

const char* const action_names[] = {
  "add", // ACTION_ADD
  "done", // ACTION_DONE
  "list", // ACTION_LIST
  "clear", // ACTION_CLEAR
  "help", // ACTION_QUIT
  "exit", // ACTION_QUIT
};

Action Action_parse(const char* buffer) {
  for (unsigned int i = 0; i < ACTION_COUNT; i++) {
    if (strcmp(buffer, action_names[i]) == 0) {
      return (Action)i;
    }
  }

  return ACTION_COUNT;
}

struct input_t {
  Action action;
  char item[MAX_ITEM_LEN];
};

void print_help() {
  printf("todo <action> [options]\n");
  printf("\n");
  printf("actions:\n");
  printf("\tadd <name>: adds an item (name) to the list\n");
  printf("\tdone <name>: mark the item (name) as complete\n");
  printf("\tlist: list all items in the list\n");
  printf("\tclear: clear all items in the list\n");
  printf("\n");
}

void item_print(Item item) {
  if (item.vacant == 0) {
    printf("\titem: %s, complete: %s\n", item.name, item.complete ? "yes" : "no");
  }
}

void action_add(const char* item) {
  for (int i = 0; i < MAX_TODO_ITEMS; i++) {
    if (items[i].vacant == 1) {
      strncpy(items[i].name, item, MAX_ITEM_LEN);
      items[i].complete = 0;
      items[i].vacant = 0;

      break;
    }
  }
}

void action_done(const char* item) {
  for (int i = 0; i < MAX_TODO_ITEMS; i++) {
    if (items[i].vacant == 1) {
      continue;
    }

    if (strncmp(items[i].name, item, MAX_ITEM_LEN) == 0) {
      items[i].complete = 1;

      return;
    }
  }

  printf("done> item %s is not in the list\n", item);
}

void action_list() {
  for (int i = 0; i < MAX_TODO_ITEMS; i++) {
    item_print(items[i]);
  }
}

void input_process(Input input) {
  switch (input.action) {
    case ACTION_ADD: {
      // printf("ADD> adding %s to list\n", input.item);
      action_add(input.item);
      break;
    }
    case ACTION_DONE: {
      // printf("DONE> completing %s on list\n", input.item);
      action_done(input.item);
      break;
    }
    case ACTION_LIST: {
      // printf("LIST> listing the list\n");
      action_list();
      break;
    }
    case ACTION_CLEAR: {
      // printf("CLEAR> clearing the list\n");
      items_init();
      break;
    }
    case ACTION_HELP: {
      // printf("HELP> giving assistance\n");
      print_help();
      break;
    }
    case ACTION_QUIT: {
      // printf("QUIT> bye bye\n");
      break;
    }
    default: {
      printf("invalid action provided\n");
    }
  }
}

void get_word(char* buffer, char* out_buffer, unsigned int max_len) {
  unsigned int i = 0, j = 0;

  // skip leading spaces
  while(i < max_len && isspace(buffer[i])) {
    // printf("skipping space: %u, '%c'\n", i, buffer[i]);
    i++;
  }

  while(i < max_len && isspace(buffer[i]) == 0 && buffer[i] != EOF) {
    // printf("adding: %c\n", buffer[i]);
    out_buffer[j] = tolower(buffer[i]);

    i++;
    j++;
  }

  out_buffer[j] = '\0';
  // printf("ending string: '%s'\n", out_buffer);
}

#define MAX_ACTION_LENGTH 8
Input parse_input(char* buffer, unsigned int max_len) {
  char word[MAX_ACTION_LENGTH];
  char item[MAX_ITEM_LEN];
  Input input;
  input.action = ACTION_COUNT;

  get_word(buffer, (char*)word, max_len);
  input.action = Action_parse(word);

  unsigned int word_len = strnlen(word, max_len);
  unsigned int item_max_len = max_len - word_len;
  char* word_start = buffer + strlen(word);
  // printf("buffer start: %p\nword len: %u\nitem start: %p\n", buffer, word_len, word_start);

  get_word(word_start, (char*)item, item_max_len);
  // printf("parsed item: %s\n", item);

  strncpy(input.item, item, MAX_ITEM_LEN);
  
  return input;
}

void debug_print_input(Input input) {
  if ((unsigned int)input.action >= ACTION_COUNT) {
    return;
  }

  const char* action_name = action_names[(unsigned int)input.action];
  printf("Action:\n");
  printf("\tName: %s\n", action_name);
  printf("\tItem: %s\n", input.item);
}

#define PRINT_LINE_READY() printf("todo> ");

int main(int argc, char** argv) {
  printf("Hello, World!\n");
  // print_help();
  items_init();

  const unsigned int STREAM_BUFFER_SIZE = 256;
  char buffer[STREAM_BUFFER_SIZE];

  printf("what ya got?\n");
  PRINT_LINE_READY();

  while(fgets((char*)buffer, STREAM_BUFFER_SIZE, stdin)) {
    unsigned int buflen = strnlen(buffer, STREAM_BUFFER_SIZE);
    // printf("input length: %u\n", buflen);
    Input input = parse_input(buffer, buflen);

    // debug_print_input(input);
    if (input.action == ACTION_QUIT) {
      break;
    }
    input_process(input);
    
    PRINT_LINE_READY();
  }

  return 0;
}
