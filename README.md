# Sunflower programming Language
![logo](sunflower-logo.png)
## Overview
Sunflower is a simple, expressive programming language designed to be easy to learn and fun to use. Sunflower is a dynamic-typed language based in Lua and C; the language is built around a simple syntax and supports a wide range of features such as variable assignment, loops, conditional statements, and function declarations.

This README provides an overview of the Sunflower language's grammar, syntax, and usage. It also includes code examples to demonstrate how Sunflower can be used for various tasks.

## Table of Contents
1. [How to Build](#Build)
2. [Examples](#examples)
3. [Basic Syntax](#basic-syntax)
4. [Variables](#variables)
5. [Control Flow](#control-flow)
6. [Functions](#functions)
7. [Expressions](#expressions)
8. [Statements](#statements)


# Build
## Clone the repository

```bash
git clone https://github.com/hen-mi/sunflower
```
## Dependencies
[GNU GCC](https://gcc.gnu.org/) [CLang](https://clang.llvm.org/) or [MSVC](https://learn.microsoft.com/en-us/cpp/?view=msvc-170);
## Build with [cmake](https://cmake.org/)
```bash
cd sunflower
cmake -S . -B build
cd build
cmake --build .
```


## Basic Syntax
Sunflower follows a context-free grammar, meaning that the syntax of the language is defined by a set of rules for how tokens can be combined to form valid expressions and statements. The syntax is designed to be easy to read and parse, supporting typical programming constructs like conditionals, loops, and function definitions.

### Program Structure
A Sunflower program consists of a series of declarations followed by the end of the file (EOF). Each declaration can either be a function declaration, a variable declaration, or a statement.



## Lexical Tokens

The lexical grammar for Sunflower includes the following tokens:

- **NUMBER**: A number, which is a sequence of digits, possibly with a decimal point (It's always a double value).
- **STRING**: A string, which is enclosed in double quotes and may contain any character except a backslash.
- **IDENTIFIER**: A valid variable or function name, consisting of alphabetic characters and digits (but starting with an alphabetic character or underscore).
- **ALPHA**: Any alphabetic character (lowercase or uppercase) or underscore.
- **DIGIT**: Any numeric digit (0-9).

## Syntax Rules

### Declarations
- **Function Declarations**: Functions are declared using the `fn` keyword followed by the function definition.

# Sintax:
```sunflower
fn functionName(parameters*) block ;
```
Example:

```sunflower

fn greet(name) 
{
    pout("Hello, " + name);
}

greet("Alice");
```

```sunflower

-- This is a comment.
pout("Hello, Sunflower!");

```

- **Variable Declarations**: Variables are declared with the `let` keyword. Optionally, a variable can be initialized with an expression.

# Sintax:
```sunflower
let variableName = expression;
```
Example:
```sunflower

let cost = 25.544;
let string = "This is a string";

```
You can also omit the initialization:


```sunflower
let count;
```

Declaration with expressions:
```sunflower
let sum = 1 + 5;
let result = myFunction();
```
### Control Flow

Sunflower supports several control flow mechanisms:

-**If Statement**:
The `if` statement allows conditional execution of a block of code; the else statement is optional.

Syntax:
```sunflower
let count;
```
Example:
```sunflower
  -> (x > 5) 
{
    pout("x is greater than 5");
} -> 
{
    pout("x is less than or equal to 5");
}
```
-**While Loop**:
The `while` loop repeats a block of code as long as a condition is true.

Syntax:
```latex
while (expression) statement ;
```
Example:

```sunflower
while (x > 0) 
{
    x = x - 1;
    pout(x);
}
```
-**For Loop**:
The `For` loop allows iteration with initialization, condition, and increment expressions.

Syntax:
```latex
for (varDecl | exprStmt | ";") expression? ";" expression? ")" statement ;
```
Example:

```sunflower
for (let i = 0; i < 10; i = i + 1) 
{
    pout(i);
}
```


### Statements
Sunflower supports several types of statements, including:

- **Print Statements**: Outputting the value of an expression to the console.

```sunflower
pout("Printing");
```
- **Return Statements**: Returning a value from a function.


```sunflower
fn max(a, b) 
{
    ->(a > b) return a;

    return b;
}
```

- **Blocks**: A sequence of declarations or statements enclosed in curly braces.
```sunflower
{
    declaration* statement* ;
}
```
Example of nested blocks:
```
fn main() 
{
    let x = 10;

    ->(x > 5)
    {
        pout("x is greater than 5");
    }
}
```

### Expressions
Expressions are used to compute values and can be simple or complex, involving operators, function calls, and more.

## Assignment
You can assign values to variables with the `=` operator:

```sunflower

x = 5;

```
## Logic Operators
Sunflower supports logical `or` and `and` operators:

```sunflower

let isTrue = (x > 5) or (y < 10);
let isFalse = (x == 5) and (y != 10);
```
## Comparison Operators
Sunflower supports comparison operators:

```sunflower

let isEqual = (x == y);
let isGreaterThan = (x > y);
let isSmallerThan = (x < y);
let isGreaterOrEqual = (x >= y);
let isSmallerOrEqual = (x <= y);
```
## Arithmetic Operators
Sunflower supports arithmetic operations, with proper precedence:

```sunflower

let sum = 10 + 5;
let difference = 10 - 5;
let product = 10 * 5;
let quotient = 10 / 5;
let all = ((10+2)/10)* 5-2; -- Result is 4
```
## Unary Operators
You can use the negation (`-`) or logical negation (`!`) operators:

```sunflower

let negative = -x;
let isFalse = !isTrue;
```
### Functions
Functions in Sunflower are defined using the `fn` keyword and may take parameters and return a result.

### Arguments and Parameters
- **Arguments**: Passed to functions during a call.
- **Parameters**: Defined in the function declaration to accept values.

---

# Examples

- **Hello, World!**: 

```sunflower

-- This is a comment.
pout("Hello, Sunflower!");

```

- **Sum Function**:
```sunflower


let x = 10;
let y = 20;

fn sum(a, b) 
{
    return a + b;
}

let result = sum(x, y);
pout(result);

```
- **Factorial Function**:
```sunflower
fn factorial(n) 
{
    ->(n <= 1) 
     {
        return 1;
    }
    return n * factorial(n - 1);
}

let result = factorial(5);
pout(result); -- Outputs: 120

```

To check the full grammar, see grammar.MD