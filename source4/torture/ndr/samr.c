/* 
   Unix SMB/CIFS implementation.
   test suite for samr ndr operations

   Copyright (C) Jelmer Vernooij 2007
   Copyright (C) Guenther Deschner 2011
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "includes.h"
#include "torture/ndr/ndr.h"
#include "librpc/gen_ndr/ndr_samr.h"
#include "torture/ndr/proto.h"

static const uint8_t samr_connect5_in_data[] = {
  0xa8, 0x71, 0x0e, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x1a, 0x00, 0x00, 0x00, 0x5c, 0x00, 0x5c, 0x00, 0x61, 0x00, 0x6d, 0x00,
  0x79, 0x00, 0x2e, 0x00, 0x73, 0x00, 0x61, 0x00, 0x6d, 0x00, 0x62, 0x00,
  0x61, 0x00, 0x34, 0x00, 0x2e, 0x00, 0x61, 0x00, 0x62, 0x00, 0x61, 0x00,
  0x72, 0x00, 0x74, 0x00, 0x6c, 0x00, 0x65, 0x00, 0x74, 0x00, 0x2e, 0x00,
  0x6e, 0x00, 0x65, 0x00, 0x74, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};

static const uint8_t samr_connect5_out_data[] = {
  0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc9, 0xe9, 0xb9, 0x40,
  0x50, 0x94, 0xa5, 0x4d, 0xb1, 0x9b, 0x3a, 0x32, 0xd0, 0xd4, 0x45, 0x0b,
  0x00, 0x00, 0x00, 0x00
};

static const uint8_t samr_opendomain_in_data[] = {
  0x00, 0x00, 0x00, 0x00, 0xc9, 0xe9, 0xb9, 0x40, 0x50, 0x94, 0xa5, 0x4d,
  0xb1, 0x9b, 0x3a, 0x32, 0xd0, 0xd4, 0x45, 0x0b, 0x00, 0x02, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05,
  0x15, 0x00, 0x00, 0x00, 0xfa, 0x50, 0x5e, 0x04, 0x12, 0x95, 0x23, 0x02,
  0xe5, 0xa3, 0xd0, 0x03
};

static const uint8_t samr_opendomain_out_data[] = {
  0x01, 0x00, 0x00, 0x00, 0x60, 0xf0, 0x16, 0x2b, 0x52, 0x19, 0x4a, 0x47,
  0x9e, 0x88, 0x8b, 0xe8, 0x93, 0xe6, 0xbf, 0x36, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t samr_lookupnamesindomain_in_data[] = {
  0x01, 0x00, 0x00, 0x00, 0x60, 0xf0, 0x16, 0x2b, 0x52, 0x19, 0x4a, 0x47,
  0x9e, 0x88, 0x8b, 0xe8, 0x93, 0xe6, 0xbf, 0x36, 0x01, 0x00, 0x00, 0x00,
  0xe8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x12, 0x00, 0x14, 0x00, 0x60, 0x6f, 0x15, 0x00, 0x0a, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x58, 0x00, 0x50, 0x00,
  0x50, 0x00, 0x52, 0x00, 0x4f, 0x00, 0x53, 0x00, 0x50, 0x00, 0x32, 0x00,
  0x24, 0x00
};

static const uint8_t samr_lookupnamesindomain_out_data[] = {
  0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0xeb, 0x03, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t samr_openuser_in_data[] = {
  0x01, 0x00, 0x00, 0x00, 0x60, 0xf0, 0x16, 0x2b, 0x52, 0x19, 0x4a, 0x47,
  0x9e, 0x88, 0x8b, 0xe8, 0x93, 0xe6, 0xbf, 0x36, 0xb0, 0x00, 0x00, 0x00,
  0xeb, 0x03, 0x00, 0x00
};

static const uint8_t samr_openuser_out_data[] = {
  0x02, 0x00, 0x00, 0x00, 0xd8, 0x6e, 0xec, 0x8c, 0xe1, 0x1f, 0x2d, 0x41,
  0x99, 0x53, 0x13, 0xe9, 0xa4, 0x51, 0xe8, 0x1d, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t samr_queryuserinfo_in_data[] = {
  0x02, 0x00, 0x00, 0x00, 0xd8, 0x6e, 0xec, 0x8c, 0xe1, 0x1f, 0x2d, 0x41,
  0x99, 0x53, 0x13, 0xe9, 0xa4, 0x51, 0xe8, 0x1d, 0x10, 0x00
};

static const uint8_t samr_queryuserinfo_out_data[] = {
  0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};

static const uint8_t samr_setuserinfo_in_data[] = {
  0x02, 0x00, 0x00, 0x00, 0xd8, 0x6e, 0xec, 0x8c, 0xe1, 0x1f, 0x2d, 0x41,
  0x99, 0x53, 0x13, 0xe9, 0xa4, 0x51, 0xe8, 0x1d, 0x10, 0x00, 0x10, 0x00,
  0x80, 0x00, 0x00, 0x00
};

static const uint8_t samr_setuserinfo_out_data[] = {
  0x00, 0x00, 0x00, 0x00
};

static const uint8_t samr_setuserinfo2_in_data[] = {
  0x02, 0x00, 0x00, 0x00, 0xd8, 0x6e, 0xec, 0x8c, 0xe1, 0x1f, 0x2d, 0x41,
  0x99, 0x53, 0x13, 0xe9, 0xa4, 0x51, 0xe8, 0x1d, 0x1a, 0x00, 0x1a, 0x00,
  0xe9, 0x83, 0x46, 0xc5, 0x1b, 0xc6, 0xd9, 0x97, 0xaf, 0x87, 0xf4, 0xb4,
  0xd4, 0x58, 0x0f, 0xfe, 0x00, 0x86, 0x28, 0x09, 0xc6, 0x76, 0x77, 0x85,
  0xd7, 0xa1, 0xf6, 0xb1, 0xe4, 0xed, 0xe1, 0xca, 0x6a, 0xf8, 0x44, 0x2a,
  0x11, 0x10, 0x16, 0x70, 0x9c, 0x55, 0x03, 0x0f, 0xfe, 0xdb, 0x41, 0xb7,
  0xed, 0x10, 0xc9, 0x3c, 0x8b, 0x28, 0xe6, 0x94, 0x6e, 0x69, 0xc2, 0x9d,
  0x81, 0x22, 0xe1, 0xc8, 0xb3, 0xb8, 0xc0, 0x81, 0x50, 0x2f, 0xf3, 0xad,
  0x2b, 0x2e, 0xfe, 0xf6, 0x58, 0x14, 0x2d, 0x33, 0x4a, 0x74, 0x58, 0x4c,
  0xfe, 0x38, 0xe6, 0x21, 0xc3, 0x65, 0x29, 0xc3, 0xc7, 0x77, 0xb7, 0x1c,
  0xfe, 0x0b, 0x22, 0xb7, 0x2b, 0xab, 0x3e, 0x9b, 0xd1, 0x8f, 0xd9, 0x07,
  0x14, 0x65, 0x37, 0x35, 0x9d, 0x08, 0xdb, 0x81, 0x7d, 0x8d, 0x96, 0x96,
  0x9d, 0x14, 0x8d, 0xeb, 0x4f, 0x0b, 0x68, 0xee, 0xf1, 0x64, 0xf4, 0x27,
  0x75, 0x13, 0xaf, 0x1a, 0x41, 0xc8, 0x96, 0x98, 0xe2, 0x8c, 0x33, 0x98,
  0x4b, 0xa3, 0x98, 0xa9, 0xdd, 0xfe, 0x37, 0x86, 0xdd, 0x18, 0xea, 0x77,
  0x44, 0xfc, 0xba, 0xdb, 0xfd, 0xfb, 0x40, 0x01, 0x65, 0x05, 0x1e, 0x73,
  0x93, 0x25, 0xc4, 0x73, 0xf7, 0x1b, 0xd9, 0xd8, 0xbc, 0xb4, 0xc4, 0x0c,
  0x47, 0x3c, 0xc3, 0x62, 0xd5, 0xf3, 0x0b, 0x00, 0x74, 0x75, 0x09, 0x7e,
  0x58, 0x40, 0x9c, 0x0b, 0x2f, 0x15, 0x26, 0xa9, 0xdc, 0xe6, 0xd2, 0x5a,
  0xf2, 0xef, 0xd2, 0x4d, 0x6f, 0x2f, 0x86, 0xe9, 0x95, 0xba, 0xfe, 0xe0,
  0x13, 0x7a, 0xb3, 0x01, 0x93, 0x23, 0x0f, 0x43, 0xf7, 0x40, 0x1c, 0xbe,
  0x2e, 0x25, 0x61, 0x35, 0xda, 0x5a, 0x43, 0x82, 0x3b, 0xff, 0x05, 0x08,
  0x7f, 0x64, 0xf2, 0xc9, 0xeb, 0x71, 0x34, 0x9c, 0x37, 0x77, 0xe9, 0x5a,
  0x23, 0x89, 0xdc, 0x88, 0xa5, 0x27, 0x16, 0x05, 0xc8, 0xf1, 0xbf, 0xbb,
  0xb3, 0xe7, 0xa8, 0x08, 0xf5, 0xe9, 0x46, 0xc9, 0x63, 0xb4, 0x5d, 0x11,
  0x38, 0x69, 0x49, 0x5d, 0x92, 0x95, 0x25, 0x35, 0x56, 0x4b, 0x3e, 0xba,
  0x6b, 0xb3, 0x99, 0x72, 0x70, 0x1c, 0xb5, 0x7e, 0x26, 0x5c, 0xbf, 0xd0,
  0xbf, 0xd8, 0x58, 0xf4, 0xeb, 0xc2, 0x37, 0xad, 0x98, 0x7d, 0xa8, 0x05,
  0x7e, 0xf7, 0x48, 0xd9, 0x73, 0x49, 0x39, 0xaa, 0x02, 0x75, 0x67, 0x5b,
  0x44, 0xda, 0xda, 0x01, 0xe6, 0x5b, 0x4e, 0x0a, 0x15, 0xe7, 0x63, 0x70,
  0x1c, 0x16, 0x73, 0x79, 0x24, 0x3d, 0x69, 0x30, 0x85, 0xb1, 0x50, 0x65,
  0xa1, 0x12, 0x73, 0xf1, 0xaf, 0x8d, 0xe9, 0x23, 0x25, 0x99, 0xa2, 0x8a,
  0x83, 0x6a, 0x39, 0x99, 0xe6, 0x6c, 0xd0, 0xe1, 0x58, 0x9a, 0xb2, 0x3f,
  0x02, 0x77, 0x48, 0x3a, 0xb0, 0x9e, 0x1a, 0x33, 0x51, 0x5e, 0xe2, 0x46,
  0xe6, 0x3d, 0x0f, 0x01, 0x64, 0xc6, 0xd1, 0xe9, 0x42, 0xf6, 0xe0, 0x38,
  0x1a, 0x33, 0xc7, 0x30, 0x80, 0xa6, 0x28, 0xc5, 0x18, 0xbb, 0xe0, 0x5a,
  0x8b, 0xf2, 0x33, 0x53, 0x4d, 0xbf, 0xe1, 0x4c, 0x98, 0x7e, 0x79, 0x1a,
  0x0a, 0xdc, 0xdc, 0x04, 0x2e, 0x58, 0x57, 0xba, 0xde, 0x09, 0xa1, 0xe0,
  0x5b, 0xfc, 0x38, 0x90, 0x58, 0x00, 0xf1, 0xa3, 0x9e, 0x3d, 0x51, 0x7c,
  0x1e, 0x50, 0xfa, 0x15, 0x55, 0xb2, 0xde, 0x8b, 0x27, 0xc2, 0xbe, 0xbf,
  0x27, 0xa4, 0x5b, 0x56, 0x38, 0x97, 0xbf, 0x3d, 0xe1, 0x73, 0x22, 0x98,
  0x9e, 0x25, 0x6d, 0x5d, 0xc5, 0x05, 0xdd, 0x72, 0x7d, 0x50, 0x06, 0x32,
  0xd8, 0x3f, 0x16, 0x13, 0x7e, 0x5e, 0xc9, 0x45, 0xf0, 0xa7, 0xc2, 0xeb,
  0xda, 0x79, 0xef, 0x62, 0x3a, 0x96, 0x58, 0x4a, 0xc7, 0xf8, 0xc1, 0xba,
  0x0b, 0x94, 0xb8, 0xd6, 0x78, 0x6d, 0xc1, 0x42, 0xff, 0xc4, 0x8f, 0x5f,
  0x3f, 0x21, 0xc1, 0x0f, 0x5f, 0x42, 0xc9, 0xbb, 0xfd, 0x0f, 0x71, 0xf6,
  0xcc, 0xfe, 0x81, 0x20, 0x00
};

static const uint8_t samr_setuserinfo2_out_data[] = {
  0x00, 0x00, 0x00, 0x00
};

static const uint8_t samr_getuserpwinfo_in_data[] = {
  0x02, 0x00, 0x00, 0x00, 0xd8, 0x6e, 0xec, 0x8c, 0xe1, 0x1f, 0x2d, 0x41,
  0x99, 0x53, 0x13, 0xe9, 0xa4, 0x51, 0xe8, 0x1d
};

static const uint8_t samr_getuserpwinfo_out_data[] = {
  0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t samr_closehandle_in_data[] = {
  0x02, 0x00, 0x00, 0x00, 0xd8, 0x6e, 0xec, 0x8c, 0xe1, 0x1f, 0x2d, 0x41,
  0x99, 0x53, 0x13, 0xe9, 0xa4, 0x51, 0xe8, 0x1d
};

static const uint8_t samr_closehandle_out_data[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t samr_changepassworduser3_w2k_in_data[] = {
	0x00, 0x00, 0x02, 0x00, 0x10, 0x00, 0x10, 0x00, 0x04, 0x00, 0x02, 0x00,
	0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
	0x5c, 0x00, 0x5c, 0x00, 0x57, 0x00, 0x32, 0x00, 0x4b, 0x00, 0x53, 0x00,
	0x52, 0x00, 0x56, 0x00, 0x04, 0x00, 0x04, 0x00, 0x08, 0x00, 0x02, 0x00,
	0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
	0x67, 0x00, 0x64, 0x00, 0x0c, 0x00, 0x02, 0x00, 0x8a, 0x5d, 0x1b, 0x5c,
	0xf4, 0xf8, 0xb6, 0x79, 0xe0, 0xbf, 0x34, 0x0a, 0x9b, 0x73, 0xdb, 0x34,
	0x6e, 0xbe, 0xe1, 0x1f, 0x75, 0x33, 0xff, 0x3c, 0xa1, 0xca, 0xc0, 0xa8,
	0xfa, 0x78, 0xbe, 0x93, 0x9d, 0xb7, 0x9e, 0x15, 0xad, 0x6e, 0x27, 0xb3,
	0x01, 0xdb, 0x6c, 0xb8, 0x5a, 0x2b, 0x73, 0xb9, 0x5a, 0x25, 0xd1, 0xf7,
	0x33, 0xe8, 0x7e, 0x97, 0x7e, 0xb4, 0x90, 0x89, 0x85, 0xc8, 0x20, 0x8d,
	0x25, 0x02, 0x68, 0xb9, 0xe4, 0x09, 0xd1, 0xd5, 0x50, 0x6e, 0x83, 0x0a,
	0x21, 0x45, 0x33, 0xb2, 0xe8, 0x3b, 0xb6, 0x5a, 0x8e, 0x3c, 0x36, 0x51,
	0xe9, 0xb5, 0x22, 0xfe, 0xeb, 0x19, 0x44, 0xa1, 0x01, 0xa9, 0x0c, 0x2c,
	0x5a, 0xd1, 0xfe, 0x69, 0x10, 0x86, 0xae, 0x78, 0x01, 0xaa, 0x5d, 0xbb,
	0x16, 0x02, 0x2b, 0xa5, 0x26, 0xcc, 0x7c, 0x9d, 0xc0, 0x6c, 0x9c, 0x24,
	0x5b, 0x8b, 0x34, 0xc3, 0xbc, 0x40, 0x5e, 0x3d, 0x7c, 0x2a, 0x55, 0x36,
	0xfd, 0x34, 0x08, 0x08, 0x05, 0xca, 0xd2, 0xd1, 0x0e, 0xe4, 0x8f, 0xea,
	0xfe, 0x3f, 0x7d, 0x56, 0x3c, 0x9a, 0x59, 0x68, 0x83, 0x89, 0x21, 0x3e,
	0x7d, 0x33, 0xfd, 0x73, 0x70, 0xa6, 0xec, 0x36, 0xe8, 0x9d, 0x62, 0x40,
	0x0e, 0x97, 0x3d, 0xb0, 0xc9, 0x9d, 0x11, 0xb1, 0x55, 0x51, 0xd0, 0x1d,
	0x72, 0xdb, 0xf1, 0x18, 0x0a, 0x39, 0x94, 0x0f, 0xf8, 0x9a, 0xba, 0x79,
	0x75, 0x37, 0x83, 0x8d, 0x1a, 0x8e, 0x8b, 0x38, 0x0a, 0x44, 0xf1, 0x46,
	0x60, 0xef, 0x8b, 0xd8, 0x83, 0xb7, 0x10, 0x91, 0x2a, 0x42, 0x09, 0x0f,
	0x6a, 0x43, 0xa9, 0xca, 0x44, 0x5d, 0x7f, 0x3b, 0x96, 0xde, 0xa9, 0xd3,
	0x69, 0x3f, 0x0e, 0x27, 0x52, 0x32, 0x54, 0xcf, 0x4c, 0xcb, 0xf1, 0x84,
	0xab, 0x13, 0x5e, 0x56, 0x18, 0x96, 0xce, 0x67, 0x4f, 0x73, 0xf8, 0xb2,
	0xde, 0x82, 0xa6, 0x0c, 0x15, 0x72, 0x73, 0x1a, 0x00, 0x9a, 0x54, 0x85,
	0x4d, 0x83, 0x6d, 0x78, 0x13, 0xc5, 0x7c, 0x86, 0xa9, 0x4b, 0x34, 0x54,
	0xbc, 0x40, 0x13, 0x9d, 0x99, 0x0d, 0xa4, 0xc8, 0xeb, 0x6e, 0xef, 0x3f,
	0x94, 0x6f, 0xc4, 0x4d, 0x2d, 0x13, 0x9f, 0xd6, 0x29, 0xc6, 0x55, 0xc1,
	0x73, 0x86, 0xe7, 0x77, 0xe2, 0x85, 0xb4, 0x03, 0xaf, 0xe2, 0x7a, 0x9c,
	0x62, 0x8e, 0xc7, 0xcb, 0xa5, 0x0c, 0x1f, 0xd5, 0xa2, 0x6b, 0x59, 0xb5,
	0xe7, 0xde, 0xf9, 0x1c, 0xa0, 0x96, 0x48, 0xcd, 0x20, 0x52, 0x23, 0x23,
	0xfb, 0x88, 0x91, 0xda, 0x64, 0x41, 0x24, 0xd4, 0x30, 0x1e, 0x92, 0x69,
	0x4e, 0xad, 0xb9, 0x41, 0x0f, 0x7f, 0x00, 0xdc, 0xdd, 0x17, 0xe8, 0x56,
	0xfc, 0xbd, 0x2f, 0x57, 0x46, 0x41, 0x5a, 0xab, 0xe8, 0xbc, 0x81, 0xc1,
	0xdf, 0x2b, 0x5b, 0xd0, 0xb8, 0x2b, 0x54, 0xaf, 0x8c, 0xd1, 0x1a, 0x91,
	0x93, 0x61, 0x21, 0xd7, 0x65, 0xc4, 0x3a, 0x8b, 0xf5, 0xb5, 0x4e, 0x9b,
	0xf9, 0xe5, 0x77, 0x59, 0x25, 0x60, 0xd0, 0xe4, 0x73, 0x58, 0x1b, 0x03,
	0x9c, 0xf4, 0x80, 0x82, 0xd1, 0xa2, 0x27, 0xe9, 0x60, 0x87, 0xfd, 0x7d,
	0x8f, 0x25, 0x6d, 0x66, 0x8c, 0xb3, 0x7e, 0x92, 0x6a, 0xae, 0x10, 0x10,
	0x29, 0xcc, 0x7a, 0xeb, 0x07, 0x6e, 0x82, 0x01, 0xd0, 0xee, 0xa0, 0xb3,
	0x2f, 0xf0, 0x9d, 0x4c, 0x82, 0x1d, 0x3e, 0x07, 0xf1, 0xbe, 0x64, 0x01,
	0x6a, 0xf8, 0x28, 0xb4, 0x08, 0x51, 0xf0, 0x01, 0x1b, 0x1b, 0x58, 0x5e,
	0x4b, 0x8c, 0x02, 0x92, 0xcb, 0x80, 0x26, 0x9d, 0x60, 0x33, 0xee, 0x6a,
	0x17, 0x39, 0x46, 0x3d, 0x10, 0x04, 0x6d, 0x60, 0x91, 0xdd, 0xb7, 0x6c,
	0xd5, 0x28, 0x36, 0x75, 0x32, 0xf8, 0xd7, 0xc3, 0xe7, 0x90, 0x82, 0xdc,
	0x2a, 0xe8, 0x72, 0x05, 0x95, 0x96, 0x07, 0x40, 0x10, 0x00, 0x02, 0x00,
	0xa7, 0x0f, 0x62, 0x18, 0xfb, 0xca, 0x87, 0x81, 0x92, 0x4a, 0x42, 0xa1,
	0x04, 0x9b, 0xf8, 0x65, 0x01, 0x00, 0x00, 0x00, 0x14, 0x00, 0x02, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x18, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t samr_changepassworduser3_w2k_out_data[] = {
	0xbb, 0x00, 0x00, 0xc0
};

static const uint8_t samr_changepassworduser3_w2k8r2_out_data[] = {
	0x00, 0x00, 0x02, 0x00, 0x07, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x00, 0x80, 0xa6, 0x0a, 0xff, 0xde, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x02, 0x00, 0x05, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6c, 0x00, 0x00, 0xc0
};

static bool samr_changepassworduser3_w2k8r2_out_check(struct torture_context *tctx,
						      struct samr_ChangePasswordUser3 *r)
{
	struct samr_DomInfo1 *dominfo = *r->out.dominfo;
	struct userPwdChangeFailureInformation *reject = *r->out.reject;

	torture_assert_int_equal(tctx, dominfo->min_password_length, 7, "min_password_length");
	torture_assert_int_equal(tctx, dominfo->password_history_length, 0, "password_history_length");
	torture_assert_int_equal(tctx, dominfo->password_properties, DOMAIN_PASSWORD_COMPLEX, "password_properties");
	torture_assert_u64_equal(tctx, dominfo->max_password_age, 0xffffdeff0aa68000, "max_password_age");
	torture_assert_u64_equal(tctx, dominfo->min_password_age, 0x0000000000000000, "min_password_age");

	torture_assert_int_equal(tctx, reject->extendedFailureReason, SAM_PWD_CHANGE_NOT_COMPLEX, "extendedFailureReason");
	torture_assert_int_equal(tctx, reject->filterModuleName.length, 0, "filterModuleName.length");
	torture_assert_int_equal(tctx, reject->filterModuleName.size, 0, "filterModuleName.size");

	torture_assert_ntstatus_equal(tctx, r->out.result, NT_STATUS_PASSWORD_RESTRICTION, "result");

	return true;
}

struct torture_suite *ndr_samr_suite(TALLOC_CTX *ctx)
{
	struct torture_suite *suite = torture_suite_create(ctx, "samr");

	torture_suite_add_ndr_pull_fn_test(suite, samr_Connect5, samr_connect5_in_data, NDR_IN, NULL);
	torture_suite_add_ndr_pull_fn_test(suite, samr_Connect5, samr_connect5_out_data, NDR_OUT, NULL);

	torture_suite_add_ndr_pull_fn_test(suite, samr_LookupNames, samr_lookupnamesindomain_in_data, NDR_IN, NULL);
	torture_suite_add_ndr_pull_fn_test(suite, samr_LookupNames, samr_lookupnamesindomain_out_data, NDR_OUT, NULL);

	torture_suite_add_ndr_pull_fn_test(suite, samr_OpenDomain, samr_opendomain_in_data, NDR_IN, NULL);
	torture_suite_add_ndr_pull_fn_test(suite, samr_OpenDomain, samr_opendomain_out_data, NDR_OUT, NULL);

	torture_suite_add_ndr_pull_fn_test(suite, samr_OpenUser, samr_openuser_in_data, NDR_IN, NULL);
	torture_suite_add_ndr_pull_fn_test(suite, samr_OpenUser, samr_openuser_out_data, NDR_OUT, NULL);

	torture_suite_add_ndr_pull_fn_test(suite, samr_QueryUserInfo, samr_queryuserinfo_in_data, NDR_IN, NULL);
	torture_suite_add_ndr_pull_io_test(suite, samr_QueryUserInfo, samr_queryuserinfo_in_data, samr_queryuserinfo_out_data, NULL);

	torture_suite_add_ndr_pull_fn_test(suite, samr_SetUserInfo, samr_setuserinfo_in_data, NDR_IN, NULL);
	torture_suite_add_ndr_pull_fn_test(suite, samr_SetUserInfo, samr_setuserinfo_out_data, NDR_OUT, NULL);

	torture_suite_add_ndr_pull_fn_test(suite, samr_SetUserInfo2, samr_setuserinfo2_in_data, NDR_IN, NULL);
	torture_suite_add_ndr_pull_fn_test(suite, samr_SetUserInfo2, samr_setuserinfo2_out_data, NDR_OUT, NULL);

	torture_suite_add_ndr_pull_fn_test(suite, samr_GetUserPwInfo, samr_getuserpwinfo_in_data, NDR_IN, NULL);
	torture_suite_add_ndr_pull_fn_test(suite, samr_GetUserPwInfo, samr_getuserpwinfo_out_data, NDR_OUT, NULL);

	torture_suite_add_ndr_pull_fn_test(suite, samr_Close, samr_closehandle_in_data, NDR_IN, NULL);
	torture_suite_add_ndr_pull_fn_test(suite, samr_Close, samr_closehandle_out_data, NDR_OUT, NULL);

	torture_suite_add_ndr_pull_fn_test(suite, samr_ChangePasswordUser3, samr_changepassworduser3_w2k_in_data, NDR_IN, NULL);
#if 0
	/* Samba currently fails to parse a w2k reply */
	torture_suite_add_ndr_pull_fn_test(suite, samr_ChangePasswordUser3, samr_changepassworduser3_w2k_out_data, NDR_OUT, NULL);
#endif
	torture_suite_add_ndr_pull_fn_test(suite,
					   samr_ChangePasswordUser3,
					   samr_changepassworduser3_w2k8r2_out_data,
					   NDR_OUT,
					   samr_changepassworduser3_w2k8r2_out_check);

	return suite;
}

