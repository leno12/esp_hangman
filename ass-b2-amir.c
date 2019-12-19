#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define ERROR_OUT_OF_MEMORY "ERROR: Out of Memory\n"
#define ERROR_WRONG_COMMAND_PARAM "usage: %s filename\n", argv[0]
#define ERROR_INVALID_CONFIG "ERROR: file %s invalid\n", argv[1]
#define ERROR_CANT_OPEN_FILE "ERROR: cannot open file %s\n", argv[1]
#define BUFFER_SIZE 255
#define INVALID_CONFIG_RET_VALUE 3
#define CANT_OPEN_FILE_RET_VALUE 2
#define WRONG_COMMAND_PARAM_RET_VALUE 1
#define OUT_OF_MEMORY_RET_VALUE 4
#define SUCCESS_RET_VALUE 0
#define MAX_GUESSES 11
#define MAX_WORDS_PER_LINE 1

typedef struct _Words_ {
    int num_of_tries;
    char* current_word;
    struct _Words_* next;
} words;


int addWord(words** head, char* new_word)
{

    words* add_word;
    words* save_head;
    add_word = malloc(sizeof(words));
    if(add_word == NULL)
    {
      printf(ERROR_OUT_OF_MEMORY);
      return OUT_OF_MEMORY_RET_VALUE;
    }
    add_word->num_of_tries = 0;
    add_word->current_word = malloc((strlen(new_word) + 1) * sizeof(char));
    if(add_word->current_word == NULL)
    {
      printf(ERROR_OUT_OF_MEMORY);
      return OUT_OF_MEMORY_RET_VALUE;
    }
    strncpy(add_word->current_word, new_word, strlen(new_word) + 1);
    add_word->next = NULL;
    save_head = *head;
    if(*head == NULL)
    {
        *head = add_word;
    }
    else
    {
      while(save_head->next != NULL)
      {
        save_head = save_head->next;
      }
      save_head->next = add_word;
    }
  return SUCCESS_RET_VALUE;
}

void freeMemory(words** head)
{
  while(*head != NULL)
  {
    free((*head)->current_word);
    words* temp = *head;
    *head = (*head)->next;
    free(temp);
  }
}

int readFile(FILE **fp, words** word)
{
  char line[BUFFER_SIZE];
  while (fgets(line, BUFFER_SIZE, *fp) != NULL)
  {
    if(strcmp(line, "\n") == 0)
    {
      continue;
    }
    int length = strlen(line);
    if(line[length - 1] == '\n')
    {
      line[length - 1] = 0;
    }

    char *ptr = strtok(line, " \t");
    int words_counter = 0;
    char* current_word;
   

    while(ptr != NULL)
    {

      if(*ptr == ' ')
      {
        ptr = strtok(NULL, " \t");
        continue;
      }

      words_counter++;
      if(words_counter > MAX_WORDS_PER_LINE)
      {
        freeMemory(word);
        fclose(*fp);
        return INVALID_CONFIG_RET_VALUE;
      }
      current_word = ptr;
      ptr = strtok(NULL, " ");
    }

     if(addWord(word, current_word) == OUT_OF_MEMORY_RET_VALUE)
     {  
        freeMemory(word);
        return OUT_OF_MEMORY_RET_VALUE;
     }

  }
  return SUCCESS_RET_VALUE;
}
int main(int argc, char** argv)
{
  if(argc != 2)
  {
    printf(ERROR_WRONG_COMMAND_PARAM);
    return WRONG_COMMAND_PARAM_RET_VALUE;
  }
  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL)
  {
    printf(ERROR_CANT_OPEN_FILE);
    return CANT_OPEN_FILE_RET_VALUE;
  }
 words* word = NULL;
 int ret_value = readFile(&fp, &word);
 if(ret_value == INVALID_CONFIG_RET_VALUE)
 {
   printf(ERROR_INVALID_CONFIG);
   return INVALID_CONFIG_RET_VALUE;
 }
 else if(ret_value == OUT_OF_MEMORY_RET_VALUE)
 {
  return OUT_OF_MEMORY_RET_VALUE;
 }

 fclose(fp);
 int rounds = 0;
 int correct_words = 0;
  while(word != NULL)
  {
    rounds++;

    char *temp = malloc((strlen(word->current_word) + 1) * sizeof(char));
    if(temp == NULL)
    {
      freeMemory(&word);
      printf(ERROR_OUT_OF_MEMORY);
      return OUT_OF_MEMORY_RET_VALUE;
    }
    temp[0] = word->current_word[0];
    temp[strlen(word->current_word)] = '\0';

    for(int i = 1; word->current_word[i] != '\0'; i++)
    {
      temp[i] = '_';
    }
   
    while(strcmp(temp, word->current_word) != 0)
    {
      char guess[255];
      printf("%s (%d)\n", temp, word->num_of_tries);
      printf("Your guess: ");
      fgets(guess, sizeof(guess), stdin);
      if(guess[0] == '\n')
        continue;

      int correct = 0;
      for(int i = 0;  word->current_word[i] != '\0'; i++)
      {
        if(tolower(guess[0]) == tolower(word->current_word[i]))
        {
          correct = 1;
          temp[i] = word->current_word[i];
        }
      }

      if(!correct)
        word->num_of_tries++;

      if(word->num_of_tries == MAX_GUESSES)
      {
        printf("%s (x_x)\n", word->current_word);
        break;
      }
      if(strcmp(temp, word->current_word) == 0)
      {
        printf("%s (%d)\n",word->current_word, word->num_of_tries);
        correct_words++;
      }
    }
    if(strlen(word->current_word) == 1)
    {
      correct_words++;
    }
    free(temp);
    free(word->current_word);
    words* save = word; 
    word = word->next;
    free(save);
  }
  printf("won (%d/%d)\n", correct_words, rounds);

  return SUCCESS_RET_VALUE;    
}