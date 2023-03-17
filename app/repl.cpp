#include <fmt/color.h>
#include <lex/lexer.hpp>
#include <parse/parser.hpp>

#include <ast/expressions.hpp>
#include <ast/statements.hpp>
#include <ast/declarations.hpp>
#include <ast/visitors/print_visitor.hpp>
#include <ast/visitors/scope_generator.hpp>
#include <ast/visitors/definition_checker.hpp>

#include <iostream>
#include <sstream>

int main() {
  std::stringstream program;
  program << "# Complete Lettuce program\n"
         "\n"
         "var global_var = if 12 == 11 + 1 then 14 else 15 + 81;\n"
         "\n"
         "fun magic_calc(x, b, c) = {\n"
         "    var a = 5;\n"
         "    if (global_var == 14) then return a * b + c; a + b * c;\n"
         "};\n"
         "fun main(argc, argv) = {\n"
         "    return magic_calc(global_var, 12 + 8 / 6, if 1 + 1 == 2 then 13 else 14);\n"
         "};\n";

  lex::Lexer lexer(program);
  parse::Parser parser(lexer);
  ast::Program* prg = parser.ParseProgram();

  ast::ScopeGenerator gen;
  prg->Accept(&gen);

  ast::DefinitionChecker checker;
  prg->Accept(&checker);

//  ast::PrintVisitor serializer;
//  decl->Accept(&serializer);

}
