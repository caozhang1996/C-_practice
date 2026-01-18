#include <type_traits>

#include "template_practice/template_class_1.h"
#include "template_practice/template_class_2.h"
#include "template_practice/template_function_1.h"
#include "template_practice/template_function_2.h"
#include "template_practice/template_function_3.h"

using namespace std::string_literals;

int main(int argc, char** argv)
{
  std::cout << "###############template_function_1_test###############"
            << std::endl;
  template_function_1_test::test_max();
  template_function_1_test::test_print();
  std::cout << std::endl;

  std::cout << "###############template_function_2_test###############"
            << std::endl;
  template_function_2_test::test_process();
  std::cout << std::endl;

  std::cout << "###############template_function_3_test###############"
            << std::endl;
  template_function_3_test::test_print_all();
  std::cout << std::endl;

  std::cout << "###############template_class_1_test###############"
            << std::endl;
  template_class_1_test::test();
  std::cout << std::endl;

  std::cout << "###############template_class_2_test###############"
            << std::endl;
  template_class_2_test::test();
  std::cout << std::endl;

  return 0;
}
