// Copyright 2015-2017 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "phy.h"

/**
 * @brief Dump LAN8742 PHY SMI configuration registers
 *
 */
void phy_lan8742_dump_registers();

/**
 * @brief Default LAN8742 phy_check_init function
 *
 */
void phy_lan8742_check_phy_init(void);

/**
 * @brief Default LAN8742 phy_get_speed_mode function
 *
 * @return eth_speed_mode_t Ethernet speed mode
 */
eth_speed_mode_t phy_lan8742_get_speed_mode(void);

/**
 * @brief Default LAN8742 phy_get_duplex_mode function
 *
 * @return eth_duplex_mode_t Ethernet duplex mode
 */
eth_duplex_mode_t phy_lan8742_get_duplex_mode(void);

/**
 * @brief Default LAN8742 phy_power_enable function
 *
 */
void phy_lan8742_power_enable(bool);

/**
 * @brief Default LAN8742 phy_init function
 *
 * @return esp_err_t
 *      - ESP_OK on success
 *      - ESP_FAIL on error
 */
esp_err_t phy_lan8742_init(void);

/** @brief For debugging, return the values of all registers.
 */
void phy_lan8742_read_registers(uint16_t values[32]);

/** @brief For debugging, return the value of a chosen MMD register.
 */
uint16_t phy_lan8742_read_mmd_register(uint16_t devad, uint16_t index);

/** @brief LAN8742 Wake-On-LAN magic packet enable function.
 */
void phy_lan8742_wol_magic_begin(void);

/** @brief LAN8742 Wake-On-LAN magic packet disable function.
 */
void phy_lan8742_wol_magic_end(void);

/**
 * @brief Default LAN8742 PHY configuration
 *
 * @note This configuration is not suitable for use as-is,
 * it will need to be modified for your particular PHY hardware setup.
 *
 */
extern const eth_config_t phy_lan8742_default_ethernet_config;

#ifdef __cplusplus
}
#endif
