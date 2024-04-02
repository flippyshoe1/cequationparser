#include "equation.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
  verification process:
  - iterate over the characters in the given string: check if abiding by the following rules:
    1. only allowed chars are 0-9, +, -, *, /, ( and ).
    2. no digit shall come before (, no operand shall come before ).
    3. there will be no trailing operands
  - if any of the rules are broken, return false.
  - otherwise, return true.
 */

bool is_char_digit(char c){
  return ((unsigned int)(c-'0') <= 9);
}

bool is_char_operand(char c){
  switch(c){
  case '+':
  case '-':
  case '*':
  case '/':
    return true;
  default:
    return false;
  }
}

// warning: string must have a null terminator.
bool equation_verify(char *e){
  int openbrackets=0;

  for(char *c=e;*c!='\0';c++){
    if(is_char_digit(*c))
      continue;

    if(is_char_operand(*c)){
      // if we are not in the start and the character before us is an operand
      if(c!=e && is_char_operand(*(c-1))) {
	fprintf(stderr, "equation_verify: trailing operands are illegal\n");
	return false;
      }
      else
	continue;
    }

    // if we have reached here, we can assume the char isnt a number nor an operand
    // hopefully a bracket

    switch(*c){
    case '(':
      // if we are not in the start and the character before us is a number
      if(c!=e&&is_char_digit(*(c-1))){
	fprintf(stderr, "equation_verify: a '(' after a digit is illegal\n");
	return false;
      }
      else {
	openbrackets+=1;
	continue;
      }

    case ')':
      // if we are not in the start and the character before us is an operand
      if(c!=e&&is_char_operand(*(c-1))){
	fprintf(stderr, "equation_verify: a ')' after an operand is illegal\n");
	return false;
      }
      else {
	openbrackets-=1;
	continue;
      }
      
    default:
      // if we reach here, char is invalid
      fprintf(stderr, "equation_verify: invalid character\n");
      return false;
    }
  }

  if(openbrackets==0)
    return true;
  else {
    fprintf(stderr, "equation_verify: some brackets have remained open\n");
    return false;
  }
}

/*
  solving process:
  - the process can be summerized as a printer
    get input, fix input to be simpler until you get nothing but a number (which you will translate into an int)

  - before the process starts the string will be verified to be valid,
    afterwards create a "fix" buffer the same size as the equation string itself
    the fix buffer will be used to keep the new equation, of which after each step of the process will be copied over to the original equation string.
  
  - solving process is split to 3 steps:
    1. resolve brackets
    2. solve multiplication/division
    3. solve addition/subtraction

  - resolve brackets:
    - iterate over the entire string, look for a '('
    - if found, get a pointer to the last ')' in the string
    - recall the function, this time using the string inside the brackets
    
  - solve multiplication/division
    - iterate over the entire string, look for any operand
    - if found, check which one is it
    - if lower order (+,-), copy the string, starting from the last operand found (or the start of the string) all the way to the current operand.
    - if higher order (*,/), dont save the opernad on popernand, keep going until you find a lower order operation
    - after that, copy the entire equation, start to finish, to an equation solver function

  - solve addition/subtraction
    - initialise a result int to be 0, iterate over the entire function
    - every operand you find perform the appropriate action using the previous number with the result int
 */

char *find_last_cb(char *s){
  char *p=NULL;
  for(char *c=s;*c!='\0';c++)
    if(*c==')')
      p=c;

  return p;
}

// this essentially solves an equation disregarding pemdas completely, solving all operations in a straight line.
// you SHOULDNT call this function normally, use equation_solve instead
// i assume the function is called from equation_solve, thus i wont perform any checks
int equation_solve_straight(char *e){
  
  int res=0;
  char *pop = e;
  char *c = e;
  do {
    if(is_char_digit(*c))
      continue;

    if(is_char_operand(*c) || *c=='\0'){
      
      if(pop==e){
	res=atoi(pop);
	pop=c;
	continue;
      }
      
      switch(*pop){
      case '+':
	res+=atoi(pop+1);
	break;
      case '-':
	res-=atoi(pop+1);
	break;
      case '*':
	res*=atoi(pop+1);
	break;
      case '/':
	if(atoi(pop+1)==0){
	  fprintf(stderr, "equation_solve_straight: attempted to divide by 0... why?\n");
	  return -1;
	}
	res/=atoi(pop+1);
	break;
      }
      pop=c;
      continue;
      
    } else {
      fprintf(stderr, "equation_solve_straight: invalid char in equation, is everything alright?\nbad char: %c position: %ld\n", *c, c-e);
      return -1;
    }
  } while (*(c++)!='\0');

  return res;
}

// warning: string e must include a null terminator :3
int equation_solve(char *equation){
  if(!equation_verify(equation)){
    fprintf(stderr, "equation_solve: provided equation is invalid, check error from equation_verify\n");
    return 0;
  }
  
  char e[strlen(equation)];
  strcpy(e, equation);

  fprintf(stdout, "equation_solve: entering %s\n", e);
  
  char fix[strlen(e)];
  char *d=fix;
  
  // step 1: resolve brackets
  for(char *c=e; *c!='\0'; c++)
    if(*c=='('){
      char buff[strlen(e)];
      // adding 1 as padding to not include the brackets themselves
      char *a = stpncpy(buff, c+1, find_last_cb(e)-c-1);
      *a = '\0';
      
      int res = equation_solve(buff);
      
      sprintf(buff, "%d", res);
      d = stpcpy(d, buff);
      c = find_last_cb(e);
      
    } else {
      *d=*c;
      d+=1;
      continue;
    }
  *d='\0';

  strcpy(e, fix);
  d = fix;
  fprintf(stdout, "> resolved equation brackets, new equation: %s\n", e);

  // step 2: solve higher order operations
  bool highorder = false;
  char *poperand = e;
  char *c=e;
  do {
    switch(*c){
    case '\0':
      // ugly hack to make this thing not add the operand too when reaching the null terminator
      // also yes intentional fall through
      //poperand++;
    case '+':
    case '-':
      // yes im treating the null terminator as a lower order operation
      // shut up
      //printf("\nc:%c %d\nd:%c %d\npoperand:%c %d\n",*c, c-e, *d, d-fix, *poperand, poperand-e);
      if(!highorder){
	char *start= (poperand==e) ? poperand : poperand+1;
	size_t len = (poperand==e) ? c-poperand+1 : c-poperand;
	
	d = stpncpy(d, start, len);
      } else {
	highorder = false;
	
	// store the answer of the equation between the lower order operands
	// TODO: i think i can prob make buff smaller
	char buff[strlen(e)];
	char *offset = (poperand==e) ? poperand : poperand+1;
	char *bluff = stpncpy(buff, offset, c-poperand);
	*bluff='\0';
	int res = equation_solve_straight(buff);
	sprintf(buff, "%d", res);

	// copy to fix
	d = stpncpy(d, buff, strlen(e));
	*(d++) = *c;
      }
      poperand=c;
      continue;
      
    case '*':
    case '/':
      if(!highorder)
	highorder = true;
      continue;
      
    default:
      continue;
    }
  } while(*c++!='\0');
  *d='\0';
  fprintf(stdout,"> resolved high order operations, new equation: %s\n", fix);

  // step 3: just solve it.
  int res = equation_solve_straight(fix);
  fprintf(stdout,"> resolved low order operations, final answer: %d\n", res);
  
  return res;
}
