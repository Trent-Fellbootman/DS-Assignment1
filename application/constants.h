/*
 * tokens.h
 *
 *  Created on: Apr 17, 2022
 *      Author: zbc
 */

#ifndef APPLICATION_INCLUDE_CONTANTS_H_
#define APPLICATION_INCLUDE_CONTANTS_H_

#define LEFT_BRACE '('
#define RIGHT_BRACE ')'
#define WHITE_SPACE ' '
#define LEFT_BRACKET '{'
#define RIGHT_BRACKET '}'

#define PLUS_CH '+'
#define MINUS_CH '-'
#define MULTIPLY_CH '*'
#define DIVIDE_CH '/'

#define OP_ASSIGN "assign"
#define OP_DISPLAY "disp"
#define OP_EVALUATE "eval"
#define OP_PLOT "plot"
#define OP_HELP "help"
#define OP_EXIT "exit"

#define INVALID_COMMAND_MESSAGE "Command is invalid (Type \"help()\" for help)."
#define HELPER_MESSAGE "TODO"
#define UNKNOWN_OP_MESSAGE "TODO"

namespace app {
enum class OpType { ASSIGN, DISPLAY, EVALUATE, PLOT, UNKNOWN, HELP, EXIT };
enum class TokenTypes { VAR, POLY, OP };

enum class CalcOps { ADD, SUB, MUL, DIV };

} // namespace app

#endif /* APPLICATION_INCLUDE_CONTANTS_H_ */

#define POLYNOMIAL_NOT_FOUND_MESSAGE "Error: Polynomial named %s does not exist"
#define VERTICAL_AXIS_NUMBER_WIDTH 20
#define HORIZONTAL_AXIS_NUMBER_WIDTH 20
#define GRID_INTERIM_X 20
#define GRID_INTERIM_Y 20
