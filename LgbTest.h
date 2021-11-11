//
// Created by malanore on 11/12/21.
//

#ifndef XGBTEST__LGBTEST_H_
#define XGBTEST__LGBTEST_H_

#include <string>
#include <LightGBM/c_api.h>

class LgbTest {

 public:
  LgbTest() = default;

  LgbTest(std::string  model_path, std::string  data_path)
      : model_path_(std::move(model_path)), data_path_(std::move(data_path)) {}

  void Init();

  static void InitBooster(std::string model_path);

  void Process();

 private:
  std::string model_path_;
  std::string data_path_;
  BoosterHandle booster_;
  double * std_result;
  uint64_t std_len;
  static BoosterHandle static_booster_;
};

#endif //XGBTEST__LGBTEST_H_
