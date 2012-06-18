/*
 * Copyright (C) 2003-2012 FreeIPMI Core Team
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef IPMI_OEM_SPEC_H
#define IPMI_OEM_SPEC_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************* 
 * Dell                                    *
 *******************************************/
  
/*
 * Dell Poweredge R610
 * Dell Poweredge R710
 */

/* IPMI_NET_FN_OEM_GROUP_RQ / IPMI_NET_FN_OEM_GROUP_RS */
#define IPMI_OEM_DELL_RESERVED_EXTENDED_CONFIGURATION 0x01
#define IPMI_OEM_DELL_GET_EXTENDED_CONFIGURATION      0x02
#define IPMI_OEM_DELL_SET_EXTENDED_CONFIGURATION      0x03

/* IPMI_NET_FN_OEM_DELL_GENERIC_RQ / IPMI_NET_FN_OEM_DELL_GENERIC_RS */
#define IPMI_OEM_DELL_RESET_TO_DEFAULTS                                    0x21
#define IPMI_OEM_DELL_SET_NIC_SELECTION                                    0x24
#define IPMI_OEM_DELL_GET_NIC_SELECTION                                    0x25
#define IPMI_OEM_DELL_QUERY_CHASSIS_IDENTIFY_STATUS                        0x32
#define IPMI_OEM_DELL_GET_CPLD_VERSION                                     0x33
#define IPMI_OEM_DELL_GET_POWER_CONSUMPTION_DATA                           0x9C
#define IPMI_OEM_DELL_RESET_POWER_CONSUMPTION_DATA                         0x9D
#define IPMI_OEM_DELL_POWER_SUPPLY_INFO                                    0xB0
#define IPMI_OEM_DELL_POWER_MONITORING_OVER_A_SPECIFIED_AVERAGING_INTERVAL 0xB1
#define IPMI_OEM_DELL_POWER_MONITORING_AVERAGING_INTERVAL_RANGE            0xB2
#define IPMI_OEM_DELL_POWER_CONSUMPTION                                    0xB3
#define IPMI_OEM_DELL_LCD_INFO                                             0xB5
/* achu: this one is taken from code, is correct name? */
#define IPMI_OEM_DELL_POWER_CAPACITY_STATUS                                0xBA
#define IPMI_OEM_DELL_GET_POWER_HEAD_ROOM                                  0xBB
#define IPMI_OEM_DELL_ROLLBACK_FIRMWARE_VERSION                            0xBE
#define IPMI_OEM_DELL_GET_IDRAC_FIRMWARE_VERSION_NUMBER                    0xBF
#define IPMI_OEM_DELL_GET_ACTIVE_LOM_STATUS                                0xC1
#define IPMI_OEM_DELL_IDRAC_VIRTUAL_MAC                                    0xC9

/*
 * Dell Poweredge C410x
 */

/* IPMI_NET_FN_OEM_DELL_GENERIC_RQ / IPMI_NET_FN_OEM_DELL_GENERIC_RS */
#define IPMI_OEM_DELL_SLOT_POWER_CONTROL 0xF0

/* IPMI_NET_FN_OEM_DELL_GENERIC_PORT_MAP_RQ / IPMI_NET_FN_OEM_DELL_GENERIC_PORT_MAP_RS */
#define IPMI_OEM_DELL_PORT_MAP           0xC8

/******************************************* 
 * Fujitsu                                 *
 *******************************************/

/*
 * Fujitsu iRMC S1 / iRMC S2
 *
 * http://manuals.ts.fujitsu.com/file/4390/irmc_s2-en.pdf
 */
/* IPMI_NET_FN_OEM_GROUP_RQ / IPMI_NET_FN_OEM_GROUP_RS */
#define IPMI_OEM_FUJITSU_POWER         0x01
#define IPMI_OEM_FUJITSU_COMMUNICATION 0x02
#define IPMI_OEM_FUJITSU_FAN_TEST      0x10
#define IPMI_OEM_FUJITSU_BIOS          0xF1
#define IPMI_OEM_FUJITSU_SYSTEM        0xF5

/* IPMI_NET_FN_FIRMWARE_RQ / IPMI_NET_FN_FIRMWARE_RS */
#define IPMI_OEM_FUJITSU_SET_FIRMWARE_SELECTOR 0x04
#define IPMI_OEM_FUJITSU_GET_FIRMWARE_SELECTOR 0x05

/* IPMI_NET_FN_OEM_FUJITSU_GENERIC_RQ / IPMI_NET_FN_OEM_FUJITSU_GENERIC_RS */
#define IPMI_OEM_FUJITSU_GET_REMOTE_STORAGE_CONNECTION_OR_STATUS 0x19
#define IPMI_OEM_FUJITSU_SET_VIDEO_DISPLAY_ON_OFF                0x1A

/*******************************************
 * IBM                                     *
 *******************************************/

/*
 * IBM x3455/x3755
 */

/* w/ IPMI_CMD_OEM_IBM_GET_LED */

#define IPMI_OEM_IBM_LED_STATE_INACTIVE                  0x0

#define IPMI_OEM_IBM_LED_ACTIVE_BY_LED                   0x1
#define IPMI_OEM_IBM_LED_ACTIVE_BY_SENSOR                0x2
#define IPMI_OEM_IBM_LED_ACTIVE_BY_USER                  0x3
#define IPMI_OEM_IBM_LED_ACTIVE_BY_BIOS_OR_ADMINISTRATOR 0x4

#define IPMI_OEM_IBM_LED_X3455_LOCATION          0x1

#define IPMI_OEM_IBM_LED_X3755_CPU               0x0010
#define IPMI_OEM_IBM_LED_X3755_CPU1              0x0030
#define IPMI_OEM_IBM_LED_X3755_CPU2              0x0031
#define IPMI_OEM_IBM_LED_X3755_CPU3              0x0032
#define IPMI_OEM_IBM_LED_X3755_CPU4              0x0033

#define IPMI_OEM_IBM_LED_X3755_CPU1_BOARD        0x00B8
#define IPMI_OEM_IBM_LED_X3755_CPU2_BOARD        0x00B9
#define IPMI_OEM_IBM_LED_X3755_CPU3_BOARD        0x00BA
#define IPMI_OEM_IBM_LED_X3755_CPU4_BOARD        0x00BB

#define IPMI_OEM_IBM_LED_X3755_DIMM_1            0x0060
#define IPMI_OEM_IBM_LED_X3755_DIMM_2            0x0061
#define IPMI_OEM_IBM_LED_X3755_DIMM_3            0x0062 
#define IPMI_OEM_IBM_LED_X3755_DIMM_4            0x0063
#define IPMI_OEM_IBM_LED_X3755_DIMM_5            0x0064
#define IPMI_OEM_IBM_LED_X3755_DIMM_6            0x0065
#define IPMI_OEM_IBM_LED_X3755_DIMM_7            0x0066
#define IPMI_OEM_IBM_LED_X3755_DIMM_8            0x0067
#define IPMI_OEM_IBM_LED_X3755_DIMM_9            0x0068
#define IPMI_OEM_IBM_LED_X3755_DIMM_10           0x0069
#define IPMI_OEM_IBM_LED_X3755_DIMM_11           0x006A
#define IPMI_OEM_IBM_LED_X3755_DIMM_12           0x006B
#define IPMI_OEM_IBM_LED_X3755_DIMM_13           0x006C
#define IPMI_OEM_IBM_LED_X3755_DIMM_14           0x006D
#define IPMI_OEM_IBM_LED_X3755_DIMM_15           0x006E
#define IPMI_OEM_IBM_LED_X3755_DIMM_16           0x006F
#define IPMI_OEM_IBM_LED_X3755_DIMM_17           0x00C0
#define IPMI_OEM_IBM_LED_X3755_DIMM_18           0x00C1
#define IPMI_OEM_IBM_LED_X3755_DIMM_19           0x00C2
#define IPMI_OEM_IBM_LED_X3755_DIMM_20           0x00C3
#define IPMI_OEM_IBM_LED_X3755_DIMM_21           0x00C4
#define IPMI_OEM_IBM_LED_X3755_DIMM_22           0x00C5
#define IPMI_OEM_IBM_LED_X3755_DIMM_23           0x00C6
#define IPMI_OEM_IBM_LED_X3755_DIMM_24           0x00C7
#define IPMI_OEM_IBM_LED_X3755_DIMM_25           0x00C8
#define IPMI_OEM_IBM_LED_X3755_DIMM_26           0x00C9
#define IPMI_OEM_IBM_LED_X3755_DIMM_27           0x00CA
#define IPMI_OEM_IBM_LED_X3755_DIMM_28           0x00CB
#define IPMI_OEM_IBM_LED_X3755_DIMM_29           0x00CC
#define IPMI_OEM_IBM_LED_X3755_DIMM_30           0x00CD
#define IPMI_OEM_IBM_LED_X3755_DIMM_31           0x00CE
#define IPMI_OEM_IBM_LED_X3755_DIMM_32           0x00CF

#define IPMI_OEM_IBM_LED_X3755_FAN               0x0014
#define IPMI_OEM_IBM_LED_X3755_FAN_1             0x0050
#define IPMI_OEM_IBM_LED_X3755_FAN_2             0x0051
#define IPMI_OEM_IBM_LED_X3755_FAN_3             0x0052
#define IPMI_OEM_IBM_LED_X3755_FAN_4             0x0053
#define IPMI_OEM_IBM_LED_X3755_FAN_5             0x0054
#define IPMI_OEM_IBM_LED_X3755_FAN_6             0x0055
#define IPMI_OEM_IBM_LED_X3755_FAN_7             0x0056
#define IPMI_OEM_IBM_LED_X3755_FAN_8             0x0057

#define IPMI_OEM_IBM_LED_X3755_PCI               0x0020
#define IPMI_OEM_IBM_LED_X3755_PCI_1             0x0070
#define IPMI_OEM_IBM_LED_X3755_PCI_2             0x0071
#define IPMI_OEM_IBM_LED_X3755_PCI_3             0x0072
#define IPMI_OEM_IBM_LED_X3755_PCI_4             0x0073
#define IPMI_OEM_IBM_LED_X3755_PCI_5             0x0074
#define IPMI_OEM_IBM_LED_X3755_PCI_6             0x0075

#define IPMI_OEM_IBM_LED_X3755_SERVERAID_8K_BATT 0x00D0
#define IPMI_OEM_IBM_LED_X3755_SERVERAID_8K_ERR  0x00D1

#define IPMI_OEM_IBM_LED_X3755_ALERT             0x00D9
#define IPMI_OEM_IBM_LED_X3755_BK_BLUE           0x00D8
#define IPMI_OEM_IBM_LED_X3755_BOARD             0x000E
#define IPMI_OEM_IBM_LED_X3755_CNFG              0x0006
#define IPMI_OEM_IBM_LED_X3755_DASD              0x0013
#define IPMI_OEM_IBM_LED_X3755_FAULT             0x0000
#define IPMI_OEM_IBM_LED_X3755_HTX               0x00B0
#define IPMI_OEM_IBM_LED_X3755_INFO              0x0003
#define IPMI_OEM_IBM_LED_X3755_LOCATION          0x0001
#define IPMI_OEM_IBM_LED_X3755_MEM               0x0015
#define IPMI_OEM_IBM_LED_X3755_NMI               0x0019
#define IPMI_OEM_IBM_LED_X3755_OVERSPEC          0x001B
#define IPMI_OEM_IBM_LED_X3755_RAID              0x000F
#define IPMI_OEM_IBM_LED_X3755_SEER              0x000B
#define IPMI_OEM_IBM_LED_X3755_SP                0x001E
#define IPMI_OEM_IBM_LED_X3755_TEMP              0x001C
#define IPMI_OEM_IBM_LED_X3755_VRM               0x0011
 
#define IPMI_OEM_IBM_LED_X3755_UNKNOWN1          0x0040
#define IPMI_OEM_IBM_LED_X3755_UNKNOWN2          0x0041
#define IPMI_OEM_IBM_LED_X3755_UNKNOWN3          0x0047

/*******************************************
 * Intel                                   *
 *******************************************/

/*
 * Intel S5500WB/Penguin Computing Relion 700
 * Intel S2600JF/Appro 512X
 */

/* IPMI_NET_FN_OEM_INTEL_GENERIC_RQ / IPMI_NET_FN_OEM_INTEL_GENERIC_RS */
#define IPMI_OEM_INTEL_RESTORE_CONFIGURATION 0x02
#define IPMI_OEM_INTEL_SET_FAULT_INDICATION  0x57

/* IPMI_NET_FN_OEM_INTEL_CONFIG_RQ / IPMI_NET_FN_OEM_INTEL_CONFIG_RS */
#define IPMI_OEM_INTEL_SET_SMTP_CONFIGURATION 0x37
#define IPMI_OEM_INTEL_GET_SMTP_CONFIGURATION 0x38

/*
 * Intel S2600JF/Appro 512X
 */

/* IPMI_NET_FN_OEM_INTEL_GENERIC_RQ / IPMI_NET_FN_OEM_INTEL_GENERIC_RS */
#define IPMI_OEM_INTEL_SET_POWER_RESTORE_DELAY 0x54
#define IPMI_OEM_INTEL_GET_POWER_RESTORE_DELAY 0x55
#define IPMI_OEM_INTEL_GET_BMC_SERVICE_STATUS  0xB2
#define IPMI_OEM_INTEL_CONTROL_BMC_SERVICES    0xB1

/*
 * Intel Node Manager
 *
 * For Intel Chips, not just Intel Motherboards.  Confirmed for:
 *
 * Intel S5500WB/Penguin Computing Relion 700
 * Intel S2600JF/Appro 512X
 * Inventec 5441/Dell Xanadu II
 * Inventec 5442/Dell Xanadu III
 * Quanta S99Q/Dell FS12-TY
 * Quanta QSSC-S4R/Appro GB812X-CN
 */

/* IPMI_NET_FN_OEM_GROUP_RQ / IPMI_NET_FN_OEM_GROUP_RS */
#define IPMI_OEM_INTEL_NODE_MANAGER_ENABLE_DISABLE_NODE_MANAGER_POLICY_CONTROL 0xC0
#define IPMI_OEM_INTEL_NODE_MANAGER_SET_NODE_MANAGER_POLICY                    0xC1
#define IPMI_OEM_INTEL_NODE_MANAGER_GET_NODE_MANAGER_POLICY                    0xC2
#define IPMI_OEM_INTEL_NODE_MANAGER_SET_NODE_MANAGER_ALERT_THRESHOLDS          0xC3
#define IPMI_OEM_INTEL_NODE_MANAGER_GET_NODE_MANAGER_ALERT_THRESHOLDS          0xC4
#define IPMI_OEM_INTEL_NODE_MANAGER_SET_NODE_MANAGER_POLICY_SUSPEND_PERIODS    0xC5
#define IPMI_OEM_INTEL_NODE_MANAGER_GET_NODE_MANAGER_POLICY_SUSPEND_PERIODS    0xC6
#define IPMI_OEM_INTEL_NODE_MANAGER_RESET_NODE_MANAGER_STATISTICS              0xC7
#define IPMI_OEM_INTEL_NODE_MANAGER_GET_NODE_MANAGER_STATISTICS                0xC8
#define IPMI_OEM_INTEL_NODE_MANAGER_GET_NODE_MANAGER_CAPABILITIES              0xC9
#define IPMI_OEM_INTEL_NODE_MANAGER_GET_NODE_MANAGER_VERSION                   0xCA
#define IPMI_OEM_INTEL_NODE_MANAGER_SET_NODE_MANAGER_POWER_DRAW_RANGE          0xCB
#define IPMI_OEM_INTEL_NODE_MANAGER_SET_NODE_MANAGER_ALERT_DESTINATION         0xCC
#define IPMI_OEM_INTEL_NODE_MANAGER_GET_NODE_MANAGER_ALERT_DESTINATION         0xCD

/******************************************* 
 * Inventec                                *
 *******************************************/

/*
 * Inventec 5441/Dell Xanadu II
 * Inventec 5442/Dell Xanadu III
 */

/* IPMI_NET_FN_FIRMWARE_RQ / IPMI_NET_FN_FIRMWARE_RS */
#define IPMI_OEM_INVENTEC_UPDATE_FIRMARE    0x01
#define IPMI_OEM_INVENTEC_GET_UPDATE_STATUS 0x02
#define IPMI_OEM_INVENTEC_COPY_IMAGE_DATA   0x03

/* IPMI_NET_FN_OEM_GROUP_RQ / IPMI_NET_FN_OEM_GROUP_RS */
/* achu: not official names, named based on use context */
#define IPMI_OEM_INVENTEC_SET_DEDICATED_MAC_ADDRESS    0x21
#define IPMI_OEM_INVENTEC_SET_SHARED_MAC_ADDRESS       0x23

/* IPMI_NET_FN_OEM_INVENTEC_GENERIC_RQ / IPMI_NET_FN_OEM_INVENTEC_GENERIC_RS */
#define IPMI_OEM_INVENTEC_RESERVED_EXTENDED_CONFIGUATION 0x01
#define IPMI_OEM_INVENTEC_GET_EXTENDED_CONFIGURATION     0x02
#define IPMI_OEM_INVENTEC_SET_EXTENDED_CONFIGURATION     0x03
#define IPMI_OEM_INVENTEC_RESTORE_TO_DEFAULTS            0x04
#define IPMI_OEM_INVENTEC_GET_RESTORE_STATUS             0x05
#define IPMI_OEM_INVENTEC_SET_SYSTEM_GUID                0xB3

/* IPMI_NET_FN_OEM_INVENTEC_SPECIFIC_RQ / IPMI_NET_FN_OEM_INVENTEC_SPECIFIC_RS */ 
#define IPMI_OEM_INVENTEC_SET_WEB_PORT_NUM                0x02
#define IPMI_OEM_INVENTEC_GET_WEB_PORT_NUM                0x03
#define IPMI_OEM_INVENTEC_SET_BOARD_ID                    0x10
#define IPMI_OEM_INVENTEC_GET_BOARD_ID                    0x11
#define IPMI_OEM_INVENTEC_SET_ASSET_TAG                   0x12
#define IPMI_OEM_INVENTEC_SET_LAN_SOURCE                  0x13
#define IPMI_OEM_INVENTEC_GET_LAN_SOURCE                  0x14
#define IPMI_OEM_INVENTEC_SET_FCB_FW_VERSION              0x15
#define IPMI_OEM_INVENTEC_GET_FCB_FW_VERSION              0x16
#define IPMI_OEM_INVENTEC_SET_FAN_CONTROL                 0x61
#define IPMI_OEM_INVENTEC_GET_FAN_CONTROL                 0x62
#define IPMI_OEM_INVENTEC_SET_FSC_TABLE                   0x63
#define IPMI_OEM_INVENTEC_GET_FSC_TABLE                   0x64
#define IPMI_OEM_INVENTEC_GET_FCB_SKU_INFO                0x6A
#define IPMI_OEM_INVENTEC_GET_FCB_POWER_THROTTLING_STATUS 0x6B
#define IPMI_OEM_INVENTEC_OEM_GET_PIC_MODEL               0x70
#define IPMI_OEM_INVENTEC_OEM_SET_FLASH_PIN               0x71
#define IPMI_OEM_INVENTEC_OEM_GET_FLASH_PIN               0x72
#define IPMI_OEM_INVENTEC_OEM_NEW_MASTER_WRITE_READ       0x73
#define IPMI_OEM_INVENTEC_SET_POWER_THROTTLING_BEHAVIOR   0xB1
#define IPMI_OEM_INVENTEC_GET_POWER_THROTTLING_BEHAVIOR   0xB2
#define IPMI_OEM_INVENTEC_GET_PSU_MISMATCH_AND_TYPE       0xB3

/******************************************* 
 * Quanta                                  *
 *******************************************/

/*
 * Quanta S99Q/Dell FS12-TY
 */

/* achu: all named from doc except 'lan' configuration id, which I assumed names */

/* w/ IPMI_CMD_OEM_QUANTA_GET_EXTENDED_CONFIGURATION / IPMI_OEM_QUANTA_SET_EXTENDED_CONFIGURATION */

#define IPMI_OEM_QUANTA_EXTENDED_CONFIGURATION_ID_NIC                      0x02
#define IPMI_OEM_QUANTA_EXTENDED_CONFIGURATION_ID_SOL                      0x03
#define IPMI_OEM_QUANTA_EXTENDED_CONFIGURATION_ID_SECURITY                 0x04
#define IPMI_OEM_QUANTA_EXTENDED_CONFIGURATION_ID_ACCOUNT_STATUS           0x05
#define IPMI_OEM_QUANTA_EXTENDED_CONFIGURATION_ID_DNS                      0x06
#define IPMI_OEM_QUANTA_EXTENDED_CONFIGURATION_ID_WEB_SERVER_CONFIGURATION 0x0C
#define IPMI_OEM_QUANTA_EXTENDED_CONFIGURATION_ID_FIRMWARE_LOG             0x0E
#define IPMI_OEM_QUANTA_EXTENDED_CONFIGURATION_ID_FIRMWARE_INFORMATION     0x0F
#define IPMI_OEM_QUANTA_EXTENDED_CONFIGURATION_ID_FIRMWARE_UPDATE          0x10
#define IPMI_OEM_QUANTA_EXTENDED_CONFIGURATION_ID_POWER_MANAGEMENT         0x11

/* nic status - 1 byte, 0 = shared, 1 = dedicated
 */
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_NIC_MODE 0x01

/* sol idle timeout - 2 bytes, ls byte first, 0h = no timeout, default = 01h
 *
 * telnet/ssh redirect enable - 1 byte, 0 = disable, 1 = enabled 
 */
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_SOL_SOL_IDLE_TIMEOUT           0x01
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_SOL_TELNET_SSH_REDIRECT_ENABLE 0x02

/* service disabled - 1 byte, bitmask
 *                  - 0x01 = all service except IPMI are disabled
 *                           (takes precedence over other bits)
 *                  - 0x02 = KVM/Virtual Storage
 *                  - 0x04 = HTTP/HTTPS
 *                  - 0x08 = SSH/Telnet
 */
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_SECURITY_SERVICE_DISABLED            0x01

/* number of user - 1 byte, read only
 *
 * number of enabled user - 1 byte, read only
 *
 * user name - 1-17 bytes, read only
 *           - Quanta 5441/Xanadu II - reports stored as p-string, does not return p-string
 *           - Quanta 5442/Xanadu III - returns as p-string
 *
 * account status - 1 byte
 *                - 0x00 - status unspecified
 *                - 0x01 - enabled via set user password
 *                - 0x02 - disabled via set user password
 *                - 0x03 - user id is lockout
 */
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_ACCOUNT_STATUS_NUMBER_OF_USER         0x01
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_ACCOUNT_STATUS_NUMBER_OF_ENABLED_USER 0x02
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_ACCOUNT_STATUS_USER_NAME              0x03
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_ACCOUNT_STATUS_ACCOUNT_STATUS         0x04

/* DNS DHCP enable - 1 byte, 0 - false, 1 - true
 * - DNS server IP addresses should be assigned from DHCP server
 *
 * DNS Server1 - 4 bytes
 * - IP address for DNS server 1, read only if DNS DHCP Enable and DHCP are enabled
 *
 * DNS Server2 - 4 bytes
 * - IP address for DNS server 2, read only if DNS DHCP Enable and DHCP are enabled
 *
 * DNS Register BMC - 0 - false, 1 - true
 * - Enable registering the BMC host name on the DNS server
 *
 * DNS BMC Host Name - 1-64 bytes
 * - Specifies the DNS BMC host name, read only if DNS Register BMC is TRUE.
 * - Stored as P-string
 *
 * DNS Domain Name DHCP Enable - 1 byte
 * - DNS domain name should be assigned from DHCP
 *
 * DNS Domain Name - 1-256 bytes
 * - DNS domain name string, read only if DNS Domain Name DHCP Enable is TRUE
 * - Stored as P-string
 */
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_DNS_DNS_DHCP_ENABLE             0x01
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_DNS_DNS_SERVER1                 0x02
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_DNS_DNS_SERVER2                 0x03
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_DNS_DNS_REGISTER_BMC            0x04
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_DNS_DNS_BMC_HOST_NAME           0x05
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_DNS_DNS_DOMAIN_NAME_DHCP_ENABLE 0x06
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_DNS_DNS_DOMAIN_NAME             0x07

/* web server enabled - 1 byte, 0 = false, 1 = true (default)
 *
 * max web sessions - 1 byte, read only
 *
 * active web sessions - 1 byte, read only
 *
 * web server timeout - 4 bytes, in seconds, 0 = disable, range 60-1920, default 300
 *
 * http port num - 2 bytes, default 80
 *
 * https port num - 2 bytes, default 443
 */
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_WEB_SERVER_CONFIGURATION_WEB_SERVER_ENABLED  0x01
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_WEB_SERVER_CONFIGURATION_MAX_WEB_SESSIONS    0x02
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_WEB_SERVER_CONFIGURATION_ACTIVE_WEB_SESSIONS 0x03
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_WEB_SERVER_CONFIGURATION_WEB_SERVER_TIMEOUT  0x04
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_WEB_SERVER_CONFIGURATION_HTTP_PORT_NUM       0x05
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_WEB_SERVER_CONFIGURATION_HTTPS_PORT_NUM      0x06

/* entity - 1 byte (read only)
 *
 * firmware version - 1-16 bytes (read only)
 *
 * branch - 1-16 bytes (read only)
 *
 * build information - 1-16 bytes (read only)
 *
 * update date / time - 3 bytes, from 0:00 1/1/08, lsbyte first
 */
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_LOG_ENTITY            0x01
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_LOG_FIRMWARE_VERSION  0x02
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_LOG_BRANCH            0x03
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_LOG_BUILD_INFORMATION 0x04
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_LOG_UPDATE_DATE_TIME  0x05

/* name - 1-16 bytes (read only)
 *
 * description - 1-256 bytes (read only)
 *
 * entity - 1 byte (read only)
 *
 * product info - 1-64 bytes (read only)
 *
 * firmware version - 1-16 bytes (read only)
 *
 * branch - 1-16 bytes (read only)
 *
 * build information - 1-16 bytes (read only) 
 *
 */
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_NAME              0x01
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_DESCRIPTION       0x02
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_ENTITY            0x03
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_PRODUCT_INFO      0x04
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_FIRMWARE_VERSION  0x05
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_BRANCH            0x06
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_BUILD_INFORMATION 0x07

#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_NAME_LEN              16
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_DESCRIPTION_LEN       256
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_PRODUCT_INFO_LEN      64
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_FIRMWARE_VERSION_LEN  16
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_BRANCH_LEN            16
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_BUILD_INFORMATION_LEN 16

/* remote update enable - 1 byte, ??
 *
 * protocol - 1 byte, bitmask, 7:3 reserved, 2 : http, 1: ftp, 0: tftp (read only)
 *
 * uri - 1-256 bytes
 * note - first char should be a file or unit separator
 *
 * connection retry - 1 byte, 0 = no retries
 *
 * retry interval - 1 byte, in 5 second increments
 *
 * delay time - 1 byte, in seconds, 0h = immediate, ffh = random between 5 and 10
 */
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_UPDATE_REMOTE_UPDATE_ENABLE 0x01
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_UPDATE_PROTOCOL             0x02
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_UPDATE_URI                  0x03
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_UPDATE_CONNECTION_RETRY     0x04
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_UPDATE_RETRY_INTERVAL       0x05
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_UPDATE_DELAY_TIME           0x06

#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_UPDATE_RETRY_INTERVAL_INCREMENTS 5

#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_FIRMWARE_UPDATE_URI_LEN 256

/* power management enable - 1 byte, bit 7: 0 = disable, 1 = enable, reset reserved
 *
 * power staggery ac recovery - 1 byte, 0x00 = immediate power on
 * (default), 0x01 = auto, random between min and max below, 0x02 =
 * user defined, must be between min and max below
 *
 * power on delay - 2 bytes
 *
 * minimum power on delay - 2 bytes (read only)
 *
 * maximum power on delay - 2 bytes
 */
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_POWER_MANAGEMENT_POWER_MANAGEMENT_ENABLE      0x01
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_POWER_MANAGEMENT_POWER_STAGGERING_AC_RECOVERY 0x02
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_POWER_MANAGEMENT_POWER_ON_DELAY               0x03
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_POWER_MANAGEMENT_MINIMUM_POWER_ON_DELAY       0x04
#define IPMI_OEM_QUANTA_EXTENDED_ATTRIBUTE_ID_POWER_MANAGEMENT_MAXIMUM_POWER_ON_DELAY       0x05

#define IPMI_OEM_QUANTA_EXTENDED_CONFIG_READ_ALL_BYTES           0xFF

#define IPMI_OEM_QUANTA_EXTENDED_CONFIG_NIC_MODE_SHARED    0x00
#define IPMI_OEM_QUANTA_EXTENDED_CONFIG_NIC_MODE_DEDICATED 0x01

#define IPMI_OEM_QUANTA_EXTENDED_CONFIG_SECURITY_SERVICES_DISABLED_ENABLE_ALL   0x00
#define IPMI_OEM_QUANTA_EXTENDED_CONFIG_SECURITY_SERVICES_DISABLED_BITMASK_ALL  0x01
#define IPMI_OEM_QUANTA_EXTENDED_CONFIG_SECURITY_SERVICES_DISABLED_BITMASK_KVM  0x02
#define IPMI_OEM_QUANTA_EXTENDED_CONFIG_SECURITY_SERVICES_DISABLED_BITMASK_HTTP 0x04
#define IPMI_OEM_QUANTA_EXTENDED_CONFIG_SECURITY_SERVICES_DISABLED_BITMASK_SSH  0x08

#define IPMI_OEM_QUANTA_EXTENDED_CONFIG_ACCOUNT_STATUS_UNSPECIFIED 0x00
#define IPMI_OEM_QUANTA_EXTENDED_CONFIG_ACCOUNT_STATUS_ENABLED     0x01
#define IPMI_OEM_QUANTA_EXTENDED_CONFIG_ACCOUNT_STATUS_DISABLED    0x02
#define IPMI_OEM_QUANTA_EXTENDED_CONFIG_ACCOUNT_STATUS_LOCKOUT     0x03

/* DPNM = Dynamic Power Node Management */
#define IPMI_OEM_QUANTA_EXTENDED_CONFIG_POWER_MANAGEMENT_ENABLE_DPNM_BITMASK 0x80
#define IPMI_OEM_QUANTA_EXTENDED_CONFIG_POWER_MANAGEMENT_ENABLE_DPNM_SHIFT   7
#define IPMI_OEM_QUANTA_EXTENDED_CONFIG_POWER_MANAGEMENT_ENABLE_DPNM_ENABLE  1
#define IPMI_OEM_QUANTA_EXTENDED_CONFIG_POWER_MANAGEMENT_ENABLE_DPNM_DISABLE 0

#define IPMI_OEM_QUANTA_EXTENDED_CONFIG_POWER_STAGGERING_AC_RECOVERY_IMMEDIATE    0x00
#define IPMI_OEM_QUANTA_EXTENDED_CONFIG_POWER_STAGGERING_AC_RECOVERY_AUTO         0x01
#define IPMI_OEM_QUANTA_EXTENDED_CONFIG_POWER_STAGGERING_AC_RECOVERY_USER_DEFINED 0x02

/* w/ IPMI_CMD_OEM_QUANTA_RESET_TO_DEFAULTS */ 
#define IPMI_OEM_QUANTA_RESET_TO_DEFAULTS_RESTORE_FLAG_RESTORE_PARAMETERS_NOT_INCLUDED_BELOW 0x7
#define IPMI_OEM_QUANTA_RESET_TO_DEFAULTS_RESTORE_FLAG_REMAINING_PARAMETERS_STAY_WHAT_IT_IS  0x0
#define IPMI_OEM_QUANTA_RESET_TO_DEFAULTS_RESTORE_FLAG_SHIFT                                 5

#define IPMI_OEM_QUANTA_RESET_TO_DEFAULTS_PEF_BITMASK                  0x10
#define IPMI_OEM_QUANTA_RESET_TO_DEFAULTS_SERIAL_CONFIGURATION_BITMASK 0x08
#define IPMI_OEM_QUANTA_RESET_TO_DEFAULTS_SOL_CONFIGURATION_BITMASK    0x04
#define IPMI_OEM_QUANTA_RESET_TO_DEFAULTS_LAN_CONFIGURATION_BITMASK    0x02
#define IPMI_OEM_QUANTA_RESET_TO_DEFAULTS_USER_ACCOUNTS_BITMASK        0x01

/* w/ IPMI_CMD_OEM_QUANTA_GET_RESTORE_STATUS */
#define IPMI_OEM_QUANTA_GET_RESTORE_STATUS_RESTORE_IN_PROGRESS 0x00
#define IPMI_OEM_QUANTA_GET_RESTORE_STATUS_RESTORE_COMPLETE    0x01

/* w/ IPMI_CMD_OEM_QUANTA_GET_PROCESSOR_INFORMATION */
#define IPMI_OEM_QUANTA_PROCESSOR_TYPE_CELERON        0x00
#define IPMI_OEM_QUANTA_PROCESSOR_TYPE_PENTIUM_3      0x01
#define IPMI_OEM_QUANTA_PROCESSOR_TYPE_PENTIUM_4      0x02
#define IPMI_OEM_QUANTA_PROCESSOR_TYPE_XEON           0x03
#define IPMI_OEM_QUANTA_PROCESSOR_TYPE_PRESTONIA      0x04
#define IPMI_OEM_QUANTA_PROCESSOR_TYPE_NOCONA         0x05
#define IPMI_OEM_QUANTA_PROCESSOR_TYPE_OPTERON        0x06
#define IPMI_OEM_QUANTA_PROCESSOR_TYPE_DEMPSEY        0x07
#define IPMI_OEM_QUANTA_PROCESSOR_TYPE_CLOVERTOWN     0x08
#define IPMI_OEM_QUANTA_PROCESSOR_TYPE_TIGERTON       0x09
#define IPMI_OEM_QUANTA_PROCESSOR_TYPE_DUNNINGTON     0x0A
/* achu: listed as "Hapertown" in spec, assuming typo */
#define IPMI_OEM_QUANTA_PROCESSOR_TYPE_HARPERTOWN     0x0B
#define IPMI_OEM_QUANTA_PROCESSOR_TYPE_WOLFDALE_DP    0x0C
#define IPMI_OEM_QUANTA_PROCESSOR_TYPE_NEHALEM_EP     0x0D
#define IPMI_OEM_QUANTA_PROCESSOR_TYPE_WESTMERE_EP    0x0E
/* 0x0F-0xFE - Reserved */
#define IPMI_OEM_QUANTA_PROCESSOR_TYPE_NO_CPU_PRESENT 0xFF

/******************************************* 
 * Sun Microsystems                        *
 *******************************************/
  
/*
 * Sun 4140
 */

/* w/ IPMI_CMD_OEM_SUN_GET_LED / IPMI_CMD_OEM_SUN_SET_LED */
#define IPMI_OEM_SUN_LED_MODE_OFF     0
#define IPMI_OEM_SUN_LED_MODE_ON      1
#define IPMI_OEM_SUN_LED_MODE_STANDBY 2
#define IPMI_OEM_SUN_LED_MODE_SLOW    3
#define IPMI_OEM_SUN_LED_MODE_FAST    4

#define IPMI_OEM_SUN_LED_FORCE_GO_THRU_CONTROLLER     0
#define IPMI_OEM_SUN_LED_FORCE_DIRECTLY_ACCESS_DEVICE 1

/******************************************* 
 * Supermicro                              *
 *******************************************/
  
/*
 * Supermicro X8DTG
 */

/* w/ IPMI_CMD_OEM_SUPERMICRO_GENERIC_EXTENSION */
#define IPMI_OEM_SUPERMICRO_SUB_COMMAND_BMC_SERVICES 0xF0

/* w/ IPMI_OEM_SUPERMICRO_SUB_COMMAND_BMC_SERVICES */
#define IPMI_OEM_SUPERMICRO_BMC_SERVICES_ACTION_DISABLE  0x00
#define IPMI_OEM_SUPERMICRO_BMC_SERVICES_ACTION_ENABLE   0x01
#define IPMI_OEM_SUPERMICRO_BMC_SERVICES_ACTION_STATUS   0x02

#define IPMI_OEM_SUPERMICRO_BMC_SERVICES_STATUS_DISABLED 0x00
#define IPMI_OEM_SUPERMICRO_BMC_SERVICES_STATUS_ENABLED  0x01


#ifdef __cplusplus
}
#endif

#endif /* IPMI_OEM_SPEC_H */