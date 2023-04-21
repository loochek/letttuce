#include <fmt/color.h>
#include <lex/lexer.hpp>
#include <utils/storage.hpp>

#include <parse/parser.hpp>
#include <ast/expressions.hpp>
#include <ast/declarations.hpp>

#include <ast/visitors/print_visitor.hpp>
#include <passes/symbol_table_builder.hpp>
#include <passes/definition_checker.hpp>
#include <passes/type_evaluator.hpp>

#include <fstream>

int main(int argc, const char* argv[]) {
  if (argc < 2) {
    fmt::print("Usage: {} <source>\n", argv[0]);
    return 0;
  }

  std::ifstream program(argv[1]);

  lex::Lexer lexer(program);
  utils::Storage<types::Type> type_keeper;

  parse::Parser parser(lexer, type_keeper);
  ast::Program* prg = parser.ParseProgram();

  passes::SymbolTableBuilder gen;
  prg->Accept(&gen);

  passes::DefinitionChecker checker;
  prg->Accept(&checker);

  passes::TypeEvaluator type_evaluator;
  prg->Accept(&type_evaluator);

  ast::PrintVisitor serializer;
  prg->Accept(&serializer);
}
