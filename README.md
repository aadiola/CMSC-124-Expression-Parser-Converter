# CMSC-124-Expression-Parser-Converter

This Machine Problem Solution presents an Expression Parser and Converter, and an Expression Evaluator in fulfillment of the requirements of CMSC 124.

## About the Machine Problem

This machine problem is primarily divided into two (2) subproblems. 

The first subproblem is to create a program that evaluates a given expression to see if it is a valid expression or not as well as identify its current notation. If it is valid, the program should also convert the expression to the _other two notations_. For example, if an inputted expression is in infix notation, the program should also output its prefix and postfix notation. If it's in postfix notation, the output should have its equivalent prefix and infix notation.

The second subproblem is to create a program that evaluates a given numerical expression. This takes advantage of the first subproblem solution, albeit, with the grammar modified since this subproblem includes a space character `' '` as a delimiter among tokens.

The solution for the first subproblem is primarily divided into three steps. First, the expression is tokenized. Second, the tokenized expression is parsed using all three grammars of the various notations (i.e. infix, prefix, postfix) to see whether it is a valid expression, and this also notes which notation the expression uses, if it indeed valid. Third, the tokenized expression is converted to its equivalent in the other two notations.

The solution for the second subproblem is primarily done in two steps. First, the expression is converted to postfix notation using the first subproblem solution (albeit modified). Second, the tokens are stored in a stack and evaluated.

## Issues

The converter function implements a simplification process while some of the tests (directly copied from the machine problem file from class which is why I did not modify the test code to make the program pass) are not simplified. This is evident in Test Case 10 and 11 in the prefix to infix tester `ExpressionTester.prefixToInfixTester()`.

## About CMSC 124

CMSC 124, or better known as Design and Implementation of Programming Language, is a theory and programming course offered for Computer Science students in the [University of the Philippines Cebu](upcebu.edu.ph) during their junior year. This course primarily deals with programming languages (PL) and is encompassed by the following topics:

- History and various types of PLs
- Syntax and semantics
- Data Types
- Lexical and Syntax Analysis (First two steps in compilation)
- Parsing
- Bindings, scopes and environments
- Expressions
- Statements, subprograms, and operators
