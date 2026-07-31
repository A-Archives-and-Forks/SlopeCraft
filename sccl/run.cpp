/*
Copyright © 2021-2026  TokiNoBug
    This file is part of SlopeCraft.

    SlopeCraft is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SlopeCraft is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SlopeCraft. If not, see <https://www.gnu.org/licenses/>.

    Contact with me:
    github:https://github.com/SlopeCraft/SlopeCraft
*/
#include <memory>
#include <print>
#include <cassert>
#include <ranges>

#include "sccl_internal.h"

void run(const inputs& task, bool build_dir_mode) {
  const std::filesystem::path SC_default_blocks_dir =
#ifdef __linux__
      "../share/SlopeCraft/Blocks_SCL";
#else
      "./Blocks_SCL";
#endif
  const std::filesystem::path SCL_blocks_dir =
      build_dir_mode ? "../SCL_block_lists" : SC_default_blocks_dir;
  using namespace SlopeCraft;
  // load block lists from zips
  const auto block_lists = [&] {
    std::vector<std::unique_ptr<block_list_interface, deleter>> ret;
    for (std::string_view filename : {"FixedBlocks.zip", "CustomBlocks.zip"}) {
      const auto path = SCL_blocks_dir / filename;
      std::string warnings, errors;
      warnings.resize(8192);
      errors.resize(8192);
      string_deliver sd_warnings{warnings.data(), warnings.size()},
          sd_errors{errors.data(), errors.size()};
      block_list_create_info blci;
      blci.warnings = &sd_warnings;
      blci.error = &sd_errors;
      std::unique_ptr<block_list_interface, deleter> ptr{
        SCL_create_block_list(path.string().c_str(), blci)};
      warnings.resize(sd_warnings.size);
      errors.resize(sd_errors.size);
      if (not warnings.empty()) {
        std::println("Warnings when parsing block list {}: {}", path.string(),
                     warnings);
      }
      if (ptr == nullptr) {
        throw std::runtime_error{std::format(
            "Failed to parse block list {}: {}", path.string(), errors)};
      }
      ret.emplace_back(std::move(ptr));
    }
    return ret;
  }();
  // Arrange blocks from zips
  const auto total_blocks = [&block_lists, &task] {
    std::vector<std::vector<mc_block_interface*>> total_blocks{64};
    for (auto& blp : block_lists) {
      std::vector<mc_block_interface*> blocks;
      std::vector<uint8_t> basecolors;
      blocks.resize(blp->size(), nullptr);
      basecolors.resize(blp->size(), 0);
      const auto n_blocks [[maybe_unused]] =
          blp->get_blocks(blocks.data(), basecolors.data(), basecolors.size());
      assert(n_blocks == blocks.size());
      assert(n_blocks == basecolors.size());

      for (const auto [blkp, base_color] :
           std::views::zip(blocks, basecolors)) {
        if (task.version >= blkp->getVersion()) {
          total_blocks[base_color].emplace_back(blkp);
        }
      }
    }
    return total_blocks;
  }();
}