/* Copyright (c) 2010 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * Common functions between firmware and kernel verified boot.
 */

#ifndef VBOOT_REFERENCE_VBOOT_COMMON_H_
#define VBOOT_REFERENCE_VBOOT_COMMON_H_

#include <stdint.h>

#include "cryptolib.h"
#include "vboot_struct.h"

/* Error Codes for VerifyFirmware. */
#define VBOOT_SUCCESS 0
#define VBOOT_INVALID_IMAGE 1
#define VBOOT_KEY_SIGNATURE_FAILED 2
#define VBOOT_INVALID_ALGORITHM 3
#define VBOOT_PREAMBLE_SIGNATURE_FAILED 4
#define VBOOT_SIGNATURE_FAILED 5
#define VBOOT_WRONG_MAGIC 6
#define VBOOT_ERROR_MAX 7  /* Generic catch-all. */

extern char* kVbootErrors[VBOOT_ERROR_MAX];


/* Return offset of ptr from base. */
uint64_t OffsetOf(const void* base, const void* ptr);


/* Helper functions to get data pointed to by a public key or signature. */
uint8_t* GetPublicKeyData(VbPublicKey* key);
const uint8_t* GetPublicKeyDataC(const VbPublicKey* key);
uint8_t* GetSignatureData(VbSignature* sig);
const uint8_t* GetSignatureDataC(const VbSignature* sig);


/* Helper functions to verify the data pointed to by a subfield is inside
 * the parent data.  Returns 0 if inside, 1 if error. */
int VerifyMemberInside(const void* parent, uint64_t parent_size,
                       const void* member, uint64_t member_size,
                       uint64_t member_data_offset,
                       uint64_t member_data_size);

int VerifyPublicKeyInside(const void* parent, uint64_t parent_size,
                          const VbPublicKey* key);

int VerifySignatureInside(const void* parent, uint64_t parent_size,
                          const VbSignature* sig);


/* Converts a public key to RsaPublicKey format.  The returned key must
 * be freed using RSAPublicKeyFree().
 *
 * Returns NULL if error. */
RSAPublicKey* PublicKeyToRSA(const VbPublicKey* key);


/* Verifies [data] matches signature [sig] using [key]. */
int VerifyData(const uint8_t* data, const VbSignature* sig,
               const RSAPublicKey* key);


/* Checks the sanity of a key block of size [size] bytes, using public
 * key [key].  If [key]==NULL, uses only the block checksum to verify
 * the key block.  Header fields are also checked for sanity.  Does not
 * verify key index or key block flags. */
int VerifyKeyBlock(const VbKeyBlockHeader* block, uint64_t size,
                   const VbPublicKey *key);


/* Checks the sanity of a firmware preamble of size [size] bytes,
 * using public key [key].
 *
 * Returns VBOOT_SUCCESS if successful. */
int VerifyFirmwarePreamble2(const VbFirmwarePreambleHeader* preamble,
                           uint64_t size, const RSAPublicKey* key);


/* Checks the sanity of a kernel preamble of size [size] bytes,
 * using public key [key].
 *
 * Returns VBOOT_SUCCESS if successful. */
int VerifyKernelPreamble2(const VbKernelPreambleHeader* preamble,
                         uint64_t size, const RSAPublicKey* key);




#endif  /* VBOOT_REFERENCE_VBOOT_COMMON_H_ */