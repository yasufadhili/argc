#include <vector>
#include <iostream>
#include "ast.hh"

auto main() -> int {
    // Test arithmetic expressions
    // Create separate operands for each operation
    auto* add_num1 = new ast::expr::arith::Add(
    new ast::expr::Constant("23"),
    new ast::expr::Constant("30")
    );
    auto* add_num2 = new ast::expr::arith::Add(nullptr, nullptr);
    const auto* add = new ast::expr::arith::Add(add_num1, add_num2);

    auto* sub_num1 = new ast::expr::arith::Add(nullptr, nullptr);
    auto* sub_num2 = new ast::expr::arith::Add(nullptr, nullptr);
    const auto* sub = new ast::expr::arith::Sub(sub_num1, sub_num2);

    auto* mul_num1 = new ast::expr::arith::Add(nullptr, nullptr);
    auto* mul_num2 = new ast::expr::arith::Add(nullptr, nullptr);
    const auto* mul = new ast::expr::arith::Mul(mul_num1, mul_num2);

    auto* div_num1 = new ast::expr::arith::Add(nullptr, nullptr);
    auto* div_num2 = new ast::expr::arith::Add(nullptr, nullptr);
    const auto* div = new ast::expr::arith::Div(div_num1, div_num2);

    auto* mod_num1 = new ast::expr::arith::Add(nullptr, nullptr);
    auto* mod_num2 = new ast::expr::arith::Add(nullptr, nullptr);
    const auto* mod = new ast::expr::arith::Mod(mod_num1, mod_num2);

    const auto* neg = new ast::expr::arith::Neg();
    const auto* not_op = new ast::expr::arith::Not();

    // Test boolean expressions
    const auto* true_expr = new ast::expr::boolean::True();
    const auto* false_expr = new ast::expr::boolean::False();

    // Test relational expressions
    const auto* equal = new ast::expr::rel::Equal();
    const auto* not_equal = new ast::expr::rel::NotEqual();
    const auto* greater_than = new ast::expr::rel::GreaterThan();
    const auto* less_than = new ast::expr::rel::LessThan();
    const auto* greater_equal = new ast::expr::rel::GreaterThanOrEqual();
    const auto* less_equal = new ast::expr::rel::LessThanOrEqual();

    // Create a block of statements
    const std::vector<ast::stmt::Statement*> statements = {
        new ast::stmt::Statement(),
        new ast::stmt::Statement()
    };
    auto* block = new ast::stmt::Block(statements);

    // Create a function
    auto* function = new ast::func::Function(block);

    // Create a module with statements and functions
    const std::vector<ast::stmt::Statement*> module_statements = {
        new ast::stmt::Statement(),
        new ast::stmt::Statement()
    };

    const std::vector<ast::func::Function*> module_functions = {function};
    auto* module = new ast::module::Module(module_statements, module_functions);

    // Create a program with modules
    const std::vector<ast::module::Module*> modules = {module};
    auto* program = new ast::prog::Program(modules);

    // Print the program structure
    std::cout << "Testing Program Structure:\n";
    program->print();

    // Clean up - only delete the program, which will cascade delete everything else
    delete program;

    // Clean up expressions that aren't part of the program structure
    delete add;
    delete sub;
    delete mul;
    delete div;
    delete mod;
    delete neg;
    delete not_op;
    delete true_expr;
    delete false_expr;
    delete equal;
    delete not_equal;
    delete greater_than;
    delete less_than;
    delete greater_equal;
    delete less_equal;

    return 0;
}