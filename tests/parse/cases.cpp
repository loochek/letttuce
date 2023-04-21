#include <lex/lexer.hpp>
#include <parse/parser.hpp>
#include <parse/parse_error.hpp>
#include <ast/visitors/serialize_visitor.hpp>

// Finally,
#include <catch2/catch.hpp>

#include <iostream>
#include <sstream>

//////////////////////////////////////////////////////////////////////


TEST_CASE("Parser: just works", "[parse]") {
  std::stringstream expr;
  expr << "(1 + 2) * 3 / 7;";

  std::string expected_output = "Expression statement\n"
      "\tBinary expression: /\n"
      "\tLHS:\n"
      "\t\tBinary expression: *\n"
      "\t\tLHS:\n"
      "\t\t\tBinary expression: +\n"
      "\t\t\tLHS:\n"
      "\t\t\t\tLiteral expression: 1\n"
      "\t\t\tRHS\n"
      "\t\t\t\tLiteral expression: 2\n"
      "\t\tRHS\n"
      "\t\t\tLiteral expression: 3\n"
      "\tRHS\n"
      "\t\tLiteral expression: 7\n";

  lex::Lexer lexer(expr);
  parse::Parser parser(lexer);
  ast::Statement* stmt = parser.ParseStatement();

  ast::SerializeVisitor serializer;
  stmt->Accept(&serializer);

  CHECK(serializer.GetSerializedString() == expected_output);
}

TEST_CASE("Parser: complex test", "[parse]") {
  std::stringstream program;
  program << "fun main(argc, argv) = {"
             "    var hello = 6 * (12 + 1);"
             "    if hello == 12 then {"
             "        abobus;"
             "    } else {"
             "        amogus;"
             "    };"
             "    return bebra != 7;"
             "};";

  std::string expected_output = "Function declaration: main\n"
      "Params: argc argv \n"
      "Body:\n"
      "\tBlock expression\n"
      "\tStatement 0:\n"
      "\t\tVariable declaration: hello\n"
      "\t\tInitializer:\n"
      "\t\t\tBinary expression: *\n"
      "\t\t\tLHS:\n"
      "\t\t\t\tLiteral expression: 6\n"
      "\t\t\tRHS\n"
      "\t\t\t\tBinary expression: +\n"
      "\t\t\t\tLHS:\n"
      "\t\t\t\t\tLiteral expression: 12\n"
      "\t\t\t\tRHS\n"
      "\t\t\t\t\tLiteral expression: 1\n"
      "\tStatement 1:\n"
      "\t\tExpression statement\n"
      "\t\t\tIf\n"
      "\t\t\tCondition:\n"
      "\t\t\t\tBinary expression: ==\n"
      "\t\t\t\tLHS:\n"
      "\t\t\t\t\tLiteral expression: hello\n"
      "\t\t\t\tRHS\n"
      "\t\t\t\t\tLiteral expression: 12\n"
      "\t\t\tThen branch:\n"
      "\t\t\t\tBlock expression\n"
      "\t\t\t\tStatement 0:\n"
      "\t\t\t\t\tExpression statement\n"
      "\t\t\t\t\t\tLiteral expression: abobus\n"
      "\t\t\tElse branch:\n"
      "\t\t\t\tBlock expression\n"
      "\t\t\t\tStatement 0:\n"
      "\t\t\t\t\tExpression statement\n"
      "\t\t\t\t\t\tLiteral expression: amogus\n"
      "\tStatement 2:\n"
      "\t\tExpression statement\n"
      "\t\t\tReturn\n"
      "\t\t\tValue (expression):\n"
      "\t\t\t\tBinary expression: !=\n"
      "\t\t\t\tLHS:\n"
      "\t\t\t\t\tLiteral expression: bebra\n"
      "\t\t\t\tRHS\n"
      "\t\t\t\t\tLiteral expression: 7\n";

  lex::Lexer lexer(program);
  parse::Parser parser(lexer);
  ast::Declaration* decl = parser.ParseDeclaration();

  ast::SerializeVisitor serializer;
  decl->Accept(&serializer);

  CHECK(serializer.GetSerializedString() == expected_output);
}

TEST_CASE("Parser: basic errors", "[parse]") {
  std::stringstream expr;
  expr << "{ (1 + 2) * 3 / 7 if kek then true; };";

  lex::Lexer lexer(expr);
  parse::Parser parser(lexer);
  CHECK_THROWS_AS(parser.ParseStatement(), parse::errors::ParseCompoundError);

  std::stringstream expr2;
  expr2 << "if (1 + 2) * 3 / 7";

  lex::Lexer lexer2(expr);
  parse::Parser parser2(lexer2);
  CHECK_THROWS_AS(parser.ParseExpression(), parse::errors::ParseError);
}

TEST_CASE("Parser: whole program", "[parse]") {
  std::stringstream prg;
  prg << "# Complete Lettuce program\n"
         "\n"
         "var global_var = if 12 == 11 + 1 then 14 else 15 + 81;\n"
         "\n"
         "fun magic_calc(a, b, c) = { if (global_var == 14) then return a * b + c; a + b * c; };\n"
         "\n"
         "fun main(argc, argv) = {\n"
         "    return magic_calc(global_var, 12 + 8 / 6, if 1 + 1 == 2 then 13 else 14);\n"
         "};\n";

  std::string expected_output = "Program\n"
      "Declaration 0:\n"
      "\tVariable declaration: global_var\n"
      "\tInitializer:\n"
      "\t\tIf\n"
      "\t\tCondition:\n"
      "\t\t\tBinary expression: ==\n"
      "\t\t\tLHS:\n"
      "\t\t\t\tLiteral expression: 12\n"
      "\t\t\tRHS\n"
      "\t\t\t\tBinary expression: +\n"
      "\t\t\t\tLHS:\n"
      "\t\t\t\t\tLiteral expression: 11\n"
      "\t\t\t\tRHS\n"
      "\t\t\t\t\tLiteral expression: 1\n"
      "\t\tThen branch:\n"
      "\t\t\tLiteral expression: 14\n"
      "\t\tElse branch:\n"
      "\t\t\tBinary expression: +\n"
      "\t\t\tLHS:\n"
      "\t\t\t\tLiteral expression: 15\n"
      "\t\t\tRHS\n"
      "\t\t\t\tLiteral expression: 81\n"
      "Declaration 1:\n"
      "\tFunction declaration: magic_calc\n"
      "\tParams: a b c \n"
      "\tBody:\n"
      "\t\tBlock expression\n"
      "\t\tStatement 0:\n"
      "\t\t\tExpression statement\n"
      "\t\t\t\tIf\n"
      "\t\t\t\tCondition:\n"
      "\t\t\t\t\tBinary expression: ==\n"
      "\t\t\t\t\tLHS:\n"
      "\t\t\t\t\t\tLiteral expression: global_var\n"
      "\t\t\t\t\tRHS\n"
      "\t\t\t\t\t\tLiteral expression: 14\n"
      "\t\t\t\tThen branch:\n"
      "\t\t\t\t\tReturn\n"
      "\t\t\t\t\tValue (expression):\n"
      "\t\t\t\t\t\tBinary expression: +\n"
      "\t\t\t\t\t\tLHS:\n"
      "\t\t\t\t\t\t\tBinary expression: *\n"
      "\t\t\t\t\t\t\tLHS:\n"
      "\t\t\t\t\t\t\t\tLiteral expression: a\n"
      "\t\t\t\t\t\t\tRHS\n"
      "\t\t\t\t\t\t\t\tLiteral expression: b\n"
      "\t\t\t\t\t\tRHS\n"
      "\t\t\t\t\t\t\tLiteral expression: c\n"
      "\t\tStatement 1:\n"
      "\t\t\tExpression statement\n"
      "\t\t\t\tBinary expression: +\n"
      "\t\t\t\tLHS:\n"
      "\t\t\t\t\tLiteral expression: a\n"
      "\t\t\t\tRHS\n"
      "\t\t\t\t\tBinary expression: *\n"
      "\t\t\t\t\tLHS:\n"
      "\t\t\t\t\t\tLiteral expression: b\n"
      "\t\t\t\t\tRHS\n"
      "\t\t\t\t\t\tLiteral expression: c\n"
      "Declaration 2:\n"
      "\tFunction declaration: main\n"
      "\tParams: argc argv \n"
      "\tBody:\n"
      "\t\tBlock expression\n"
      "\t\tStatement 0:\n"
      "\t\t\tExpression statement\n"
      "\t\t\t\tReturn\n"
      "\t\t\t\tValue (expression):\n"
      "\t\t\t\t\tFunction call\n"
      "\t\t\t\t\tCallable:\n"
      "\t\t\t\t\t\tLiteral expression: magic_calc\n"
      "\t\t\t\t\tArg 0:\n"
      "\t\t\t\t\t\tLiteral expression: global_var\n"
      "\t\t\t\t\tArg 1:\n"
      "\t\t\t\t\t\tBinary expression: +\n"
      "\t\t\t\t\t\tLHS:\n"
      "\t\t\t\t\t\t\tLiteral expression: 12\n"
      "\t\t\t\t\t\tRHS\n"
      "\t\t\t\t\t\t\tBinary expression: /\n"
      "\t\t\t\t\t\t\tLHS:\n"
      "\t\t\t\t\t\t\t\tLiteral expression: 8\n"
      "\t\t\t\t\t\t\tRHS\n"
      "\t\t\t\t\t\t\t\tLiteral expression: 6\n"
      "\t\t\t\t\tArg 2:\n"
      "\t\t\t\t\t\tIf\n"
      "\t\t\t\t\t\tCondition:\n"
      "\t\t\t\t\t\t\tBinary expression: ==\n"
      "\t\t\t\t\t\t\tLHS:\n"
      "\t\t\t\t\t\t\t\tBinary expression: +\n"
      "\t\t\t\t\t\t\t\tLHS:\n"
      "\t\t\t\t\t\t\t\t\tLiteral expression: 1\n"
      "\t\t\t\t\t\t\t\tRHS\n"
      "\t\t\t\t\t\t\t\t\tLiteral expression: 1\n"
      "\t\t\t\t\t\t\tRHS\n"
      "\t\t\t\t\t\t\t\tLiteral expression: 2\n"
      "\t\t\t\t\t\tThen branch:\n"
      "\t\t\t\t\t\t\tLiteral expression: 13\n"
      "\t\t\t\t\t\tElse branch:\n"
      "\t\t\t\t\t\t\tLiteral expression: 14\n";

  lex::Lexer lexer(prg);
  parse::Parser parser(lexer);
  ast::Program* stmt = parser.ParseProgram();

  ast::SerializeVisitor serializer;
  stmt->Accept(&serializer);
  CHECK(serializer.GetSerializedString() == expected_output);
}

TEST_CASE("Parser: complex function call", "[parse]") {
  std::stringstream prg;
  prg << "test + (5 + 4)(kek)(lol + 4, 12)(hh)";

  std::string expected_output = "Binary expression: +\n"
      "LHS:\n"
      "\tLiteral expression: test\n"
      "RHS\n"
      "\tFunction call\n"
      "\tCallable:\n"
      "\t\tFunction call\n"
      "\t\tCallable:\n"
      "\t\t\tFunction call\n"
      "\t\t\tCallable:\n"
      "\t\t\t\tBinary expression: +\n"
      "\t\t\t\tLHS:\n"
      "\t\t\t\t\tLiteral expression: 5\n"
      "\t\t\t\tRHS\n"
      "\t\t\t\t\tLiteral expression: 4\n"
      "\t\t\tArg 0:\n"
      "\t\t\t\tLiteral expression: kek\n"
      "\t\tArg 0:\n"
      "\t\t\tBinary expression: +\n"
      "\t\t\tLHS:\n"
      "\t\t\t\tLiteral expression: lol\n"
      "\t\t\tRHS\n"
      "\t\t\t\tLiteral expression: 4\n"
      "\t\tArg 1:\n"
      "\t\t\tLiteral expression: 12\n"
      "\tArg 0:\n"
      "\t\tLiteral expression: hh\n";

  lex::Lexer lexer(prg);
  parse::Parser parser(lexer);
  ast::Expression* expr = parser.ParseExpression();

  ast::SerializeVisitor serializer;
  expr->Accept(&serializer);
  CHECK(serializer.GetSerializedString() == expected_output);
}