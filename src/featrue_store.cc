#include "minibricks/feature_store.h"

#include <array>
#include <string>
#include <vector>

namespace minibricks {

void FeatureStore::SeedDemoData() const {
  db_->WithTransaction([this]() {
    Statement insert_user(
        db_->Handle(),
        "INSERT OR REPLACE INTO users(id, name, age, country, created_at) "
        "VALUES(?, ?, ?, ?, datetime('now'));"
    );

    struct UserSeed {
      int64_t id;
      const char* name;
      int age;
      const char* country;
    };

    const std::array<UserSeed, 8> users = {{{1, "Ada", 24, "US"},
                                            {2, "Grace", 31, "US"},
                                            {3, "Linus", 28, "SE"},
                                            {4, "Barbara", 36, "US"},
                                            {5, "Ken", 41, "CA"},
                                            {6, "Margaret", 33, "UK"},
                                            {7, "Edsger", 29, "NL"},
                                            {8, "Donald", 45, "US"}}};

    for (const auto& user : users) {
      insert_user.BindInt64(1, user.id);
      insert_user.BindText(2, user.name);
      insert_user.BindInt(3, user.age);
      insert_user.BindText(4, user.country);
      static_cast<void>(insert_user.Step());
      insert_user.Reset();
      insert_user.ClearBindings();
    }

    Statement insert_event(
        db_->Handle(),
        "INSERT INTO user_events(user_id, event_type, amount, ts) "
        "VALUES(?, ?, ?, datetime('now', ?));");

    struct EventSeed {
      int64_t user_id;
      const char* event_type;
      double amount;
      const char* offset;
    };

    const std::vector<EventSeed> events = {
        {1, "session", 1.0, "-1 day"},
        {1, "purchase", 29.0, "-2 day"},
        {1, "session", 1.0, "-3 day"},
        {2, "session", 1.0, "-1 day"},
        {2, "support_ticket", 1.0, "-10 day"},
        {3, "session", 1.0, "-1 day"},
        {3, "session", 1.0, "-2 day"},
        {3, "session", 1.0, "-3 day"},
        {4, "purchase", 180.0, "-5 day"},
        {4, "session", 1.0, "-1 day"},
        {4, "support_ticket", 1.0, "-4 day"},
        {5, "session", 1.0, "-14 day"},
        {5, "purchase", 12.0, "-20 day"},
        {6, "session", 1.0, "-1 day"},
        {6, "purchase", 90.0, "-2 day"},
        {6, "session", 1.0, "-3 day"},
        {7, "support_ticket", 1.0, "-2 day"},
        {7, "session", 1.0, "-9 day"},
        {8, "purchase", 300.0, "-1 day"},
        {8, "session", 1.0, "-1 day"},
        {8, "session", 1.0, "-2 day"}};

    for (const auto& event : events) {
      insert_event.BindInt64(1, event.user_id);
      insert_event.BindText(2, event.event_type);
      insert_event.BindDouble(3, event.amount);
      insert_event.BindText(4, event.offset);
      static_cast<void>(insert_event.Step());
      insert_event.Reset();
      insert_event.ClearBindings();
    }
  });
}

} // namespace minibricks