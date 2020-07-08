/*
   Copyright 2020 Alexander Chernenko (achernenko@mail.ru)
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

#include <cstdint>
#include <iostream>
#include <stdexcept>

#include "modbus_data.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) try {
  std::cout << "Comming soon...\n";

/////// DRAFT ///////
//  ReadHoldingRegistersFn readHoldingRegisters = [&serial, timeout_ms](
// 		uint8_t addr, uint16_t reg, uint16_t quantity, uint16_t data[]) {
// 		if (!serial.isOpen()) {
// 			return StatusType::IOError;
// 		}
// 		const auto request_adu = modbus_adu_request{ addr,
// 			modbus_pdu_request{}.read_holding_registers(reg, quantity) }.to_ascii();
// 		serial.clear();
// 		serial.write(request_adu.data(), request_adu.size());
// 		serial.flush();
// 		if (!serial.waitForBytesWritten(timeout_ms)) {
// 			return StatusType::IOError;
// 		}
// 		size_t read_timeout{};
// 		const size_t read_timeout_delta{ 1 };
// 		StatusType status{ StatusType::IOError };
// 		QByteArray response_raw{};
// 		while (read_timeout < timeout_ms) {
// 			read_timeout += read_timeout_delta;
// 			if (serial.waitForReadyRead(read_timeout_delta)) {
// 				response_raw.append(serial.readAll());
// 				if (response_raw.contains(':') &&
// 					response_raw.contains("\r\n")) {
// 					try {
// 						auto response_adu = modbus_adu_response{
// 							response_raw.toStdString() };
// 						if (response_adu.get_dev_addr() != addr) {
// 							break;
// 						}
// 						auto response_pdu = response_adu.get_pdu();
// 						if (response_pdu.get_function() !=
// 							modbus_function::read_holding_registers) {
// 							break;
// 						}
// 						if (response_pdu.get_quantity() != quantity) {
// 							break;
// 						}
// 						std::memcpy(data, response_pdu.get_data().data(), response_pdu.size());
// 						status = StatusType::NoError;
// 						break;
// 					}
// 					catch (...) {
// 						break;
// 					}
// 				}
// 			}
// 		}
// 		// TODO: add error check
// 		return status;
// 	};
// 	setIO(readHoldingRegisters);
// 	WriteSingleRegisterFn writeSingleRegister = [&serial, timeout_ms](
// 		uint8_t addr, uint16_t reg, uint16_t data) {
// 		if (!serial.isOpen()) {
// 			return StatusType::IOError;
// 		}
// 		const auto request_adu = modbus_adu_request{ addr,
// 			modbus_pdu_request{}.write_single_register(reg,
// 				modbus_data{ modbus_value<uint16_t>{ data } }) }.to_ascii();
// 		serial.clear();
// 		serial.write(request_adu.data(), request_adu.size());
// 		serial.flush();
// 		if (!serial.waitForBytesWritten(timeout_ms)) {
// 			return StatusType::IOError;
// 		}
// 		size_t read_timeout{};
// 		const size_t read_timeout_delta{ 1 };
// 		StatusType status{ StatusType::IOError };
// 		QByteArray response_raw{};
// 		while (read_timeout < timeout_ms) {
// 			read_timeout += read_timeout_delta;
// 			if (serial.waitForReadyRead(read_timeout_delta)) {
// 				response_raw.append(serial.readAll());
// 				if (response_raw.contains(':') &&
// 					response_raw.contains("\r\n")) {
// 					// TODO: add parse answer
// 					status = StatusType::NoError;
// 					break;
// 				}
// 			}
// 		}
// 		// TODO: add error check
// 		return status;
// 	};
// 	setIO(writeSingleRegister);
// 	WriteMultipleRegistersFn writeMultipleRegisters = [&serial, timeout_ms](
// 		uint8_t addr, uint16_t reg, uint16_t quantity, const uint16_t data[]) {
// 		if (!serial.isOpen()) {
// 			return StatusType::IOError;
// 		}
// 		modbus_data mb_data{};
// 		for (int idx{}; idx < quantity; ++idx) {
// 			mb_data += modbus_value<uint16_t>{ data[idx] };
// 		}
// 		const auto request_adu = modbus_adu_request{ addr,
// 			modbus_pdu_request{}.write_multipule_register(reg, quantity, mb_data) }.to_ascii();
// 		serial.clear();
// 		serial.write(request_adu.data(), request_adu.size());
// 		serial.flush();
// 		if (!serial.waitForBytesWritten(timeout_ms)) {
// 			return StatusType::IOError;
// 		}
// 		size_t read_timeout{};
// 		const size_t read_timeout_delta{ 1 };
// 		StatusType status{ StatusType::IOError };
// 		QByteArray response_raw{};
// 		while (read_timeout < timeout_ms) {
// 			read_timeout += read_timeout_delta;
// 			if (serial.waitForReadyRead(read_timeout_delta)) {
// 				response_raw.append(serial.readAll());
// 				if (response_raw.contains(':') &&
// 					response_raw.contains("\r\n")) {
// 					// TODO: add parse answer
// 					status = StatusType::NoError;
// 					break;
// 				}
// 			}
// 		}
// 		// TODO: add error check
// 		return status;
// 	};
// 	setIO(writeMultipleRegisters);

  return 0;
} catch (...) {
  std::cerr << "\nunexpected exception\n";
  return 0;
}
