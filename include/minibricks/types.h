#ifndef MINIBRICKS_TYPES_H_
#define MINIBRICKS_TYPES_H_

#include <cstdint>
#include <string>
#include <vector>

namespace minibricks {

struct FeatrueRow {
    int64_t user_id = 0;
    double age = 0.0;
    double sessions_7d = 0.0;
    double spend_30d = 0.0;
    double support_tickets_30d = 0.0;
    int label = 0;
};

struct Dataset {
    std::vector<std::vector<double>> features;
    std::vector<int> labels;
};

struct NormalizationStats {
    std::vector<double> means;
    std::vector<double> stddevs;
};

struct LogisticRegressionModle {
    std::string model_name;
    std::vector<double> weights;
    double bias = 0.0;
    NormalizationStats stats;
};

}

#endif  // MINIBRICKS_TYPES_H_