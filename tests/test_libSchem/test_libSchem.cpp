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
    bilibili:https://space.bilibili.com/351429231
*/

#include <Schem/Schem.h>
#include <Schem/bit_shrink.h>

#include <iostream>
#include <vector>

using std::cout, std::endl;

void test_bit_shrink(const uint16_t* const, const size_t u16_num,
                     size_t block_types);

std::vector<std::string> generate_trash();

const std::vector<std::string> trash_id = generate_trash();

int main() {
  libSchem::Schem schem;

  schem.set_MC_major_version_number(SCL_gameVersion::MC21);
  schem.set_MC_version_number(MCDataVersion::MCDataVersion_t::Java_1_21_1);

  schem.resize(12, 9, 12);

  std::vector<const char*> ids;  //= {"minecraft:air", "minecraft:glass"};

  ids.resize(1);
  for (auto& id : ids) {
    id = "minecraft:air";
  }

  ids[0] = "minecraft:air";

  schem.set_block_id(ids.data(), ids.size());

  for (int idx = 0; idx < schem.size(); idx++) {
    schem(idx) = (idx % schem.palette_size());
  }

  int64_t err_idx;
  if (schem.have_invalid_block(&err_idx)) {
    cout << "error idx = " << err_idx << endl;
    return 1;
  }

  libSchem::litematic_info info;
  libSchem::WorldEditSchem_info weinfo;

  // test_bit_shrink(&schem(0), schem.size(), schem.block_types());

  auto check =
      [](const std::expected<void, std::pair<SCL_errorFlag, std::string>>& exp,
         std::string_view filename) {
        if (not exp) {
          auto& err = exp.error();
          std::println(std::cerr,
                       "Failed to export file {}; Error info = ", filename,
                       err.second);
          exit(1);
        }
      };

  check(schem.export_litematic("test12.litematic", info), "test12.litematic");
  check(schem.export_structure("test12.nbt", true), "test12.nbt");
  check(schem.export_WESchem("test12.schem", weinfo), "test12.schem");

  return 0;
}

void test_bit_shrink(const uint16_t* const src, const size_t u16_num,
                     size_t block_types) {
  std::vector<uint64_t> shrinked;

  cout << "Before bit shrinking : [";
  for (int idx = 0; idx < static_cast<int>(u16_num); idx++) {
    cout << src[idx] << ", ";
  }
  cout << "]" << endl;

  cout << "binary before shrinking : \n";
  for (int idx = 0; idx < static_cast<int>(u16_num); idx++) {
    uint16_t mask = (1ULL << 15);
    for (int bit = 0; bit < static_cast<int>(8 * sizeof(uint16_t)); bit++) {
      cout << ((src[idx] & mask) ? ('1') : ('0'));
      mask = mask >> 1;
    }
    cout << ' ';
  }
  cout << endl;

  ::shrink_bits(src, u16_num, block_types, &shrinked);

  cout << "binary after shirnking : \n";

  uint8_t* const data = reinterpret_cast<uint8_t*>(shrinked.data());

  for (int64_t idx = 0;
       idx < static_cast<int64_t>(shrinked.size() * sizeof(uint64_t)); idx++) {
    uint8_t mask = (1 << 7);
    const uint8_t curbyte = data[idx];
    for (int bit = 0; bit < 8; bit++) {
      cout << ((curbyte & mask) ? '1' : '0');
      mask >>= 1;
    }
    cout << ' ';
  }
  cout << endl;
}

std::vector<std::string> generate_trash() {
  std::vector<std::string> result;
  result.reserve(2000);

  const std::array<std::string_view, 4> direct_names = {"north", "east", "west",
                                                        "south"};
  const std::array<std::string_view, 3> direct_values = {"none", "side", "up"};
  const std::array<std::string_view, 16> power_values = {
    "0", "1", "2",  "3",  "4",  "5",  "6",  "7",
    "8", "9", "10", "11", "12", "13", "14", "15"};

  const std::string base_id = "minecraft:redstone_wire[";

  std::string id;
  id.reserve(1024);
  for (int power = 0; power < 16; power++) {
    for (int north = 0; north < 3; north++) {
      for (int east = 0; east < 3; east++) {
        for (int west = 0; west < 3; west++) {
          for (int south = 0; south < 3; south++) {
            id = base_id;
            const std::array<int, 4> dir_temp = {north, east, west, south};
            for (int dir = 0; dir < 4; dir++) {
              id += direct_names[dir];
              id += '=';
              id += direct_values[dir_temp[dir]];
              id += ',';
            }

            id += "power=";
            id += power_values[power];
            id += "]";
            result.emplace_back(id);
          }
        }
      }
    }
  }

  return result;
}