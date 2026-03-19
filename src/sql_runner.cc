#include "minibricks/sql_runner.h"

namespace minibricks {

void SqlRunner::InitializePragmas() const {
  db_->Execute("PRAGMA foreign_keys = ON;");
  db_->Execute("PRAGMA journal_mode = WAL;");
  db_->Execute("PRAGMA synchronous = NORMAL;");
  db_->Execute("PRAGMA temp_store = MEMORY;");
}

void SqlRunner::BootstrapSchema(const std::string& schema_path) const {
  InitializePragmas();
  db_->ExecuteFile(schema_path);
}

}  // namespace minibricks
