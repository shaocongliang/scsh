add_executable(AST_test SyntaxTree_test.cpp ../AST.cpp ../Util.cpp)
target_link_libraries(AST_test gtest)