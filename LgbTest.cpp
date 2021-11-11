//
// Created by malanore on 11/12/21.
//

#include "LgbTest.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <unordered_map>

using namespace std;

const double EXP = 1e-4;

BoosterHandle LgbTest::static_booster_ = nullptr;

auto Split(const string& str, char delim) -> vector<string> {
  vector<string> ret;
  string tmp;
  for (int i = 0; i <= str.length(); ++i) {
    if (i == str.length() || str[i] == delim) {
      ret.push_back(tmp);
      tmp = "";
    } else {
      tmp.push_back(str[i]);
    }
  }
  return ret;
}

auto LoadCsv(const string& path, char delim=',') -> vector<vector<string> > {
  ifstream fs(path);
  string line;
  vector<vector<string> > ret;
  while (getline(fs, line)) {
    if (!line.empty())
      ret.push_back(Split(line, delim));
  }
  fs.close();
  return ret;
}

auto LoadFloatData(const string& data_path, int *len) -> pair<float*, float*> {
  vector<vector<string> > data = LoadCsv(data_path);
  *len = (int) data.size();
  float *mat_data = (float*) malloc(sizeof(float) * 28 * data.size());
  float *labels = (float*) malloc(sizeof(float ) * 1 * data.size());
  for (int i = 0; i < data.size(); ++i) {
    for (int j = 0; j < 28; ++j) {
      if (data[i][j].empty()) {
        mat_data[28 * i + j] = 0;
      } else {
        mat_data[28 * i + j] = stof(data[i][j]);
      }
    }
    labels[i] = stof(data[i][28]);
  }
  return make_pair(mat_data, labels);
}

int check_equal_1(const double * lp, uint64_t ll, const double * rp, uint64_t rl) {
  if (ll != rl) return 0;
  for (int i = 0; i < ll; ++i) {
    double diff = lp[i] - rp[i];
    if (diff < -EXP || diff > EXP) {
      return 0;
    }
  }
  return 1;
}

void LgbTest::InitBooster(std::string model_path) {
  int num;
  if (LGBM_BoosterCreateFromModelfile(model_path.c_str(), &num, &static_booster_) != 0) {
    cout << "Load booster failed" << endl;
  }
}

void LgbTest::Init() {
  if (static_booster_ != nullptr) {
    this->booster_ = static_booster_;
  } else {
    int num;
    if (LGBM_BoosterCreateFromModelfile(this->model_path_.c_str(), &num, &this->booster_) != 0) {
      cout << "Load booster failed" << endl;
    }
  }
  FILE* fp = fopen("../data/lgb.pred", "rb");
  fread(&this->std_len, sizeof(uint64_t), 1, fp);
  this->std_result = (double*) malloc(sizeof(double) * this->std_len);
  fread(this->std_result, sizeof(double ), this->std_len, fp);
  fclose(fp);
}

void LgbTest::Process() {
  int len;
  auto p = LoadFloatData(this->data_path_, &len);
  float* mat_data = p.first;
  float* labels = p.second;
  auto * result = (double *) malloc(sizeof(double ) * len);
  int64_t result_len;

  if (LGBM_BoosterPredictForMat(this->booster_, mat_data, C_API_DTYPE_FLOAT32, len, 28, 1,
                                C_API_PREDICT_NORMAL, -1, "", &result_len, result) != 0) {
    cout << "Predict failed" << endl;
  }

  if (check_equal_1(this->std_result, this->std_len, result, result_len) == 0) {
    cout << "-------- ERROR ----------" << endl;
  }

  free(mat_data);
  free(labels);
  free(result);
}