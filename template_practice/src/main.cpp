#include <type_traits>

#include "template_practice/template_class_1.h"
#include "template_practice/template_class_2.h"
#include "template_practice/template_class_3.h"
#include "template_practice/template_class_4.h"
#include "template_practice/template_class_5.h"
#include "template_practice/template_class_6.h"
#include "template_practice/template_class_7.h"
#include "template_practice/template_function_1.h"
#include "template_practice/template_function_2.h"
#include "template_practice/template_function_3.h"
#include "template_practice/template_function_4.h"
#include "template_practice/template_static_member_1.h"
#include "template_practice/template_variable_1.h"
#include "template_practice/template_variable_2.h"

using namespace std::string_literals;

int main(int argc, char** argv)
{
  std::cout << "###############template_function_1_test###############"
            << std::endl;
  template_function_1_test::testMax();
  template_function_1_test::testPrint();
  std::cout << std::endl;

  std::cout << "###############template_function_2_test###############"
            << std::endl;
  template_function_2_test::testProcess();
  std::cout << std::endl;

  std::cout << "###############template_function_3_test###############"
            << std::endl;
  template_function_3_test::testPrintAll();
  std::cout << std::endl;

  std::cout << "###############template_function_4_test###############"
            << std::endl;
  template_function_4_test::test();
  std::cout << std::endl;

  std::cout << "###############template_class_1_test###############"
            << std::endl;
  template_class_1_test::test();
  std::cout << std::endl;

  std::cout << "###############template_class_2_test###############"
            << std::endl;
  template_class_2_test::test();
  std::cout << std::endl;

  std::cout << "###############template_class_3_test###############"
            << std::endl;
  template_class_3_test::test();
  std::cout << std::endl;

  std::cout << "###############template_class_4_test###############"
            << std::endl;
  template_class_4_test::test();
  std::cout << std::endl;

  std::cout << "###############template_class_5_test###############"
            << std::endl;
  template_class_5_test::test();
  std::cout << std::endl;

  std::cout << "###############template_class_6_test###############"
            << std::endl;
  template_class_6_test::test();
  std::cout << std::endl;

  std::cout << "###############template_class_7_test###############"
            << std::endl;
  template_class_7_test::testOne();
  std::cout << std::endl;

  std::cout << "###############template_variable_1_test###############"
            << std::endl;
  template_variable_1_test::test();
  template_variable_1_test::testFullySpecialized();
  std::cout << std::endl;

  std::cout << "###############template_variable_2_test###############"
            << std::endl;
  template_variable_2_test::testOne();
  template_variable_2_test::testTwo();
  std::cout << std::endl;

  std::cout << "###############template_static_member_1_test###############"
            << std::endl;
  template_static_member_1_test::test();
  std::cout << std::endl;

  return 0;
}
