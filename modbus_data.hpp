/*

MODBUS_DATA
Copyright (C) 2020 Alexander Chernenko (achernenko@mail.ru)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

template <typename T>
class modbus_value final {
 public:
  using value_type = T;
  using register_type = uint16_t;
  using register_reference = register_type &;
  using register_const_reference = const register_reference &;
  using register_pointer = register_type *;
  using register_const_pointer = const register_type *;
  modbus_value() = default;
  modbus_value(T value) { data.value = value; }
  static constexpr uint16_t quantity() noexcept {
    return sizeof(value_type) < sizeof(uint16_t)
               ? sizeof(uint16_t) / sizeof(register_type)
               : uint16_t(sizeof(value_type) / sizeof(register_type));
  }
  value_type get_value() const noexcept { return data.value; }
  modbus_value &operator=(value_type rhs) noexcept {
    data.value = rhs;
    return *this;
  }
  register_reference operator[](int idx) noexcept { return data.regs[idx]; }
  register_const_pointer cbegin() const noexcept { return data.regs; }
  register_pointer begin() noexcept { return data.regs; }
  register_const_pointer cend() const noexcept {
    return data.regs + quantity();
  }
  register_pointer end() noexcept { return data.regs + quantity(); }
  static constexpr size_t bytes() noexcept { return sizeof(value_type); }

 private:
  union data_type {
    value_type value;
    register_type regs[quantity()];
  };
  data_type data;
};

class modbus_data : public std::vector<uint16_t> {
 public:
  modbus_data() = default;
  template <typename T>
  modbus_data(modbus_value<T> &mb_value) {
    for (const auto &value : mb_value) {
      push_back(value);
    }
  }
  template <typename T>
  modbus_data &operator+=(modbus_value<T> &rhs) {
    for (const auto &value : rhs) {
      push_back(value);
    }
    return *this;
  }
};

using modbus_function_type = uint8_t;
enum class modbus_function : modbus_function_type {
  read_holding_registers = 0x03,
  write_single_register = 0x06,
  write_multipule_register = 0x10
};

class modbus_pdu_request : public std::vector<uint8_t> {
 public:
  modbus_pdu_request() = default;
  modbus_pdu_request &read_holding_registers(uint16_t reg, uint16_t quantity) {
    push_back(value_type(modbus_function::read_holding_registers));
    push_back((reg >> 8) & 0xFF);
    push_back(reg & 0xFF);
    push_back((quantity >> 8) & 0xFF);
    push_back(quantity & 0xFF);
    return *this;
  }
  modbus_pdu_request &write_single_register(uint16_t reg,
                                            modbus_data &mb_data) {
    push_back(value_type(modbus_function::write_single_register));
    push_back((reg >> 8) & 0xFF);
    push_back(reg & 0xFF);
    for (const auto &value : mb_data) {
      push_back((value >> 8) & 0xFF);
      push_back(value & 0xFF);
    }
    return *this;
  }
  modbus_pdu_request &write_multipule_register(uint16_t reg, uint16_t quantity,
                                               modbus_data &mb_data) {
    push_back(value_type(modbus_function::write_multipule_register));
    push_back((reg >> 8) & 0xFF);
    push_back(reg & 0xFF);
    push_back((quantity >> 8) & 0xFF);
    push_back(quantity & 0xFF);
    push_back(value_type(sizeof(uint16_t) * quantity));
    for (const auto &value : mb_data) {
      push_back((value >> 8) & 0xFF);
      push_back(value & 0xFF);
    }
    return *this;
  }
};

class modbus_pdu_response : public std::vector<uint8_t> {
 public:
  modbus_pdu_response() = default;
  auto get_function() const noexcept { return modbus_function(operator[](0)); }
  auto get_quantity() const noexcept {
    return uint16_t(operator[](1) / sizeof(uint16_t));
  }
  auto get_data() const noexcept {
    modbus_data data{};
    data.resize(get_quantity());
    size_t idx{2};
    for (auto &value : data) {
      value = uint16_t(operator[](idx)) << 8;
      ++idx;
      value |= uint16_t(operator[](idx));
      ++idx;
    }
    return data;
  }
  auto get_size() const noexcept { return uint16_t(operator[](1)); }
};

class modbus_adu {
 public:
  using ascii_type = std::string;
  using rtu_type = std::vector<uint8_t>;

 protected:
  std::vector<uint8_t> data{};
  static uint8_t ascii_lrc(const std::vector<uint8_t> &data) noexcept {
    uint8_t lrc{};
    for (const auto &value : data) {
      lrc += value;
    }
    return (~lrc + 1);
  }
};

class modbus_adu_request : public modbus_adu {
  ascii_type to_ascii_elm(uint8_t value) const noexcept {
    ascii_type elm("\0\0", 2);
    std::snprintf((char *)elm.data(), elm.size() + 1, "%X%X",
                  (value >> 4) & 0x0F, value & 0x0F);
    return elm;
  }

 public:
  using ascii_type = std::string;
  modbus_adu_request(uint8_t dev_addr, const modbus_pdu_request &pdu) {
    data.push_back(dev_addr);
    data.insert(data.begin() + 1, pdu.begin(), pdu.end());
  }
  auto to_ascii() const noexcept {
    ascii_type adu_ascii{':'};
    for (const auto &value : data) {
      adu_ascii += to_ascii_elm(value);
    }
    const auto lrc = ascii_lrc(data);
    adu_ascii += to_ascii_elm(lrc);
    adu_ascii += '\r';
    adu_ascii += '\n';
    return adu_ascii;
  }
};

class modbus_adu_response : public modbus_adu {
  auto from_ascii_elm(const char elm[]) const noexcept {
    int value{};
    std::sscanf(elm, "%2X", &value);
    return uint8_t(value);
  }

 public:
  modbus_adu_response(const ascii_type &response) {
    if (response.empty() || response[0] != ':') {
      throw std::runtime_error("invalid modbus adu response");
    } else if (response[response.size() - 2] != '\r' &&
               response[response.size() - 1] != '\n') {
      // TODO: check and parse error
      throw std::runtime_error("modbus error response");
    }
    const uint8_t lrc_response = from_ascii_elm(&response[response.size() - 4]);
    for (int i{1}; i < response.size() - 4; i += 2) {
      data.push_back(from_ascii_elm(&response[i]));
    }
    const auto lrc_calculate = ascii_lrc(data);
    if (lrc_calculate != lrc_response) {
      throw std::runtime_error("invalid checksum in modbus adu response");
    }
  }
  auto get_dev_addr() const noexcept { return data[1]; }
  auto get_pdu() {
    modbus_pdu_response pdu{};
    pdu.insert(pdu.begin(), data.begin() + 1, data.end());
    return pdu;
  }
};
