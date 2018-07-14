/**
 *
 */
#include <iostream>

#include <gtest/gtest.h>
#include <AST.h>
#include <Util.h>
using namespace std;
TEST(SyntaxTreeTest, PIPE_TEST)
{
    std::vector<std::string> toks = {"cat", "nums", "|", "wc", "-l", "\n"};
    auto node = ConstructAbstractSyntaxTree(toks.cbegin(), toks.cend());
    EXPECT_EQ(TPIPE, node->left_->type_);
    InorderPrintAST(node);
}

TEST(SyntaxTreeTest, SingleCMDTest)
{
    std::vector<std::string> toks = {"cat", "nums", "\n"};
    auto node = ConstructAbstractSyntaxTree(toks.cbegin(), toks.cend());
    EXPECT_NE(nullptr, node);
    EXPECT_EQ(2, node->left_->cmds_.size());
    EXPECT_EQ(0, node->cmds_.size());
    EXPECT_EQ(TLIST, node->type_);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
