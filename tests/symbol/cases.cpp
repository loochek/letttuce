#include <lex/lexer.hpp>
#include <parse/parser.hpp>
#include <parse/parse_error.hpp>
#include <passes/symbol_table_builder.hpp>
#include <passes/definition_checker.hpp>

// Finally,
#include <catch2/catch.hpp>

#include <iostream>
#include <sstream>

//////////////////////////////////////////////////////////////////////


TEST_CASE("Symbol table: just works", "[symbol]") {
  std::stringstream program;
  program << "# Complete Lettuce program\n"
             "\n"
             "var global_var = if 12 == 11 + 1 then 14 else 15 + 81;\n"
             "\n"
             "fun magic_calc(a, b, c) = {\n"
             "    if (global_var == 14) then return a * b + c; a + b * c;\n"
             "};";

  lex::Lexer lexer(program);
  parse::Parser parser(lexer);
  ast::Program* prg = parser.ParseProgram();

  ast::SymbolTableBuilder gen;
  prg->Accept(&gen);

  ast::DefinitionChecker checker;
  prg->Accept(&checker);
}

TEST_CASE("Symbol table: undefined symbol", "[symbol]") {
  std::stringstream program;
  program << "# Complete Lettuce program\n"
             "\n"
             "var global_var = if 12 == 11 + 1 then 14 else 15 + 81;\n"
             "\n"
             "fun magic_calc(b, c) = {\n"
             "    if (global_var == 14) then return a * b + c; a + b * c;\n"
             "};";

  lex::Lexer lexer(program);
  parse::Parser parser(lexer);
  ast::Program* prg = parser.ParseProgram();

  ast::SymbolTableBuilder gen;
  prg->Accept(&gen);

  ast::DefinitionChecker checker;
  CHECK_THROWS_AS(prg->Accept(&checker), ast::errors::UndefinedSymbolError);
}

TEST_CASE("Symbol table: order-sensitive", "[symbol]") {
  std::stringstream program;
  program << "# Complete Lettuce program\n"
             "\n"
             "var global_var = if 12 == 11 + 1 then 14 else 15 + 81;\n"
             "\n"
             "fun magic_calc(b, c) = {\n"
             "    if (global_var == 14) then return a * b + c; a + b * c;\n"
             "    var a = 14;\n"
             "};";

  lex::Lexer lexer(program);
  parse::Parser parser(lexer);
  ast::Program* prg = parser.ParseProgram();

  ast::SymbolTableBuilder gen;
  prg->Accept(&gen);

  ast::DefinitionChecker checker;
  CHECK_THROWS_AS(prg->Accept(&checker), ast::errors::UndefinedSymbolError);
}

TEST_CASE("Symbol table: redefinition", "[symbol]") {
  std::stringstream program;
  program << "# Complete Lettuce program\n"
             "\n"
             "var global_var = if 12 == 11 + 1 then 14 else 15 + 81;\n"
             "\n"
             "fun magic_calc(b, c) = {\n"
             "    var a = 14;\n"
             "    var a = global_var;\n"
             "    if (global_var == 14) then return a * b + c; a + b * c;\n"
             "};";

  lex::Lexer lexer(program);
  parse::Parser parser(lexer);
  ast::Program* prg = parser.ParseProgram();

  ast::SymbolTableBuilder gen;
  CHECK_THROWS_AS(prg->Accept(&gen), ast::errors::RedefinitionError);
}

