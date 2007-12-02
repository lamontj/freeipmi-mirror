/* 
   ipmi-cipher-suite-utils.h - IPMI Cipher Suite Utility Functions

   Copyright (C) 2003, 2004, 2005 FreeIPMI Core Team

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.  
*/

#ifndef _IPMI_CIPHER_SUITE_UTILS_H
#define	_IPMI_CIPHER_SUITE_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

int8_t ipmi_cipher_suite_id_to_algorithms(uint8_t cipher_suite_id,
					  uint8_t *authentication_algorithm,
					  uint8_t *integrity_algorithm,
					  uint8_t *confidentiality_algorithm);

int8_t ipmi_algorithms_to_cipher_suite_id(uint8_t authentication_algorithm,
					  uint8_t integrity_algorithm,
					  uint8_t confidentiality_algorithm,
					  uint8_t *cipher_suite_id);

#ifdef __cplusplus
}
#endif

#endif /* ipmi-cipher_suite-utils.h */
