#include <stdbool.h>

#ifndef EQUATION_H
#define EQUATION_H

// as the name may suggest, this function recieves a character as input
// and returns whether its a math operation or a number
//
// input: character (char) to test
// output: bool - whether it is as name implies or not
bool is_char_digit(char character);
bool is_char_operand(char character);

// function recieves an equation string as input, returns whether its legal or not.
// rules the equation must abide:
//   1. only allowed chars are 0-9, +, -, *, /, ( and ). (not even \n)
//   2. no digit will be before '(', no operand will be before ')'
//   3. trailing operands ('+-','**', etc) are not allowed.
//
// input: equation (char *) to be verified
// output: whether the equation follows the guidelines as stated above
bool equation_verify(char *equation);

// function recieves an equation string as input, returns the result of said equation
// nothing stops you from using these functions 
// caveats:
//   function will return an __integer__ value as of the current state.
//   function may require more memory than it may actually need at current state...
//
// input: equation (char *) to be solved
// output: the result of the equation
int equation_solve(char *equation);

#endif
