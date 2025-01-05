#include <WCDB/WCDBCpp.h>
// Sample.hpp
class Sample {
public:
    Sample();//必须要有默认构造函数
    Sample(int identifier, const std::string& content);//非必须实现的构造函数，只是为了演示方便
    int identifier;
    std::string content;
    WCDB_CPP_ORM_DECLARATION(Sample)
};


int add(int a, int b);