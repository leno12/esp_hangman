#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
typedef struct _Hangman_
{
  int tries;
  char* word_checker;
  char* word;
  struct _Hangman_* next_word;
} hangman;



void save_new_word(hangman** first_word, char* new_word)
{
  if(*first_word == NULL)
  {
    int len = strlen(new_word) + 1;
    *first_word = (hangman*)malloc(sizeof(hangman));
    (*first_word)->word = (char*)malloc(sizeof(char) * len);
    strcpy((*first_word)->word, new_word);
    (*first_word)->word_checker = (char*)malloc(sizeof(char) * len);
    (*first_word)->word_checker[0] = (*first_word)->word[0];
    (*first_word)->tries = 0;
    for(int i = 1; i < len - 1; i++)
    {
      (*first_word)->word_checker[i] = '_';
    }
    (*first_word)->word_checker[len-1] = 0;
    (*first_word)->next_word = NULL;
  }
  else
  {
    hangman* create_new_word = (hangman*)malloc(sizeof(hangman));
    int len = strlen(new_word) + 1;
    create_new_word->word = (char*)malloc(sizeof(char) * len);
    strcpy(create_new_word->word, new_word);
    create_new_word->word_checker = (char*)malloc(sizeof(char) * len);
    create_new_word->word_checker[0] = create_new_word->word[0];
    create_new_word->tries = 0;
    create_new_word->next_word = NULL;
    for(int i = 1; i < len - 1; i++)
    {
      create_new_word->word_checker[i] = '_';
    }
    create_new_word->word_checker[len-1] = 0;

    hangman* save_first_word = *first_word;
    while(save_first_word->next_word != NULL)
    {
      save_first_word = save_first_word->next_word;
    }
    save_first_word->next_word = create_new_word;

  }

}

void freeMemory(hangman** word)
{
  while(*word != NULL)
  {
    free((*word)->word);
    free((*word)->word_checker);
    hangman* temp = *word;
    *word = (*word)->next_word;
    free(temp);
  }
}

int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    printf("usage: %s filename\n", argv[0]);
    return 1;
  }
  char* file_name = argv[1];

  FILE *fp = fopen(file_name, "r");
  if(fp == NULL)
  {
    printf("ERROR: cannot open file %s\n", file_name);
    return 2;
  }
  char parse_line[255];
  char save_word[255];
  int words_count = 0;
  int counter = 0;


  hangman* first_word = NULL;

  while(fgets(parse_line,255,fp) != NULL)
  {
    if(parse_line[0] == '\n')
      continue;
    words_count = 0;
    counter = 0;
    memset(save_word,0,255);
    for(int i = 0; i < strlen(parse_line) - 1; i++)
    {
      if(parse_line[i] == ' ' || parse_line[i] == '\t')
        continue;
      if(parse_line[i] != ' ')
      {
        words_count++;
        for(int j = i; j < strlen(parse_line); j++)
        {
          if(parse_line[j] == ' ')
          {
            i = j;
            break;
          }

          save_word[counter] = parse_line[j];
          counter++;
          i = j;
        }
        
        
      }
    
      if(words_count > 1)
        {
          freeMemory(&first_word);
          printf("ERROR: file %s invalid\n", file_name);
          fclose(fp); 
          return 3;
        }
      

    }
    if(save_word[strlen(save_word)-1] == '\n')
      save_word[strlen(save_word)-1] = 0;

    save_new_word(&first_word, save_word);
    
  }
  fclose(fp);
  int total_num = 0;
  int correct_answers = 0;
  while(first_word != NULL)
  {
    if(strlen(first_word->word) == 1)
    {
      correct_answers++;
      total_num++;
      free(first_word->word);
      free(first_word->word_checker);
      hangman* temp = first_word;
      first_word = first_word->next_word;
      free(temp);
      continue;
    }
    int guessed = 0;
    printf("%s (%d)\n", first_word->word_checker, first_word->tries);
    printf("Your guess: ");
    char answer; 
    while((answer = getchar()) != '\n' && answer != EOF)
    {
      for(int i = 0; i < strlen(first_word->word_checker); i++)
      {
        if(tolower(first_word->word[i]) != tolower(answer))
          continue;

        first_word->word_checker[i] = first_word->word[i];
        guessed = 1;
      }
      if(!guessed)
      {
        first_word->tries++;
        if(first_word->tries >= 11)
        {
          total_num++;
          printf("%s (x_x)\n", first_word->word);
          free(first_word->word);
          free(first_word->word_checker);
          hangman* save_word_temp = first_word;
          first_word = first_word->next_word;
          free(save_word_temp);
        }
      }
      else if(strcmp(first_word->word, first_word->word_checker) == 0)
      {
        printf("%s (%d)\n", first_word->word_checker, first_word->tries);
        total_num++;
        free(first_word->word);
        free(first_word->word_checker);
        hangman* save_word_temp = first_word;
        first_word = first_word->next_word;
        correct_answers++;
        free(save_word_temp);

      }

    }
    
    
  }
  
    printf("won (%d/%d)\n", correct_answers, total_num);

  return 0;
}