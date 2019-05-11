/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef TENSORFLOW_CORE_KERNELS_DATA_MAP_DATASET_OP_H_
#define TENSORFLOW_CORE_KERNELS_DATA_MAP_DATASET_OP_H_

#include "tensorflow/core/framework/dataset.h"
#include "tensorflow/core/kernels/data/captured_function.h"

namespace tensorflow {
namespace data {

class MapDatasetOp : public UnaryDatasetOpKernel {
 public:
  using MapIteratorFunction =
      std::function<Status(IteratorContext*, InstantiatedCapturedFunction*,
                           std::vector<Tensor>, std::vector<Tensor>*)>;

  static constexpr char kDatasetType[] = "Map";

  explicit MapDatasetOp(OpKernelConstruction* ctx) : UnaryDatasetOpKernel(ctx) {
    FunctionMetadata::Params params;
    OP_REQUIRES_OK(ctx, ctx->GetAttr("use_inter_op_parallelism",
                                     &params.use_inter_op_parallelism));
    OP_REQUIRES_OK(ctx,
                   FunctionMetadata::Create(ctx, "f", params, &func_metadata_));
    OP_REQUIRES_OK(ctx, ctx->GetAttr("output_types", &output_types_));
    OP_REQUIRES_OK(ctx, ctx->GetAttr("output_shapes", &output_shapes_));
    OP_REQUIRES_OK(
        ctx, ctx->GetAttr("preserve_cardinality", &preserve_cardinality_));
  }

 protected:
  void MakeDataset(OpKernelContext* ctx, DatasetBase* input,
                   DatasetBase** output) override;

 private:
  class Dataset;
  std::shared_ptr<FunctionMetadata> func_metadata_ = nullptr;
  DataTypeVector output_types_;
  std::vector<PartialTensorShape> output_shapes_;
  bool preserve_cardinality_;
};

}  // namespace data
}  // namespace tensorflow

#endif  // TENSORFLOW_CORE_KERNELS_DATA_MAP_DATASET_OP_H_
