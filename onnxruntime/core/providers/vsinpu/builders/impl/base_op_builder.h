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
#pragma once

#include "core/providers/vsinpu/builders/op_builder.h"
#include "core/providers/vsinpu/vsinpu_ep_graph.h"
#include "tim/vx/operation.h"
#include "tim/vx/ops.h"

namespace onnxruntime {
namespace vsi {
namespace npu {
class BaseOpBuilder : public IOpBuilder {
 public:
  virtual ~BaseOpBuilder() = default;

  bool IsSupported(const onnxruntime::GraphViewer& graph_viewer,
                   const Node* node) const override;
  bool BuildOp(vsi::npu::GraphEP* graph_ep,
               const onnxruntime::GraphViewer& graph_viewer, const Node* node);
  virtual bool IsOpSupported(const onnxruntime::GraphViewer& graph_viewer,
                             const Node* node) const {
    return true;
  }
  virtual bool HandleBuildOp(
      vsi::npu::GraphEP* graph_ep,
      std::vector<std::shared_ptr<tim::vx::Tensor>>& inputs,
      std::vector<std::shared_ptr<tim::vx::Tensor>>& outputs,
      const Node* node) {
    return true;
  }
};
}  // namespace npu
}  // namespace vsi
}  // namespace onnxruntime
