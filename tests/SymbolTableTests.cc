#include "SymbolTable.hh"
#include <gtest/gtest.h>

using namespace argc;

class SymbolTableTest : public ::testing::Test {
protected:
  SymbolTable table;
  std::shared_ptr<Type> intType;
  std::shared_ptr<Type> boolType;
  loc::SourceLocation dummyLoc{0, 0, ""};

  void SetUp() override {
    intType = std::make_shared<PrimitiveType>("i32");
    boolType = std::make_shared<PrimitiveType>("bool");
  }
};

TEST_F(SymbolTableTest, InsertAndLookupSymbolInGlobalScope) {
  auto sym = std::make_shared<SymbolEntry>("x", SymbolKind::VARIABLE, intType, 0, dummyLoc);
  ASSERT_TRUE(table.insert(sym));

  auto found = table.lookup("x");
  ASSERT_NE(found, nullptr);
  EXPECT_EQ(found->name(), "x");
  EXPECT_EQ(found->scope_level(), 0);
}

TEST_F(SymbolTableTest, SymbolShadowingAcrossScopes) {
  table.insert(std::make_shared<SymbolEntry>("x", SymbolKind::VARIABLE, intType, 0, dummyLoc));

  table.enter_scope("func_main");
  auto innerSym = std::make_shared<SymbolEntry>("x", SymbolKind::VARIABLE, boolType, 1, dummyLoc);
  ASSERT_TRUE(table.insert(innerSym));

  auto found = table.lookup("x");
  ASSERT_NE(found, nullptr);
  EXPECT_EQ(found->scope_level(), 1);
  EXPECT_EQ(found->type()->name(), "bool");

  table.exit_scope();

  auto global = table.lookup("x");
  EXPECT_EQ(global->scope_level(), 0);
  EXPECT_EQ(global->type()->name(), "i32");
}

TEST_F(SymbolTableTest, LookupFailsForUndeclaredSymbol) {
  auto found = table.lookup("y");
  EXPECT_EQ(found, nullptr);
}

TEST_F(SymbolTableTest, InsertFailsOnDuplicateInSameScope) {
  auto sym1 = std::make_shared<SymbolEntry>("x", SymbolKind::VARIABLE, intType, 0, dummyLoc);
  auto sym2 = std::make_shared<SymbolEntry>("x", SymbolKind::VARIABLE, boolType, 0, dummyLoc);

  EXPECT_TRUE(table.insert(sym1));
  EXPECT_FALSE(table.insert(sym2)); // should fail
}

TEST_F(SymbolTableTest, StructTypeAndFieldStorage) {
  auto structType = std::make_shared<StructType>("MyStruct");

  TypeTable ttable;
  ttable.insert(structType);

  auto retrieved = ttable.lookup("MyStruct");
  ASSERT_NE(retrieved, nullptr);
  EXPECT_EQ(retrieved->name(), "MyStruct");
  EXPECT_EQ(retrieved->kind(), TypeKind::STRUCT);
}
