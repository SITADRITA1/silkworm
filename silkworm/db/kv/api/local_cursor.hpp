/*
   Copyright 2023 The Silkworm Authors

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#pragma once

#include <memory>
#include <string>
#include <utility>

#include <silkworm/infra/concurrency/task.hpp>

#include <boost/asio/use_awaitable.hpp>

#include <silkworm/core/common/util.hpp>
#include <silkworm/db/datastore/kvdb/mdbx.hpp>

#include "cursor.hpp"

namespace silkworm::db::kv::api {

class LocalCursor : public CursorDupSort {
  public:
    LocalCursor(mdbx::txn& txn, uint32_t cursor_id) : cursor_id_{cursor_id}, txn_{txn} {}

    uint32_t cursor_id() const override { return cursor_id_; };

    Task<void> open_cursor(const std::string& table_name, bool is_dup_sorted) override;

    Task<KeyValue> seek(ByteView key) override;

    Task<KeyValue> seek_exact(ByteView key) override;

    Task<KeyValue> first() override;

    Task<KeyValue> last() override;

    Task<KeyValue> next() override;

    Task<KeyValue> previous() override;

    Task<KeyValue> next_dup() override;

    Task<void> close_cursor() override;

    Task<silkworm::Bytes> seek_both(ByteView key, ByteView value) override;

    Task<KeyValue> seek_both_exact(ByteView key, ByteView value) override;

  private:
    uint32_t cursor_id_;
    datastore::kvdb::PooledCursor db_cursor_;
    mdbx::txn& txn_;
};

}  // namespace silkworm::db::kv::api
