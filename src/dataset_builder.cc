#include "minibricks/dataset_builder.h"

#include <stdexcept>

namespace minibricks {

Dataset DatasetBuilder::LoadTrainingDataset() const {
  Dataset dataset;
  Statement stmt(
      db_->Handle(),
      "SELECT age, sessions_7d, spend_30d, support_tickets_30d, label "
      "FROM user_features ORDER BY user_id;");

  while (stmt.Step()) {
    dataset.features.push_back({stmt.ColumnDouble(0), stmt.ColumnDouble(1),
                                stmt.ColumnDouble(2), stmt.ColumnDouble(3)});
    dataset.labels.push_back(stmt.ColumnInt(4));
  }

  if (dataset.features.empty()) {
    throw std::runtime_error(
        "training dataset is empty; run materialize before training");
  }
  return dataset;
}

std::vector<double> DatasetBuilder::LoadFeatureVectorForUser(int64_t user_id) const {
  Statement stmt(
      db_->Handle(),
      "SELECT age, sessions_7d, spend_30d, support_tickets_30d "
      "FROM user_features WHERE user_id = ?;");
  stmt.BindInt64(1, user_id);
  if (!stmt.Step()) {
    throw std::runtime_error("user_features row not found for user");
  }
  return {stmt.ColumnDouble(0), stmt.ColumnDouble(1), stmt.ColumnDouble(2),
          stmt.ColumnDouble(3)};
}

}