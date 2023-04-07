```
////////////////////////////////////////////////////////////////////////////////////
//!                             Declarations
////////////////////////////////////////////////////////////////////////////////////


<file> ::= <declaration>*

<declaration> ::= <function-definition>
                | <variable-declaration>

<function-declaration> ::= <signature> fun <identifier> <parameter-list> = <expression> ;

<variable-declaration> ::= <signature> var <identifier> = <expression> ;


////////////////////////////////////////////////////////////////////////////////////
//!                             Expressions
////////////////////////////////////////////////////////////////////////////////////


<expression> ::= <equality-expression>

<equality-expression> ::= <relational-expression>
                        | <equality-expression> == <relational-expression>
                        | <equality-expression> != <relational-expression>

<relational-expression> ::= <shift-expression>
                          | <relational-expression> < <shift-expression>
                          | <relational-expression> > <shift-expression>

<shift-expression> ::= <additive-expression>

<additive-expression> ::= <multiplicative-expression>
                        | <additive-expression> + <multiplicative-expression>
                        | <additive-expression> - <multiplicative-expression>

<multiplicative-expression> ::= <unary-expression>
                              | <multiplicative-expression> * <unary-expression>
                              | <multiplicative-expression> / <unary-expression>

<unary-expression> ::= <postfix-expression>
                     | <unary-operator> <unary-expression>

<unary-operator> ::= -
                   | !

<postfix-expression> ::= <primary-expression>
                       | <postfix-expression> ( {<expression>,}* )    // <------- TODO: implement

<primary-expression> ::= <identifier>
                       | <constant>
                       | <keyword-expression>
                       | <compound-expression>
                       | ( <expression> )

<compound-expression> ::= { {<declaration> | <statement>}* } // <------- deleted tail expression

<keyword-expression> ::= return <expression>
                       | yield <expression>
                       | <selection-expression>

<selection-expression> ::= if <expression> then? <expression>
                         | if <expression> then? <expression> else <expression>

<constant> ::= <integer-constant>
             | <string>
             
             
// Typing 

<signature> ::= of <type>

<type> ::= <simple-type> # Simple type
         | [<type>, ...] -> <type> # Function type

<simple-type> ::=
                     | * <simple-type>
                     | <primitive-type>
                     
<primitive-type> ::= Int | Bool | String | Char | Unit | () | !

////////////////////////////////////////////////////////////////////////////////////
//!                             Statements
////////////////////////////////////////////////////////////////////////////////////


<statement> ::= <expression-statement>
              | <assignment-statement>

<expression-statement> ::= <expression> ;

<assignment-statement> ::= <unary-expression> = <expression> ;


////////////////////////////////////////////////////////////////////////////////////
```