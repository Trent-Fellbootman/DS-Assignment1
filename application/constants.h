#include <vector>

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
#define OP_DELETE "delete"
#define OP_HELP "help"
#define OP_EXIT "exit"
#define OP_SET_PROPERTY "set"
#define OP_CLEAR_VARIABLES "clear"
#define OP_CLEAR_CONSOLE "clc"

#define MIN_CANVAS_WIDTH 5
#define MIN_CANVAS_HEIGHT 5

#define INVALID_COMMAND_MESSAGE "Command is invalid (Type \"help()\" for help)."
#define HELPER_MESSAGE R"(EOF)USAGE: <commandName>(arg1, arg2, ..., argn)

All commands or arguments are case-sensitive.
White space between function name and parenthesis is not allowed.
However, using white space to separate arguments and/or parts of expressions is allowed.

Supported commands:
  assign(<polynomialName>, <expression>)
    Creates a new polynomial with the given name and expression, or assign a new polynomial to an existing name.
    Example: assign(a, b + {3} * {x^2 + 1 + 2x})
    Expressions can include polynomial names (names that already have polynomials assigned to) and/or polynomial literals.
    Polynomials literals are delimited with "{" and "}". For example: "{1 + x^2}"
    Note that polynomial multiplications are not yet supported. For example, "{3} * {1 + x^2}" is allowed, but "{1 + x} * {1 - x}" is not.
    
  disp()
    Displays all existing polynomials.

  plot(<polynomialName>, <xMin>, <xMax>)

  set(<propertyName>, <propertyValueArgument1>, ..., <propertyValueArgumentN>)
    Set environment variables (properties).
    Supported properties:
      RGB-Types:
        Arguments for setting these properties are formatted as <r>, <g>, <b>, with each value ranging from 0 to 255.
        Example: set(OUTPUT_COL, 255, 0, 0)
        Properties of this type include:
          OUTPUT_COL: Color of the non-graphical program output (this does not apply to colors used when showing warnings/errors);
          INPUT_COL: Color of the user input;
          PLOT_COL: The color of the curve when plotting polynomials.
      
      Numerical Types:
        Arguments for settings these properties are formatted as <value> (both integers and decimal values).
        Example: set(CANV_W, 100)
        Properties of this type include:
          CANV_W: Width of the canvas (used when plotting polynomials), in number of characters, integer;
          CANV_H: Height of the canvas, in number of characters, integer.

      Bool Types:
        Arguments for setting these properties are formatted as <ON/OFF>.
        Example: set(GRID, ON)
        Properties of this type include:
          GRID: Whether or not to show the grid when plotting polynomials. Default: OFF.


  delete(<polynomialName>)
    Deletes the polynomial with the given name.

  clear()
    Clears all existing polynomials.

  clc()
    Clears the console.

  exit()
    Exits the program.
(EOF)"
#define UNKNOWN_OP_MESSAGE "TODO"
#define MESSAGE_PREFIX_POLY_NOT_FOUND "Can't find polynomial named "
#define MESSAGE_FAILED_TO_PARSE_EXPRESSIONS "Failed to parse expression(s)"
#define MESSAGE_TOO_MANY_ARGUMENTS "Too many arguments"
#define MESSAGE_TOO_FEW_ARGUMENTS "Too few arguments"
#define MESSAGE_INVALID_ARGUMENTS "Invalid argument(s)"
#define MESSAGE_INVALID_OPERATION "Invalid operation"
#define MESSAGE_CANNOT_DIVIDE                                                  \
  "Division is not allowed in polynomial operations. Use multiplications "     \
  "only."
#define MESSAGE_PREFIX_CANVAS_WIDTH_TOO_SMALL "Canvas width must be at least "
#define MESSAGE_PREFIX_CANVAS_HEIGHT_TOO_SMALL "Canvas height must be at least "
#define MESSAGE_PREFIX_UNKNOWN_PROPERTY "Unknown property: "
#define MESSAGE_PREFIX_UNKNOWN_OPTION "Unknown option: "
#define DEFAULT_PROMPT "--> "

#define PROPERTY_PROMPT "PROMPT"
#define PROPERTY_OUTPUT_COLOR "OUTPUT_COL"
#define PROPERTY_INPUT_COLOR "INPUT_COL"

namespace app {
enum class OpType {
  ASSIGN,
  DISPLAY,
  EVALUATE,
  PLOT,
  DELETE,
  UNKNOWN,
  HELP,
  EXIT,
  SET_PROPERTY,
  CLEAR_VARIABLES,
  CLEAR_CONSOLE
};

struct Color {
  std::vector<int> rgb;

  Color(int r, int g, int b) { rgb = {r, g, b}; }
};

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
#define GRID_INTERIM_Y 5

// colors
#define SGR_FG_RED "\e[38;2;200;0;0m"
#define SGR_FG_GREEN "\e[38;2;0;200;0m"
#define SGR_FG_BLUE "\e[38;2;0;0;200m"
#define SGR_FG_YELLOW "\e[38;2;200;200;0m"
#define SGR_FG_WHITE "\e[38;2;255;255;255m"
#define SGR_FG_GREY "\e[38;2;200;200;200m"

// ANSI Operators
#define ANSI_BACKSPACE "\e[1D"
#define ANSI_CLEAR_CONSOLE "\e[2J"

// Logger
#define LOGGER_PREFIX_WARNING "[WARNING] "
#define LOGGER_PREFIX_ERROR "[ERROR] "
#define LOGGER_PREFIX_DEBUG "[DEBUG] "
#define LOGGER_PREFIX_INFO "[INFO] "

// math
#define EPSILON 1e-10
