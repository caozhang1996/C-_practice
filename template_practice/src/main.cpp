#include <type_traits>

#include "template_practice/template_function_1.h"
#include "template_practice/template_function_2.h"
#include "template_practice/template_function_3.h"

using namespace std::string_literals;

int main(int argc, char** argv)
{
  template_function_1_test::test_max();
  template_function_1_test::test_print();

  template_function_2_test::test_process();

  template_function_3_test::test_print_all();

  return 0;
}
