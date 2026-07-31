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

#include <QImage>

#include "BlockListManager/BlockListManager.h"

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
  // load preset
  const auto preset =
      load_preset(QString::fromStdString(task.preset_json.string()));

  auto color_table = [&] {
    color_table_create_info ctci{};
    ctci.map_type = task.map_type;
    ctci.mc_version = task.version;
    std::ranges::fill(ctci.blocks, nullptr);
    std::ranges::fill(ctci.basecolor_allow_LUT, false);
    for (size_t basecolor = 0; basecolor < 64; basecolor++) {
      if (total_blocks.size() <= basecolor or
          preset.values.size() <= basecolor) {
        continue;
      }
      const auto& candidates = total_blocks[basecolor];
      if (candidates.empty()) {
        std::println("Base color {} is disabled because no available block",
                     basecolor);
        continue;
      }
      const auto& expected_blk = preset.values[basecolor];
      if (not expected_blk.first) {  // The preset disabled this
        std::println("Base color {} is disabled due to preset", basecolor);
        continue;
      }

      bool found_matched_blocks = false;
      for (const auto blkp : candidates) {
        if (QString::fromUtf8(blkp->getId()) == expected_blk.second) {
          std::println("Selecting {} for base color {}", blkp->getId(),
                       basecolor);
          ctci.blocks[basecolor] = blkp;
          ctci.basecolor_allow_LUT[basecolor] = true;
          found_matched_blocks = true;
          break;
        }
      }
      if (found_matched_blocks) {
        continue;
      }
      // select most preferred block
      auto selected = candidates[0];
      std::println(
          "Selecting {} for base color {} instead of preset assigned {} "
          "(missing from block list, or not available in this version)",
          selected->getId(), basecolor, expected_blk.second.toStdString());
      ctci.blocks[basecolor] = selected;
      ctci.basecolor_allow_LUT[basecolor] = true;
    }

    std::unique_ptr<SlopeCraft::color_table, deleter> ptr{
      SCL_create_color_table(ctci)};
    return ptr;
  }();

  std::vector<std::unique_ptr<converted_image, deleter>> converted_images;
  for (const auto& [idx, img_path] : task.images | std::views::enumerate) {
    QImage img;
    if (not img.load(QString::fromStdString(img_path.string()))) {
      throw std::runtime_error{
        std::format("Failed to load image {}", img_path.string())};
    }
    img = img.convertToFormat(QImage::Format_ARGB32);

    const_image_reference raw_img_ref{
      .data = reinterpret_cast<const uint32_t*>(img.constScanLine(0)),
      .rows = static_cast<size_t>(img.size().height()),
      .cols = static_cast<size_t>(img.size().width()),
    };

    std::println("[{} / {}] Converting image {}", idx + 1, task.images.size(),
                 img_path.string());
    convert_option opt;
    opt.algo = task.algo;
    opt.dither = task.dither;
    std::unique_ptr<converted_image, deleter> converted_img{
      color_table->convert_image(raw_img_ref, opt)};
    converted_images.emplace_back(std::move(converted_img));
  }

  {
    const size_t n_tasks = [&] {
      size_t sum = 0;
      if (task.converted_image_option) {
        sum += task.images.size();
      }
      if (task.map_data_files_option) {
        sum += task.images.size();
        if (task.map_data_files_option.value().assembled_option) {
          sum += task.images.size();
        }
      }
      return sum;
    }();

    size_t task_counter = 1;
    int map_idx_counter = task.map_data_files_option.value_or({}).begin_index;
    for (const auto& [idx, pair] :
         std::views::zip(task.images, converted_images) |
             std::views::enumerate) {
      const auto& [raw_img_path, converted_img] = pair;
      if (task.converted_image_option) {
        QImage img{QSize{static_cast<int>(converted_img->cols()),
                         static_cast<int>(converted_img->rows())},
                   QImage::Format_ARGB32};
        const auto dest_img_path = task.export_prefix / raw_img_path.filename();
        std::println("[{} / {}] Saving converted image {}", task_counter++,
                     n_tasks, dest_img_path.string());
        converted_img->get_converted_image(
            reinterpret_cast<uint32_t*>(img.scanLine(0)));
        if (not img.save(QString::fromStdString(dest_img_path.string()))) {
          throw std::runtime_error{std::format(
              "Failed to save converted image {}", dest_img_path.string())};
        }
      }

      if (task.map_data_files_option) {
        const auto& mdfo = task.map_data_files_option.value();
        map_data_file_options opt{};
        opt.begin_index = map_idx_counter;
        const auto path_str = task.export_prefix.string();
        opt.folder_path = path_str.c_str();
        std::println("[{} / {}] Saving map data files for image {}",
                     task_counter++, n_tasks, raw_img_path.string());
        if (not converted_img->export_map_data(opt)) {
          throw std::runtime_error{
            std::format("Failed to export map data files for image {}",
                        raw_img_path.string())};
        }
#warning "TODO: Save assembled maps"

        {
          size_t map_rows, map_cols;
          SCL_get_map_count(converted_img->rows(), converted_img->cols(),
                            map_rows, map_cols);
          map_idx_counter += (map_rows * map_cols);
        }
      }
    }
  }
}