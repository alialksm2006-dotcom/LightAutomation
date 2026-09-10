#include "Test.h"
WebServer Test::server(80);
struct User
{
    int id;
    const char* name;
    int age;
    const char* email;
};

 Test::User Test::users[] =
{
    {1, "Ahmad", 23, "Ahmed@gmail.com"},
    {2, "Ali", 20, "Ali@gmail.com"},
    {3, "Omar", 30, "omar@gmail.com"}
};
