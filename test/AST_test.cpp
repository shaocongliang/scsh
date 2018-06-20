/**
 *
 */
#include <iostream>

#include <gtest/gtest.h>
#include <AST.h>
using namespace std;
TEST(SyntaxTreeTest, SingleCMDTest){
   char *toks[] = {"cat", "nums", "\n", "eof"}; 
   char **tokp = toks + 3;
   auto node = ConstructAbstractSyntaxTree(toks, tokp);
   EXPECT_NE(nullptr, node);
   EXPECT_EQ(2, node->left_->cmds_.size());
   EXPECT_EQ(0, node->cmds_.size());
   EXPECT_EQ(CMDSEQ, node->type_);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
