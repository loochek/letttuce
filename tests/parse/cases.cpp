#include <lex/lexer.hpp>
#include <parse/parser.hpp>
#include <ast/visitors/serialize_visitor.hpp>

// Finally,
#include <catch2/catch.hpp>

#include <iostream>
#include <sstream>

//////////////////////////////////////////////////////////////////////

TEST_CASE("Parser: Complex test", "[parse]") {
  std::stringstream program;
  program << "fun main(argc argv) = {"
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
  Parser parser(lexer);
  Declaration* decl = parser.ParseDeclaration();

  SerializeVisitor serializer;
  decl->Accept(&serializer);

  CHECK(serializer.GetSerializedString() == expected_output);
}