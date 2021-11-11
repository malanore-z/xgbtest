#include <iostream>

#include <thread>
#include <mutex>
#include <unistd.h>

#include "XgbTest.h"
#include "LgbTest.h"

using namespace std;

mutex mtx;

void xgboost_run_func() {
  cout << "Start thread: " << this_thread::get_id() << endl;

  XgbTest xt("../data/binary_model", "../data/data.csv");
  xt.Init();
  for (int i = 0; i < 300; ++i) {
    xt.Process();
  }
  cout << "End thread: " << this_thread::get_id() << endl;
}

void xgboost_test(bool use_same_booster) {
  if (use_same_booster) {
    XgbTest::InitBooster("../data/binary_model");
  }
  for (int i = 0; i < 10; ++i) {
    thread t{xgboost_run_func};
    t.detach();
  }
}

void lgb_run_func() {
  cout << "Start thread: " << this_thread::get_id() << endl;

  LgbTest lt("../data/gbm_binary_model", "../data/data.csv");
  lt.Init();
  for (int i = 0; i < 300; ++i) {
    lt.Process();
  }
  cout << "End thread: " << this_thread::get_id() << endl;
}

void lgb_test(bool use_same_booster) {
  if (use_same_booster) {
    LgbTest::InitBooster("../data/gbm_binary_model");
  }
  for (int i = 0; i < 20; ++i) {
    thread t{lgb_run_func};
    t.detach();
  }
}

int main() {

  bool use_xgboost = true;
  bool use_same_booster = false;

  if (use_xgboost) {
    xgboost_test(use_same_booster);
  } else {
    lgb_test(use_same_booster);
  }

  sleep(50);

  return 0;
}
