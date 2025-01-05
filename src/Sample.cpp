#include <Sample.hpp>
// Sample.cpp
WCDB_CPP_ORM_IMPLEMENTATION_BEGIN(Sample)
WCDB_CPP_SYNTHESIZE(identifier)
WCDB_CPP_SYNTHESIZE(content)
WCDB_CPP_ORM_IMPLEMENTATION_END


int add(int a, int b) {
    return a + b;
}