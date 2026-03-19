#ifndef MINIBRICKS_SQL_RUNNER_H_
#define MINIBRICKS_SQL_RUNNER_H_

#include <string>

#include "minibricks/db.h"

namespace minibricks {

class SqlRunner {
 public:
  explicit SqlRunner(Database* db) : db_(db) {}

  void InitializePragmas() const;
  void BootstrapSchema(const std::string& schema_path) const;

 private:
  Database* db_;
};

}  // namespace minibricks

#endif  // MINIBRICKS_SQL_RUNNER_H_
