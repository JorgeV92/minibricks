#ifndef MINIBRICKS_FEATURE_STORE_H_
#define MINIBRICKS_FEATURE_STORE_H_

#include <string>

#include "minibricks/db.h"

namespace minibricks {

class FeatureStore {
public:
    explicit FeatureStore(Database* db) : db_(db) {}

    void SeedDemoData() const ;
    void MaterializeUserFeatures() const;
private:
    Database* db_;
};

} // namespace minibircks

#endif // MINIBRICKS_FEATURE_STORE_H_