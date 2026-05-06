#include <gtest/gtest.h>
#include <array>


class fixture: public ::testing::Test{
protected:
    array_seq<Mutability::Mutable, int> test_arr;
    list_seq<Mutability::Mutable, int> test_list;
public:
    void SetUp() override{

    }
    void TearDown()override{
    }
};

//TEST_F(название фикстуры, название теста){тело теста}