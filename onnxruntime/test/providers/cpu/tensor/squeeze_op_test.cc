// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "gtest/gtest.h"
#include "test/common/dnnl_op_test_utils.h"
#include "test/providers/provider_test_utils.h"
#include "test/util/include/default_providers.h"

namespace onnxruntime {
namespace test {

// Some of the tests can't run on TensorrtExecutionProvider because axis=0 is not supported
// or there are unsupported data types. Those tests will fallback to other EPs.

TEST(SqueezeOpTest, Squeeze_1) {
  OpTester test("Squeeze");
  test.AddAttribute("axes", std::vector<int64_t>{0});
  test.AddInput<float>("data", {1, 3, 4, 5}, std::vector<float>(60, 1.0f));
  test.AddOutput<float>("squeezed", {3, 4, 5}, std::vector<float>(60, 1.0f));
  test.Run(OpTester::ExpectResult::kExpectSuccess, "", {kOpenVINOExecutionProvider});  // Incorrect precision. Will be re-enabled after it's fixed
}

TEST(SqueezeOpTest, Squeeze_Empty_Axes_1) {
  OpTester test("Squeeze");
  test.AddInput<float>("data", {1, 1, 4, 1}, std::vector<float>(4, 1.0f));
  test.AddOutput<float>("squeezed", {4}, std::vector<float>(4, 1.0f));
  // TensorRT doesn't seem to support missing 'axes'
  test.Run(OpTester::ExpectResult::kExpectSuccess, "", {kTensorrtExecutionProvider});
}

TEST(SqueezeOpTest, Squeeze_Empty_Axes_2) {
  OpTester test("Squeeze");
  // nothing to "squeeze" out in the input shape
  test.AddInput<float>("data", {2, 4}, std::vector<float>(8, 1.0f));
  test.AddOutput<float>("squeezed", {2, 4}, std::vector<float>(8, 1.0f));
  // TensorRT doesn't seem to support missing 'axes'
  test.Run(OpTester::ExpectResult::kExpectSuccess, "", {kTensorrtExecutionProvider});
}

TEST(SqueezeOpTest, Squeeze_Empty_Axes_3) {
  OpTester test("Squeeze");
  // Squeeze all for all 1's shape will end up as a scalar
  test.AddInput<float>("data", {1, 1, 1, 1}, std::vector<float>{1.0f});
  test.AddOutput<float>("squeezed", {}, std::vector<float>{1.0f});
  // TensorRT doesn't seem to support missing 'axes'
  test.Run(OpTester::ExpectResult::kExpectSuccess, "", {kTensorrtExecutionProvider});
}

TEST(SqueezeOpTest, Squeeze_1_int32) {
  OpTester test("Squeeze");
  test.AddAttribute("axes", std::vector<int64_t>{0});
  test.AddInput<int32_t>("data", {1, 3, 4, 5}, std::vector<int32_t>(60, 1));
  test.AddOutput<int32_t>("squeezed", {3, 4, 5}, std::vector<int32_t>(60, 1));
  test.Run(OpTester::ExpectResult::kExpectSuccess, "", {kOpenVINOExecutionProvider});  // Incorrect precision. Will be re-enabled after it's fixed
}

TEST(SqueezeOpTest, Squeeze_string) {
  OpTester test("Squeeze");
  test.AddAttribute("axes", std::vector<int64_t>{0, 2, 4});
  test.AddInput<std::string>("data", {1, 2, 1, 3, 1}, std::vector<std::string>({"1", "2", "3", "4", "5", "6"}));
  test.AddOutput<std::string>("squeezed", {2, 3}, std::vector<std::string>({"1", "2", "3", "4", "5", "6"}));
  test.Run();
}

TEST(SqueezeOpTest, Squeeze_2) {
  OpTester test("Squeeze");
  test.AddAttribute("axes", std::vector<int64_t>{0, 2, 3});
  test.AddInput<float>("data", {1, 4, 1, 1, 2},
                       std::vector<float>{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f});
  test.AddOutput<float>("squeezed", {4, 2},
                        std::vector<float>{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f});
  test.Run(OpTester::ExpectResult::kExpectSuccess, "", {kOpenVINOExecutionProvider});  // Incorrect precision. Will be re-enabled after it's fixed
}

TEST(SqueezeOpTest, UnsortedAxes) {
  OpTester test("Squeeze");
  test.AddShapeToTensorData(false);  // TODO: re-enable shape inference test after ONNX fix
  test.AddAttribute("axes", std::vector<int64_t>{3, 0, 2});
  test.AddInput<float>("data", {1, 4, 1, 1, 2},
                       std::vector<float>{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f});
  test.AddOutput<float>("squeezed", {4, 2},
                        std::vector<float>{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f});
  test.Run(OpTester::ExpectResult::kExpectSuccess, "", {kOpenVINOExecutionProvider});  // Incorrect precision. Will be re-enabled after it's fixed
}

TEST(SqueezeOpTest, DuplicateAxes) {
  OpTester test("Squeeze");
  test.AddShapeToTensorData(false);  // TODO: re-enable shape inference test after ONNX fix
  test.AddAttribute("axes", std::vector<int64_t>{3, 0, 2, 0, 2, 3});
  test.AddInput<float>("data", {1, 4, 1, 1, 2},
                       std::vector<float>{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f});
  test.AddOutput<float>("squeezed", {4, 2},
                        std::vector<float>{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f});
  test.Run(OpTester::ExpectResult::kExpectSuccess, "", {kOpenVINOExecutionProvider});  // Incorrect precision. Will be re-enabled after it's fixed
}

TEST(SqueezeOpTest, BadAxes) {
  // TODO: Unskip when fixed #41968513
  if (DefaultDmlExecutionProvider().get() != nullptr) {
    GTEST_SKIP() << "Skipping because of the following error: Dimension of input 0 must be 1 instead of 3. shape={3,1,4,5}";
  }

  OpTester test("Squeeze");
  test.AddShapeToTensorData(false);  // TODO: re-enable shape inference test after ONNX fix
  // Bad axes - should be 1 instead of 0.
  test.AddAttribute("axes", std::vector<int64_t>{0});
  test.AddInput<float>("data", {3, 1, 4, 5}, std::vector<float>(60, 1.0f));
  test.AddOutput<float>("squeezed", {3, 4, 5}, std::vector<float>(60, 1.0f));

  // Expect failure.
  test.Run(OpTester::ExpectResult::kExpectFailure, "Dimension of input 0 must be 1 instead of 3", {kTensorrtExecutionProvider, kOpenVINOExecutionProvider});
}

TEST(SqueezeOpTest, SqueezeNegAxis_2) {
  OpTester test("Squeeze", 11);
  test.AddAttribute("axes", std::vector<int64_t>{0, -3, -2});
  test.AddInput<float>("data", {1, 4, 1, 1, 2},
                       std::vector<float>{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f});
  test.AddOutput<float>("squeezed", {4, 2},
                        std::vector<float>{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f});

  // OpenVINO EP Incorrect precision. Will be re-enabled after its fixed
  test.Run(OpTester::ExpectResult::kExpectSuccess, "", {kOpenVINOExecutionProvider});
}

TEST(SqueezeOpTest, Squeeze_2_axes_input) {
  auto run_test = [](bool axes_is_initializer) {
    OpTester test("Squeeze", 13);
    test.AddInput<float>("data", {1, 4, 1, 1, 2},
                         std::vector<float>{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f});
    test.AddInput<int64_t>("axes", {3}, std::vector<int64_t>{0, 2, 3}, axes_is_initializer);
    test.AddOutput<float>("squeezed", {4, 2},
                          std::vector<float>{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f});
    // Incorrect precision for OpenVINO EP. Will be re-enabled after it's fixed
    // TensorRT and OpenVINO dont support "axes" input in opset 13, re-enable after
    test.Run(OpTester::ExpectResult::kExpectSuccess, "", {kOpenVINOExecutionProvider, kTensorrtExecutionProvider});
  };

  run_test(false);
  run_test(true);  // COREML EP will need axes as an initializer
}

TEST(SqueezeOpTest, Squeeze_Empty_Axes_opset13) {
  // TODO: Unskip when fixed #41968513
  if (DefaultDmlExecutionProvider().get() != nullptr) {
    GTEST_SKIP() << "Skipping because of the following error: MLOperatorAuthorImpl.cpp(2068): Catastrophic failure";
  }

  OpTester test("Squeeze", 13);
  test.AddInput<float>("data", {1, 1, 4, 1}, std::vector<float>(4, 1.0f));
  test.AddOutput<float>("squeezed", {4}, std::vector<float>(4, 1.0f));
  // TensorRT doesn't seem to support missing 'axes'
  test.Run(OpTester::ExpectResult::kExpectSuccess, "", {kTensorrtExecutionProvider});
}

TEST(SqueezeOpTest, SqueezeNegAxis_axes_input) {
  auto run_test = [](bool axes_is_initializer) {
    OpTester test("Squeeze", 13);
    test.AddInput<float>("data", {1, 4, 1, 1, 2},
                         std::vector<float>{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f});

    test.AddInput<int64_t>("axes", {3}, std::vector<int64_t>{0, -3, -2}, axes_is_initializer);
    test.AddOutput<float>("squeezed", {4, 2},
                          std::vector<float>{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f});

    // OpenVINO EP Incorrect precision. Will be re-enabled after its fixed
    // TensorRT and OpenVINO dont support "axes" input in opset 13, re-enable after
    test.Run(OpTester::ExpectResult::kExpectSuccess, "", {kOpenVINOExecutionProvider, kTensorrtExecutionProvider});
  };

  run_test(false);
  run_test(true);  // COREML EP will need axes as an initializer
}

// Add 4d input shape test, since NNAPI supports up to 4d input shape
TEST(SqueezeOpTest, Squeeze_4d_2_axes_input) {
  auto run_test = [](bool axes_is_initializer) {
    OpTester test("Squeeze", 13);
    test.AddInput<float>("data", {1, 4, 1, 2},
                         std::vector<float>{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f});
    test.AddInput<int64_t>("axes", {2}, std::vector<int64_t>{0, 2}, axes_is_initializer);
    test.AddOutput<float>("squeezed", {4, 2},
                          std::vector<float>{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f});
    // Incorrect precision for OpenVINO EP. Will be re-enabled after it's fixed
    // TensorRT and OpenVINO dont support "axes" input in opset 13, re-enable after
    test.Run(OpTester::ExpectResult::kExpectSuccess, "", {kOpenVINOExecutionProvider, kTensorrtExecutionProvider});
  };

  run_test(false);
  run_test(true);  // NNAPI EP will need axes as an initializer
}

// Add 4d input shape test, since NNAPI supports up to 4d input shape
TEST(SqueezeOpTest, Squeeze_4d_NegAxis_axes_input) {
  auto run_test = [](bool axes_is_initializer) {
    OpTester test("Squeeze", 13);
    test.AddInput<float>("data", {1, 4, 1, 2},
                         std::vector<float>{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f});

    test.AddInput<int64_t>("axes", {2}, std::vector<int64_t>{0, -2}, axes_is_initializer);
    test.AddOutput<float>("squeezed", {4, 2},
                          std::vector<float>{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f});

    // OpenVINO EP Incorrect precision. Will be re-enabled after its fixed
    // TensorRT and OpenVINO dont support "axes" input in opset 13, re-enable after
    test.Run(OpTester::ExpectResult::kExpectSuccess, "", {kOpenVINOExecutionProvider, kTensorrtExecutionProvider});
  };

  run_test(false);
  run_test(true);  // NNAPI EP will need axes as an initializer
}
#if defined(USE_DNNL)
TEST(SqueezeOpTest, Squeeze_opset13_1_bfloat16) {
#ifdef USE_DNNL
  if (!DnnlHasBF16Support()) {
    LOGS_DEFAULT(WARNING) << "Hardware does NOT support BF16";
    return;
  }
#endif
  OpTester test("Squeeze", 13);
  test.AddInput<BFloat16>("data", {1, 3, 4, 5}, FloatsToBFloat16s(std::vector<float>(60, 1.0f)));
  test.AddOutput<BFloat16>("squeezed", {3, 4, 5}, FloatsToBFloat16s(std::vector<float>(60, 1.0f)));
  std::vector<std::unique_ptr<IExecutionProvider>> execution_providers;
#if defined(USE_DNNL)
  execution_providers.push_back(DefaultDnnlExecutionProvider());
#endif  //  USE_DNNL
  test.Run(OpTester::ExpectResult::kExpectSuccess, "", {}, nullptr, &execution_providers);
}

TEST(SqueezeOpTest, Squeeze_opset13_2_bfloat16) {
#ifdef USE_DNNL
  if (!DnnlHasBF16Support()) {
    LOGS_DEFAULT(WARNING) << "Hardware does NOT support BF16";
    return;
  }
#endif
  OpTester test("Squeeze", 13);
  // test.AddAttribute("axes", std::vector<int64_t>{0, 2, 3});
  test.AddInput<BFloat16>("data", {1, 4, 1, 1, 2},
                          MakeBFloat16({1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f}));
  test.AddInput<int64_t>("axes", {3}, {0, 2, 3}, true);  // only support initializer input at axes.
  test.AddOutput<BFloat16>("squeezed", {4, 2},
                           MakeBFloat16({1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f}));
  std::vector<std::unique_ptr<IExecutionProvider>> execution_providers;
#if defined(USE_DNNL)
  execution_providers.push_back(DefaultDnnlExecutionProvider());
#endif                                                                                                                //  USE_DNNL
  test.Run(OpTester::ExpectResult::kExpectSuccess, "", {kOpenVINOExecutionProvider}, nullptr, &execution_providers);  // Incorrect precision. Will be re-enabled after it's fixed
}

TEST(SqueezeOpTest, Squeeze_Empty_Axes_1_bfloat16) {
#ifdef USE_DNNL
  if (!DnnlHasBF16Support()) {
    LOGS_DEFAULT(WARNING) << "Hardware does NOT support BF16";
    return;
  }
#endif
  OpTester test("Squeeze", 13);
  test.AddInput<BFloat16>("data", {1, 1, 4, 1}, FloatsToBFloat16s(std::vector<float>(4, 1.0f)));
  test.AddOutput<BFloat16>("squeezed", {4}, FloatsToBFloat16s(std::vector<float>(4, 1.0f)));
  // TensorRT doesn't seem to support missing 'axes'
  std::vector<std::unique_ptr<IExecutionProvider>> execution_providers;
#if defined(USE_DNNL)
  execution_providers.push_back(DefaultDnnlExecutionProvider());
#endif  //  USE_DNNL
  test.Run(OpTester::ExpectResult::kExpectSuccess, "", {kTensorrtExecutionProvider}, nullptr, &execution_providers);
}

TEST(SqueezeOpTest, Squeeze_Empty_Axes_2_bfloat16) {
#ifdef USE_DNNL
  if (!DnnlHasBF16Support()) {
    LOGS_DEFAULT(WARNING) << "Hardware does NOT support BF16";
    return;
  }
#endif
  OpTester test("Squeeze", 13);
  // nothing to "squeeze" out in the input shape
  test.AddInput<BFloat16>("data", {2, 4}, FloatsToBFloat16s(std::vector<float>(8, 1.0f)));
  test.AddOutput<BFloat16>("squeezed", {2, 4}, FloatsToBFloat16s(std::vector<float>(8, 1.0f)));
  // TensorRT doesn't seem to support missing 'axes'
  std::vector<std::unique_ptr<IExecutionProvider>> execution_providers;
#if defined(USE_DNNL)
  execution_providers.push_back(DefaultDnnlExecutionProvider());
#endif  //  USE_DNNL
  test.Run(OpTester::ExpectResult::kExpectSuccess, "", {kTensorrtExecutionProvider}, nullptr, &execution_providers);
}

TEST(SqueezeOpTest, Squeeze_Empty_Axes_3_bfloat16) {
#ifdef USE_DNNL
  if (!DnnlHasBF16Support()) {
    LOGS_DEFAULT(WARNING) << "Hardware does NOT support BF16";
    return;
  }
#endif
  OpTester test("Squeeze", 13);
  test.AddInput<BFloat16>("data", {1, 1, 1, 1}, FloatsToBFloat16s(std::vector<float>{1.0f}));
  test.AddOutput<BFloat16>("squeezed", {}, FloatsToBFloat16s(std::vector<float>{1.0f}));
  // DNNL ep does not support empty axis input.
  test.Run(OpTester::ExpectResult::kExpectSuccess, "", {kTensorrtExecutionProvider, kDnnlExecutionProvider});
}

TEST(SqueezeOpTest, UnsortedAxes_opset13_bfloat16) {
#ifdef USE_DNNL
  if (!DnnlHasBF16Support()) {
    LOGS_DEFAULT(WARNING) << "Hardware does NOT support BF16";
    return;
  }
#endif
  OpTester test("Squeeze", 13);
  test.AddInput<BFloat16>("data", {1, 4, 1, 1, 2},
                          MakeBFloat16({1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f}));
  test.AddInput<int64_t>("axes", {3}, std::vector<int64_t>{3, 0, 2}, true);
  test.AddOutput<BFloat16>("squeezed", {4, 2},
                           MakeBFloat16({1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f}));
  std::vector<std::unique_ptr<IExecutionProvider>> execution_providers;
#if defined(USE_DNNL)
  execution_providers.push_back(DefaultDnnlExecutionProvider());
#endif                                                                                                                //  USE_DNNL
  test.Run(OpTester::ExpectResult::kExpectSuccess, "", {kOpenVINOExecutionProvider}, nullptr, &execution_providers);  // Incorrect precision. Will be re-enabled after it's fixed
}

TEST(SqueezeOpTest, DuplicateAxes_opset13_bfloat16) {
#ifdef USE_DNNL
  if (!DnnlHasBF16Support()) {
    LOGS_DEFAULT(WARNING) << "Hardware does NOT support BF16";
    return;
  }
#endif
  OpTester test("Squeeze", 13);
  test.AddInput<BFloat16>("data", {1, 4, 1, 1, 2},
                          MakeBFloat16({1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f}));
  test.AddInput<int64_t>("axes", {6}, std::vector<int64_t>{3, 0, 2, 0, 2, 3}, true);
  test.AddOutput<BFloat16>("squeezed", {4, 2},
                           MakeBFloat16({1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f}));
  std::vector<std::unique_ptr<IExecutionProvider>> execution_providers;
#if defined(USE_DNNL)
  execution_providers.push_back(DefaultDnnlExecutionProvider());
#endif                                                                                                                //  USE_DNNL
  test.Run(OpTester::ExpectResult::kExpectSuccess, "", {kOpenVINOExecutionProvider}, nullptr, &execution_providers);  // Incorrect precision. Will be re-enabled after it's fixed
}

TEST(SqueezeOpTest, BadAxes_opset13_bfloat16) {
#ifdef USE_DNNL
  if (!DnnlHasBF16Support()) {
    LOGS_DEFAULT(WARNING) << "Hardware does NOT support BF16";
    return;
  }
#endif
  OpTester test("Squeeze", 13);
  // Bad axes - should be 1 instead of 0.
  test.AddInput<BFloat16>("data", {3, 1, 4, 5}, FloatsToBFloat16s(std::vector<float>(60, 1.0f)));
  test.AddInput<int64_t>("axes", {1}, std::vector<int64_t>{0}, true);
  test.AddOutput<BFloat16>("squeezed", {3, 4, 5}, FloatsToBFloat16s(std::vector<float>(60, 1.0f)));

  // Expect failure.
  std::vector<std::unique_ptr<IExecutionProvider>> execution_providers;
#if defined(USE_DNNL)
  execution_providers.push_back(DefaultDnnlExecutionProvider());
#endif                                                                                                                                                           //  USE_DNNL
  test.Run(OpTester::ExpectResult::kExpectFailure, "Dimension of input 0 must be 1 instead of 3", {kTensorrtExecutionProvider}, nullptr, &execution_providers);  // Incorrect precision. Will be re-enabled after it's fixed
}

TEST(SqueezeOpTest, SqueezeNegAxis_2_opset13_bfloat16) {
#ifdef USE_DNNL
  if (!DnnlHasBF16Support()) {
    LOGS_DEFAULT(WARNING) << "Hardware does NOT support BF16";
    return;
  }
#endif
  OpTester test("Squeeze", 13);
  test.AddInput<BFloat16>("data", {1, 4, 1, 1, 2},
                          MakeBFloat16({1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f}));
  test.AddInput<int64_t>("axes", {3}, std::vector<int64_t>{0, -3, -2}, true);
  test.AddOutput<BFloat16>("squeezed", {4, 2},
                           MakeBFloat16({1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f}));
  std::vector<std::unique_ptr<IExecutionProvider>> execution_providers;
#if defined(USE_DNNL)
  execution_providers.push_back(DefaultDnnlExecutionProvider());
#endif                                                                                                                //  USE_DNNL
  test.Run(OpTester::ExpectResult::kExpectSuccess, "", {kOpenVINOExecutionProvider}, nullptr, &execution_providers);  // Incorrect precision. Will be re-enabled after it's fixed
}
#endif  //  USE_DNNL

}  // namespace test
}  // namespace onnxruntime
