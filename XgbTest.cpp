//
// Created by malanore on 11/11/21.
//

#include "XgbTest.h"

#include <xgboost/c_api.h>
#include <iostream>

using namespace std;

const float EXP = 1e-4;

BoosterHandle XgbTest::static_booster_ = nullptr;

int check_equal(const float* lp, bst_ulong ll, const float* rp, bst_ulong rl) {
  if (ll != rl) return 0;
  for (int i = 0; i < ll; ++i) {
    float diff = lp[i] - rp[i];
    if (diff < -EXP || diff > EXP) {
      return 0;
    }
  }
  return 1;
}

void XgbTest::InitBooster(std::string model_path) {
  if (XGBoosterCreate(nullptr, 0, &static_booster_)) {
    cout << "Create booster failed" << endl;
  }
  if (XGBoosterLoadModel(static_booster_, model_path.c_str())) {
    cout << "Load model failed" << endl;
  }
}

void XgbTest::Init() {
  if (static_booster_ != nullptr) {
    this->booster_ = static_booster_;
  } else {
    if (XGBoosterCreate(nullptr, 0, &this->booster_)) {
      cout << "Create booster failed" << endl;
    }
    if (XGBoosterLoadModel(this->booster_, this->model_path_.c_str())) {
      cout << "Load model failed" << endl;
    }
  }
  FILE* fp = fopen("../data/std.pred", "rb");
  fread(&this->std_len, sizeof(bst_ulong), 1, fp);
  this->std_result = (float*) malloc(sizeof(float ) * this->std_len);
  fread(this->std_result, sizeof(float ), this->std_len, fp);
  fclose(fp);
}

void XgbTest::Process() {
  DMatrixHandle d_matrix;
  if (XGDMatrixCreateFromFile(this->data_path_.c_str(), 2, &d_matrix)) {
    cout << "Create DMatrix failed" << endl;
  }
  const float* result;
  bst_ulong len;
  if (XGBoosterPredict(booster_, d_matrix, 0, 0, &len, &result)) {
    cout << "Predict failed" << endl;
  }

  if (check_equal(this->std_result, this->std_len, result, len) == 0) {
    cout << "-------------- ERROR -------------------" << endl;
  }
  // delete result;
}