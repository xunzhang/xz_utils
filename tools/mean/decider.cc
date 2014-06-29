/**
 * Naive decider of arthimetic/harmonic/geometric means
 *
 * Ideas: 
 *  AM - is the normal average value which fits for a list of similar values
 *  HM - fits for the case that different dimension have different numeric ranges(something like median value but vary with absolute value of each dimension)
 *       also fits for the case that numbers are relevant(given as percentage increases or decreases, rather than absolute value)
 *  GM - fits for the case that some outlier value is much larger than the remaining values
 * 
 *  HM <= GM <= AM 
 *
 * Decision tree:
 *  Does the data rep returns(such as percentage increase) ---Y---> GM
 *                           |                                     / \
 *                           N                                      |
 *                           |                                      |
                            \ /                                     |
 *                    different ranges -------------Y---------------
 *                           |
 *                           N
 *                           | 
 *                          \ /
 *                    large outliers --------------Y--------------> HM
 *                           |
 *                           N
 *                           |
 *                          \ /
 *                           AM 
 *
 * Applications in ML:
 *  Error estimation such as precision/recall-tradeoff
 */

#include <cmath>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm>

template <class T>
T am(const std::vector<T> & data) {
  T r;
  for(auto & v : data) {
    r += v;
  }
  return static_cast<T>(r / data.size());
}

template <class T>
T hm(const std::vector<T> & data) {
  T r;
  for(auto & v : data) {
    r += 1. / v;
  }
  r = static_cast<T>(r / data.size());
  return static_cast<T>(1. / r);
}

template <class T>
T gm(const std::vector<T> & data) {
  T r = 1;
  double power = 1. / data.size();
  for(auto & v : data) {
    r *= v; 
  }
  return std::pow(r, power);
}

template <class T>
bool have_large_outliers(const std::vector<T> & data) {
  std::vector<T> tmp(data);
  std::sort(tmp.begin(), tmp.end());
  size_t half_bnd = tmp.size() / 2;
  auto multiple1 = tmp[half_bnd] / tmp[0];
  auto multiple2 = tmp[tmp.size() - 1] / tmp[half_bnd];
  if(multiple2 > multiple1 * 2) {
    return true;
  }
  return false;
}

// decider interface
template <class T>
int mean_decider(const std::vector<T> & data,
                 bool earning_flag = false,
                 bool range_flag = false) {
  if(earning_flag) {
    return 3; // gm
  } else if(range_flag) {
    return 3; // gm
  } else if(have_large_outliers(data)) {
    return 2; // hm
  } else {
    return 1; // am
  }
}

// mean interface
template <class T>
T mean(const std::vector<T> & data, 
       int indx = 0, 
       bool earning_flag = false, 
       bool range_flag = false) {
  if(indx == 0) {
    return mean(data, 
								mean_decider(data, earning_flag, range_flag), 
								earning_flag, 
								range_flag);
  } else if(indx == 1) {
    return am(data);
  } else if(indx == 2) {
    return hm(data);
  } else if(indx == 3) {
    return gm(data);
  } else {
    throw std::runtime_error("Wrong value of indx parameter: must be 0, 1, 2, 3\n");
  }
}

int main(int argc, char *argv[])
{
  {
    std::vector<double> data = {35, 48, 35, 40, 50., 35, 35, 40, 150, 35, 40, 35, 45, 45};
    std::cout << mean_decider(data) << std::endl;
    std::cout << mean(data) << std::endl;
  }
  std::cout << "---" << std::endl;
  {
    std::vector<double> data = {1.05, 1.2, 1.25, .9, 1.2};
    std::cout << mean_decider(data, true) << std::endl;
    std::cout << mean(data, 0, true) << std::endl;
  }
  std::cout << "---" << std::endl;
  {
    std::vector<double> data = {0.96, 8.7, 121, 1400, 9000};
    std::cout << mean_decider(data, false, true) << std::endl;
    std::cout << mean(data, 0, false, true) << std::endl;
  }
  return 0;
}
