/****************************************************************************
 *
 *    Copyright (c) 2023 Vivante Corporation
 *
 *    Permission is hereby granted, free of charge, to any person obtaining a
 *    copy of this software and associated documentation files (the "Software"),
 *    to deal in the Software without restriction, including without limitation
 *    the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *    and/or sell copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following conditions:
 *
 *    The above copyright notice and this permission notice shall be included in
 *    all copies or substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *    DEALINGS IN THE SOFTWARE.
 *
 *****************************************************************************/
#include "core/providers/shared/utils/utils.h"
#include "core/providers/vsinpu/builders/impl/base_op_builder.h"

namespace onnxruntime {
namespace vsi {
namespace npu {
class ReluOpBuilder : public BaseOpBuilder {
 public:
  bool HandleBuildOp(vsi::npu::GraphEP* graph_ep,
                     std::vector<std::shared_ptr<tim::vx::Tensor>>& inputs,
                     std::vector<std::shared_ptr<tim::vx::Tensor>>& outputs,
                     const Node* node) override {
    LOGS_DEFAULT(VERBOSE) << "Creating Relu Activation.";
    auto op = graph_ep->GetGraph()->CreateOperation<tim::vx::ops::Relu>();
    auto node_info = graph_ep->ConstructNodeIO(std::move(op), util::RemoveWrapper(node->InputDefs()), util::RemoveWrapper(node->OutputDefs()));
    graph_ep->GetOps().push_back(node_info);
    return true;
  }
};
class SigmoidOpBuilder : public BaseOpBuilder {
 public:
  bool HandleBuildOp(vsi::npu::GraphEP* graph_ep,
                     std::vector<std::shared_ptr<tim::vx::Tensor>>& inputs,
                     std::vector<std::shared_ptr<tim::vx::Tensor>>& outputs,
                     const Node* node) override {
    LOGS_DEFAULT(VERBOSE) << "Creating Sigmoid Activation.";
    auto op = graph_ep->GetGraph()->CreateOperation<tim::vx::ops::Sigmoid>();
    auto node_info = graph_ep->ConstructNodeIO(std::move(op), util::RemoveWrapper(node->InputDefs()), util::RemoveWrapper(node->OutputDefs()));
    graph_ep->GetOps().push_back(node_info);
    return true;
  }
};
class TanhOpBuilder : public BaseOpBuilder {
 public:
  bool HandleBuildOp(vsi::npu::GraphEP* graph_ep,
                     std::vector<std::shared_ptr<tim::vx::Tensor>>& inputs,
                     std::vector<std::shared_ptr<tim::vx::Tensor>>& outputs,
                     const Node* node) override {
    LOGS_DEFAULT(VERBOSE) << "Creating Tanh activation.";
    auto op = graph_ep->GetGraph()->CreateOperation<tim::vx::ops::Tanh>();
    auto node_info = graph_ep->ConstructNodeIO(std::move(op), util::RemoveWrapper(node->InputDefs()), util::RemoveWrapper(node->OutputDefs()));
    graph_ep->GetOps().push_back(node_info);
    return true;
  }
};

class LeakyReluOpBuilder : public BaseOpBuilder {
 public:
  bool HandleBuildOp(vsi::npu::GraphEP* graph_ep,
                     std::vector<std::shared_ptr<tim::vx::Tensor>>& inputs,
                     std::vector<std::shared_ptr<tim::vx::Tensor>>& outputs,
                     const Node* node) override {
    LOGS_DEFAULT(VERBOSE) << "Creating LeakyRelu activation.";
    NodeAttrHelper helper(*node);
    auto alpha = helper.Get("alpha", 1.0f);
    auto op =
        graph_ep->GetGraph()->CreateOperation<tim::vx::ops::LeakyRelu>(alpha);
    auto node_info = graph_ep->ConstructNodeIO(std::move(op), util::RemoveWrapper(node->InputDefs()), util::RemoveWrapper(node->OutputDefs()));
    graph_ep->GetOps().push_back(node_info);
    return true;
  }
};

class EluOpBuilder : public BaseOpBuilder {
 public:
  bool HandleBuildOp(vsi::npu::GraphEP* graph_ep,
                     std::vector<std::shared_ptr<tim::vx::Tensor>>& inputs,
                     std::vector<std::shared_ptr<tim::vx::Tensor>>& outputs,
                     const Node* node) override {
    LOGS_DEFAULT(VERBOSE) << "Creating Elu activation.";
    NodeAttrHelper helper(*node);
    auto alpha = helper.Get("alpha", 1.0f);
    auto op =
        graph_ep->GetGraph()->CreateOperation<tim::vx::ops::LeakyRelu>(alpha);
    auto node_info = graph_ep->ConstructNodeIO(std::move(op), util::RemoveWrapper(node->InputDefs()), util::RemoveWrapper(node->OutputDefs()));
    graph_ep->GetOps().push_back(node_info);
    return true;
  }
};

class HardSigmoidOpBuilder : public BaseOpBuilder {
 public:
  bool HandleBuildOp(vsi::npu::GraphEP* graph_ep,
                     std::vector<std::shared_ptr<tim::vx::Tensor>>& inputs,
                     std::vector<std::shared_ptr<tim::vx::Tensor>>& outputs,
                     const Node* node) override {
    LOGS_DEFAULT(VERBOSE) << "Creating HardSigmoid activation.";
    NodeAttrHelper helper(*node);
    auto alpha = helper.Get("alpha", 1.0f);
    auto beta = helper.Get("beta", 1.0f);
    auto op = graph_ep->GetGraph()->CreateOperation<tim::vx::ops::HardSigmoid>(
        alpha, beta);
    auto node_info = graph_ep->ConstructNodeIO(std::move(op), util::RemoveWrapper(node->InputDefs()), util::RemoveWrapper(node->OutputDefs()));
    graph_ep->GetOps().push_back(node_info);
    return true;
  }
};
}  // namespace npu

}  // namespace vsi
}  // namespace onnxruntime
