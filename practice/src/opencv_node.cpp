#include <iostream>
#include <vector>

#include <opencv4/opencv2/core/core.hpp>

class OpencvTest
{
 public:
  OpencvTest() = default;

  void createMat()
  {
    std::vector<double> dist = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    // 注意这里传递的 dist 的裸指针，当 dist 析构掉时，mat_
    // 持有的是野指针了，导致 mat_ 中的数据是随即值
    mat_ = cv::Mat_<double>(3, 3, dist.data());

    std::cout << "In createMat: " << mat_ << std::endl;
  }

  void printMat() { std::cout << "In printMat: " << mat_ << std::endl; }

 private:
  cv::Mat mat_;
};

int main(int agc, char** argv)
{
  OpencvTest test1;

  test1.createMat();
  test1.printMat();

  return 0;
}