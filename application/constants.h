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
#define CHAR_COMMA ','

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
#define MESSAGE_PREFIX_POLY_NOT_FOUND "Can't find polynomial named "
#define MESSAGE_FAILED_TO_PARSE_EXPRESSIONS "Failed to parse expression(s)"
#define MESSAGE_TOO_MANY_ARGUMENTS "Too many arguments"
#define MESSAGE_TOO_FEW_ARGUMENTS "Too few arguments"
#define MESSAGE_INVALID_ARGUMENTS "Invalid argument(s)"

namespace app {
enum class OpType { ASSIGN, DISPLAY, EVALUATE, PLOT, UNKNOWN, HELP, EXIT };
enum class TokenTypes { VAR, POLY, OP, BRACE };

enum class CalcOps { ADD, SUB, MUL, DIV };
enum class TokenBrace { L, R };

} // namespace app

#endif /* APPLICATION_INCLUDE_CONTANTS_H_ */

#define POLYNOMIAL_NOT_FOUND_MESSAGE "Error: Polynomial named %s does not exist"
#define AXIS_SCALE_WIDTH 20
#define VERTICAL_AXIS_NUMBER_WIDTH AXIS_SCALE_WIDTH
#define HORIZONTAL_AXIS_NUMBER_WIDTH AXIS_SCALE_WIDTH
#define GRID_INTERIM_X 20
#define GRID_INTERIM_Y 20

// colors
#define SGR_FG_RED "\x1b[38;2;255;0;0m"
#define SGR_FG_GREEN "\x1b[38;2;0;255;0m"
#define SGR_FG_BLUE "\x1b[38;2;0;0;255m"
#define SGR_FG_YELLOW "\x1b[38;2;255;255;0m"
#define SGR_FG_WHITE "\x1b[38;2;255;255;255m"
#define SGR_FG_GREY "\x1b[38;2;200;200;200m"

// ANSI Operators
#define ANSI_BACKSPACE "\x1b[1D"

// Logger
#define LOGGER_PREFIX_WARNING "[WARNING] "
#define LOGGER_PREFIX_ERROR "[ERROR] "
#define LOGGER_PREFIX_DEBUG "[DEBUG] "
#define LOGGER_PREFIX_INFO "[INFO] "
