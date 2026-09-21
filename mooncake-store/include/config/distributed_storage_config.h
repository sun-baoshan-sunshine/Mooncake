#pragma once

#include <chrono>
#include <cstdint>
#include <string>

namespace mooncake {

struct DistributedStorageConfig {
    std::string fsdir = "/mnt/3fs/mooncake";
    std::string fs_adapter_type = "hf3fs";
    bool enable_health_check = false;
    int shard_count = 64;
    uint64_t shard_capacity = 4ULL * 1024 * 1024 * 1024;
    uint64_t alignment = 4096;
    // When true, DFS shard files are opened with O_DIRECT and positional I/O
    // is issued with buffers/offsets/lengths aligned to `alignment`. The client
    // staging path produces aligned, `aligned_size`-sized buffers so the direct
    // path is taken; a misaligned request is rejected rather than silently
    // bounced through a buffered copy. Direct I/O also fsyncs each shard after a
    // batch write (before the objects are published) so peer nodes observe the
    // bytes; the non direct-I/O path keeps its original no-sync behavior.
    bool use_direct_io = false;
    bool single_tenant = true;
    bool eviction_enabled = true;
    double eviction_high_watermark = 0.9;
    double eviction_low_watermark = 0.7;
    std::chrono::seconds deferred_free_duration{30};
    std::chrono::seconds eviction_check_interval{5};

    bool Validate() const;
    bool ValidateForAllocator() const;
    static DistributedStorageConfig FromEnvironment();
    std::string FormatStr() const;
};

}  // namespace mooncake
