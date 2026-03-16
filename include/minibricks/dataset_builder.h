#ifndef MINIBRICKS_DATASET_BUILDER_H_
#define MINIBRICKS_DATASET_BUILDER_H_

#include "minibricks/db.h"
#include "minibricks/types.h"

namespace minibricks {

class DatasetBuilder {
public:
    explicit DatasetBuilder(Database* db) : db_(db) {}

    Dataset LoadTrainingDataset() const;
    std::vector<double> LoadFeatureVectorForUser(int64_t user_id) const;

private:
    Database* db_;
}
}

#endif  // MINIBRICKS_DATASET_BUILDER_H_