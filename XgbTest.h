//
// Created by malanore on 11/11/21.
//

#ifndef XGBTEST__XGBTEST_H_
#define XGBTEST__XGBTEST_H_

#include <string>
#include <utility>

#include <xgboost/c_api.h>

class XgbTest {

 public:
  XgbTest() = default;

  XgbTest(std::string  model_path, std::string  data_path)
  : model_path_(std::move(model_path)), data_path_(std::move(data_path)) {}

  void Init();

  static void InitBooster(std::string model_path);

  void Process();

 private:
  std::string model_path_;
  std::string data_path_;
  BoosterHandle booster_;
  float* std_result;
  bst_ulong std_len;
  static BoosterHandle static_booster_;
};

#endif //XGBTEST__XGBTEST_H_
