/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

/*! \file nr_nas_msg.c
 * \brief Definitions of handlers and callbacks for NR NAS UE task
 * \author Yoshio INOUE, Masayuki HARADA
 * \email yoshio.inoue@fujitsu.com,masayuki.harada@fujitsu.com
 * \date 2020
 * \version 0.1
 *
 * 2023.01.27 Vladimir Dorovskikh 16 digits IMEISV
 */

#include "nr_nas_msg.h"
#include <netinet/in.h>
#include "NR_NAS_defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "AuthenticationResponseParameter.h"
#include "FGCNasMessageContainer.h"
#include "FGSDeregistrationRequestUEOriginating.h"
#include "FGSDeregistrationType.h"
#include "NasKeySetIdentifier.h"
#include "NrUESecurityCapability.h"
#include "OctetString.h"
#include "PduSessionEstablishRequest.h"
#include "PduSessionEstablishmentAccept.h"
#include "RegistrationAccept.h"
#include "SORTransparentContainer.h"
#include "FGSIdentityResponse.h"
#include "T.h"
#include "aka_functions.h"
#include "assertions.h"
#include "common/utils/ds/byte_array.h"
#include "common/utils/tun_if.h"
#include "commonDef.h"
#include "intertask_interface.h"
#include "kdf.h"
#include "key_nas_deriver.h"
#include "nas_log.h"
#include "openair3/UICC/usim_interface.h"
#include "openair3/UTILS/conversions.h"
#include "secu_defs.h"
#include "utils.h"
#include "openair2/SDAP/nr_sdap/nr_sdap.h"
#include "fgs_nas_utils.h"

#define MAX_NAS_UE 4

static nr_ue_nas_t nr_ue_nas[MAX_NAS_UE] = {0};

#define FOREACH_STATE(TYPE_DEF)                  \
  TYPE_DEF(NAS_SECURITY_NO_SECURITY_CONTEXT, 0)  \
  TYPE_DEF(NAS_SECURITY_NEW_SECURITY_CONTEXT, 1) \
  TYPE_DEF(NAS_SECURITY_UNPROTECTED, 2)          \
  TYPE_DEF(NAS_SECURITY_INTEGRITY_FAILED, 3)     \
  TYPE_DEF(NAS_SECURITY_INTEGRITY_PASSED, 4)     \
  TYPE_DEF(NAS_SECURITY_BAD_INPUT, 5)

typedef enum { FOREACH_STATE(TO_ENUM) } security_state_t;

static const text_info_t security_state_info[] = {FOREACH_STATE(TO_TEXT)};

static fgmm_msg_header_t set_mm_header(fgs_nas_msg_t type, Security_header_t security)
{
  fgmm_msg_header_t mm_header = {0};
  mm_header.ex_protocol_discriminator = FGS_MOBILITY_MANAGEMENT_MESSAGE;
  mm_header.security_header_type = security;
  mm_header.message_type = type;
  return mm_header;
}

static void servingNetworkName(uint8_t *msg, char *imsiStr, int mnc_size)
{
  // SNN-network-identifier in TS 24.501
  // TS 24.501: If the MNC of the serving PLMN has two digits, then a zero is added at the beginning.

  // MNC
  char mnc[4];
  if (mnc_size == 2) {
    snprintf(mnc, sizeof(mnc), "0%c%c", imsiStr[3], imsiStr[4]);
  } else {
    snprintf(mnc, sizeof(mnc), "%c%c%c", imsiStr[3], imsiStr[4], imsiStr[5]);
  }

  // MCC
  char mcc[4];
  snprintf(mcc, sizeof(mcc), "%c%c%c", imsiStr[0], imsiStr[1], imsiStr[2]);

  int size = 64;

  snprintf((char *)msg, size, "5G:mnc%3s.mcc%3s.3gppnetwork.org", mnc, mcc);
}

static security_state_t nas_security_rx_process(nr_ue_nas_t *nas, uint8_t *pdu_buffer, int pdu_length)
{
  if (nas->security_container == NULL)
    return NAS_SECURITY_NO_SECURITY_CONTEXT;
  /* header is 7 bytes, require at least one byte of payload */
  if (pdu_length < 8)
    return NAS_SECURITY_BAD_INPUT;

  switch (pdu_buffer[1]) {
    case PLAIN_5GS_MSG:
      return NAS_SECURITY_UNPROTECTED;
      break;
    case INTEGRITY_PROTECTED:
    case INTEGRITY_PROTECTED_WITH_NEW_SECU_CTX:
    case INTEGRITY_PROTECTED_AND_CIPHERED_WITH_NEW_SECU_CTX:
      /* only accept "integrity protected and ciphered" messages */
      if (pdu_buffer[6] == 0)
        LOG_E(NAS, "Received nas_count_dl = %d\n", pdu_buffer[6]);
      LOG_E(NAS, "todo: unhandled security type %s (pdu_buffer[1] = %d)\n", security_header_type_s[pdu_buffer[1]].text, pdu_buffer[1]);
      return NAS_SECURITY_BAD_INPUT;
      break;
    default:
      break;
  }

  /* synchronize NAS SQN, based on 24.501 4.4.3.1 */
  // Sequence number
  int nas_sqn = pdu_buffer[6];
  int target_sqn = nas->security.nas_count_dl & 0xff;
  if (nas_sqn != target_sqn) {
    if (nas_sqn < target_sqn)
      nas->security.nas_count_dl += 256;
    nas->security.nas_count_dl &= ~255;
    nas->security.nas_count_dl |= nas_sqn;
  }
  if (nas->security.nas_count_dl > 0x00ffffff) {
    /* it's doubtful that this will happen, so let's simply exit for the time being */
    /* to be refined if needed */
    LOG_E(NAS, "max NAS COUNT DL reached\n");
    exit(1);
  }

  /* check integrity */
  uint8_t computed_mac[NAS_INTEGRITY_SIZE];
  nas_stream_cipher_t stream_cipher;
  stream_cipher.context = nas->security_container->integrity_context;
  stream_cipher.count = nas->security.nas_count_dl;
  stream_cipher.bearer = 1; /* todo: don't hardcode */
  stream_cipher.direction = 1;
  stream_cipher.message = pdu_buffer + SECURITY_PROTECTED_5GS_NAS_MESSAGE_HEADER_LENGTH - 1;
  /* length in bits */
  stream_cipher.blength = (pdu_length - SECURITY_PROTECTED_5GS_NAS_MESSAGE_HEADER_LENGTH + 1) << 3;
  stream_compute_integrity(nas->security_container->integrity_algorithm, &stream_cipher, computed_mac);

  uint8_t *received_mac = pdu_buffer + 2;

  if (memcmp(received_mac, computed_mac, NAS_INTEGRITY_SIZE) != 0)
    return NAS_SECURITY_INTEGRITY_FAILED;

  /* decipher */
  uint8_t payload_len = pdu_length - SECURITY_PROTECTED_5GS_NAS_MESSAGE_HEADER_LENGTH;
  uint8_t buf[payload_len];
  stream_cipher.context = nas->security_container->ciphering_context;
  stream_cipher.count = nas->security.nas_count_dl;
  stream_cipher.bearer = 1; /* todo: don't hardcode */
  stream_cipher.direction = 1;
  stream_cipher.message = pdu_buffer + SECURITY_PROTECTED_5GS_NAS_MESSAGE_HEADER_LENGTH;
  /* length in bits */
  stream_cipher.blength = (payload_len) << 3;
  stream_compute_encrypt(nas->security_container->ciphering_algorithm, &stream_cipher, buf);
  memcpy(pdu_buffer + SECURITY_PROTECTED_5GS_NAS_MESSAGE_HEADER_LENGTH, buf, payload_len);

  nas->security.nas_count_dl++;

  return NAS_SECURITY_INTEGRITY_PASSED;
}

static int fill_suci(FGSMobileIdentity *mi, const uicc_t *uicc)
{
  mi->suci.typeofidentity = FGS_MOBILE_IDENTITY_SUCI;
  mi->suci.mncdigit1 = uicc->nmc_size == 2 ? uicc->imsiStr[3] - '0' : uicc->imsiStr[4] - '0';
  mi->suci.mncdigit2 = uicc->nmc_size == 2 ? uicc->imsiStr[4] - '0' : uicc->imsiStr[5] - '0';
  mi->suci.mncdigit3 = uicc->nmc_size == 2 ? 0xF : uicc->imsiStr[3] - '0';
  mi->suci.mccdigit1 = uicc->imsiStr[0] - '0';
  mi->suci.mccdigit2 = uicc->imsiStr[1] - '0';
  mi->suci.mccdigit3 = uicc->imsiStr[2] - '0';
  memcpy(mi->suci.schemeoutput, uicc->imsiStr + 3 + uicc->nmc_size, strlen(uicc->imsiStr) - (3 + uicc->nmc_size));
  return sizeof(Suci5GSMobileIdentity_t);
}

static int fill_guti(FGSMobileIdentity *mi, const Guti5GSMobileIdentity_t *guti)
{
  AssertFatal(guti != NULL, "UE has no GUTI\n");
  mi->guti = *guti;
  return 13;
}

static int fill_fgstmsi(Stmsi5GSMobileIdentity_t *stmsi, const Guti5GSMobileIdentity_t *guti)
{
  AssertFatal(guti != NULL, "UE has no GUTI\n");
  stmsi->amfpointer = guti->amfpointer;
  stmsi->amfsetid = guti->amfsetid;
  stmsi->tmsi = guti->tmsi;
  stmsi->digit1 = DIGIT1;
  stmsi->spare = 0;
  stmsi->typeofidentity = FGS_MOBILE_IDENTITY_5GS_TMSI;
  return 10;
}

static int fill_imeisv(FGSMobileIdentity *mi, const uicc_t *uicc)
{
  int i = 0;
  mi->imeisv.typeofidentity = FGS_MOBILE_IDENTITY_IMEISV;
  mi->imeisv.digittac01 = getImeisvDigit(uicc, i++);
  mi->imeisv.digittac02 = getImeisvDigit(uicc, i++);
  mi->imeisv.digittac03 = getImeisvDigit(uicc, i++);
  mi->imeisv.digittac04 = getImeisvDigit(uicc, i++);
  mi->imeisv.digittac05 = getImeisvDigit(uicc, i++);
  mi->imeisv.digittac06 = getImeisvDigit(uicc, i++);
  mi->imeisv.digittac07 = getImeisvDigit(uicc, i++);
  mi->imeisv.digittac08 = getImeisvDigit(uicc, i++);
  mi->imeisv.digit09 = getImeisvDigit(uicc, i++);
  mi->imeisv.digit10 = getImeisvDigit(uicc, i++);
  mi->imeisv.digit11 = getImeisvDigit(uicc, i++);
  mi->imeisv.digit12 = getImeisvDigit(uicc, i++);
  mi->imeisv.digit13 = getImeisvDigit(uicc, i++);
  mi->imeisv.digit14 = getImeisvDigit(uicc, i++);
  mi->imeisv.digitsv1 = getImeisvDigit(uicc, i++);
  mi->imeisv.digitsv2 = getImeisvDigit(uicc, i++);
  mi->imeisv.spare = 0x0f;
  mi->imeisv.oddeven = 0;
  return 19;
}

void transferRES(uint8_t ck[16], uint8_t ik[16], uint8_t *input, uint8_t rand[16], uint8_t *output, uicc_t *uicc)
{
  uint8_t S[100] = {0};
  S[0] = 0x6B;
  servingNetworkName(S + 1, uicc->imsiStr, uicc->nmc_size);
  int netNamesize = strlen((char *)S + 1);
  S[1 + netNamesize] = (netNamesize & 0xff00) >> 8;
  S[2 + netNamesize] = (netNamesize & 0x00ff);
  for (int i = 0; i < 16; i++)
    S[3 + netNamesize + i] = rand[i];
  S[19 + netNamesize] = 0x00;
  S[20 + netNamesize] = 0x10;
  for (int i = 0; i < 8; i++)
    S[21 + netNamesize + i] = input[i];
  S[29 + netNamesize] = 0x00;
  S[30 + netNamesize] = 0x08;

  uint8_t plmn[3] = {0x02, 0xf8, 0x39};
  uint8_t oldS[100];
  oldS[0] = 0x6B;
  memcpy(&oldS[1], plmn, 3);
  oldS[4] = 0x00;
  oldS[5] = 0x03;
  for (int i = 0; i < 16; i++)
    oldS[6 + i] = rand[i];
  oldS[22] = 0x00;
  oldS[23] = 0x10;
  for (int i = 0; i < 8; i++)
    oldS[24 + i] = input[i];
  oldS[32] = 0x00;
  oldS[33] = 0x08;

  uint8_t key[32] = {0};
  memcpy(&key[0], ck, 16);
  memcpy(&key[16], ik, 16); // KEY
  uint8_t out[32] = {0};

  byte_array_t data = {.buf = S, .len = 31 + netNamesize};
  kdf(key, data, 32, out);

  memcpy(output, out + 16, 16);
}

void derive_kausf(uint8_t ck[16], uint8_t ik[16], uint8_t sqn[6], uint8_t kausf[32], uicc_t *uicc)
{
  uint8_t S[100] = {0};
  uint8_t key[32] = {0};

  memcpy(&key[0], ck, 16);
  memcpy(&key[16], ik, 16); // KEY
  S[0] = 0x6A;
  servingNetworkName(S + 1, uicc->imsiStr, uicc->nmc_size);
  int netNamesize = strlen((char *)S + 1);
  S[1 + netNamesize] = (uint8_t)((netNamesize & 0xff00) >> 8);
  S[2 + netNamesize] = (uint8_t)(netNamesize & 0x00ff);
  for (int i = 0; i < 6; i++) {
    S[3 + netNamesize + i] = sqn[i];
  }
  S[9 + netNamesize] = 0x00;
  S[10 + netNamesize] = 0x06;

  byte_array_t data = {.buf = S, .len = 11 + netNamesize};
  kdf(key, data, 32, kausf);
}

void derive_kseaf(uint8_t kausf[32], uint8_t kseaf[32], uicc_t *uicc)
{
  uint8_t S[100] = {0};
  S[0] = 0x6C; // FC
  servingNetworkName(S + 1, uicc->imsiStr, uicc->nmc_size);
  int netNamesize = strlen((char *)S + 1);
  S[1 + netNamesize] = (uint8_t)((netNamesize & 0xff00) >> 8);
  S[2 + netNamesize] = (uint8_t)(netNamesize & 0x00ff);

  byte_array_t data = {.buf = S, .len = 3 + netNamesize};
  kdf(kausf, data, 32, kseaf);
}

void derive_kamf(uint8_t *kseaf, uint8_t *kamf, uint16_t abba, uicc_t *uicc)
{
  int imsiLen = strlen(uicc->imsiStr);
  uint8_t S[100] = {0};
  S[0] = 0x6D; // FC = 0x6D
  memcpy(&S[1], uicc->imsiStr, imsiLen);
  S[1 + imsiLen] = (uint8_t)((imsiLen & 0xff00) >> 8);
  S[2 + imsiLen] = (uint8_t)(imsiLen & 0x00ff);
  S[3 + imsiLen] = abba & 0x00ff;
  S[4 + imsiLen] = (abba & 0xff00) >> 8;
  S[5 + imsiLen] = 0x00;
  S[6 + imsiLen] = 0x02;

  byte_array_t data = {.buf = S, .len = 7 + imsiLen};
  kdf(kseaf, data, 32, kamf);
}

//------------------------------------------------------------------------------
void derive_knas(algorithm_type_dist_t nas_alg_type, uint8_t nas_alg_id, uint8_t kamf[32], uint8_t *knas)
{
  uint8_t S[20] = {0};
  uint8_t out[32] = {0};
  S[0] = 0x69; // FC
  S[1] = (uint8_t)(nas_alg_type & 0xFF);
  S[2] = 0x00;
  S[3] = 0x01;
  S[4] = nas_alg_id;
  S[5] = 0x00;
  S[6] = 0x01;

  byte_array_t data = {.buf = S, .len = 7};
  kdf(kamf, data, 32, out);

  memcpy(knas, out + 16, 16);
}

static void derive_kgnb(uint8_t kamf[32], uint32_t count, uint8_t *kgnb)
{
  /* Compute the KDF input parameter
   * S = FC(0x6E) || UL NAS Count || 0x00 0x04 || 0x01 || 0x00 0x01
   */
  uint8_t input[32] = {0};
  //    uint16_t length    = 4;
  //    int      offset    = 0;

  LOG_TRACE(INFO, "%s  with count= %d", __FUNCTION__, count);
  memset(input, 0, 32);
  input[0] = 0x6E;
  // P0
  input[1] = count >> 24;
  input[2] = (uint8_t)(count >> 16);
  input[3] = (uint8_t)(count >> 8);
  input[4] = (uint8_t)count;
  // L0
  input[5] = 0;
  input[6] = 4;
  // P1
  input[7] = 0x01;
  // L1
  input[8] = 0;
  input[9] = 1;

  byte_array_t data = {.buf = input, .len = 10};
  kdf(kamf, data, 32, kgnb);

  printf("kgnb : ");
  for (int pp = 0; pp < 32; pp++)
    printf("%02x ", kgnb[pp]);
  printf("\n");
}

static void derive_ue_keys(uint8_t *buf, nr_ue_nas_t *nas)
{
  uint8_t ak[6];
  uint8_t sqn[6];

  DevAssert(nas != NULL);
  uint8_t *kausf = nas->security.kausf;
  uint8_t *kseaf = nas->security.kseaf;
  uint8_t *kamf = nas->security.kamf;
  uint8_t *output = nas->security.res;
  uint8_t *rand = nas->security.rand;
  uint8_t *kgnb = nas->security.kgnb;

  // get RAND for authentication request
  for (int index = 0; index < 16; index++) {
    rand[index] = buf[8 + index];
  }

  uint8_t resTemp[16];
  uint8_t ck[16], ik[16];
  f2345(nas->uicc->key, rand, resTemp, ck, ik, ak, nas->uicc->opc);

  transferRES(ck, ik, resTemp, rand, output, nas->uicc);

  for (int index = 0; index < 6; index++) {
    sqn[index] = buf[26 + index];
  }

  derive_kausf(ck, ik, sqn, kausf, nas->uicc);
  derive_kseaf(kausf, kseaf, nas->uicc);
  derive_kamf(kseaf, kamf, 0x0000, nas->uicc);
  derive_kgnb(kamf, 0, kgnb);

  printf("kausf:");
  for (int i = 0; i < 32; i++) {
    printf("%x ", kausf[i]);
  }
  printf("\n");

  printf("kseaf:");
  for (int i = 0; i < 32; i++) {
    printf("%x ", kseaf[i]);
  }

  printf("\n");

  printf("kamf:");
  for (int i = 0; i < 32; i++) {
    printf("%x ", kamf[i]);
  }
  printf("\n");
}

nr_ue_nas_t *get_ue_nas_info(module_id_t module_id)
{
  DevAssert(module_id < MAX_NAS_UE);
  if (!nr_ue_nas[module_id].uicc) {
    nr_ue_nas[module_id].uicc = checkUicc(module_id);
    nr_ue_nas[module_id].UE_id = module_id;
  }
  return &nr_ue_nas[module_id];
}

void generateRegistrationRequest(as_nas_info_t *initialNasMsg, nr_ue_nas_t *nas)
{
  int size = sizeof(fgmm_msg_header_t);
  fgmm_nas_message_plain_t plain = {0};

  // Plain 5GMM header
  plain.header = set_mm_header(FGS_REGISTRATION_REQUEST, PLAIN_5GS_MSG);
  size += sizeof(plain.header);

  // Plain 5GMM msg
  registration_request_msg *mm_msg = &plain.mm_msg.registration_request;
  mm_msg->fgsregistrationtype = INITIAL_REGISTRATION;
  mm_msg->naskeysetidentifier.naskeysetidentifier = 1;
  size += 1;
  if (nas->guti) {
    size += fill_guti(&mm_msg->fgsmobileidentity, nas->guti);
  } else {
    size += fill_suci(&mm_msg->fgsmobileidentity, nas->uicc);
  }

#if 0
  /* This cannot be sent in clear, the core network Open5GS rejects the UE.
   * TODO: do we have to send this at some point?
   * For the time being, let's keep it here for later proper fix.
   */
  mm_msg->registration_request.presencemask |= REGISTRATION_REQUEST_5GMM_CAPABILITY_PRESENT;
  mm_msg->registration_request.fgmmcapability.iei = REGISTRATION_REQUEST_5GMM_CAPABILITY_IEI;
  mm_msg->registration_request.fgmmcapability.length = 1;
  mm_msg->registration_request.fgmmcapability.value = 0x7;
  size += 3;
#endif

  mm_msg->presencemask |= REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_PRESENT;
  mm_msg->nruesecuritycapability.iei = REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_IEI;
  mm_msg->nruesecuritycapability.length = 8;
  mm_msg->nruesecuritycapability.fg_EA = 0xe0;
  mm_msg->nruesecuritycapability.fg_IA = 0x60;
  mm_msg->nruesecuritycapability.EEA = 0;
  mm_msg->nruesecuritycapability.EIA = 0;
  size += 10;

  // encode the message
  initialNasMsg->nas_data = malloc16_clear(size * sizeof(*initialNasMsg->nas_data));
  nas->registration_request_buf = initialNasMsg->nas_data;

  initialNasMsg->length = mm_msg_encode(&plain, initialNasMsg->nas_data, size);
  nas->registration_request_len = initialNasMsg->length;
}

void generateServiceRequest(as_nas_info_t *initialNasMsg, nr_ue_nas_t *nas)
{
  LOG_I(NAS, "Generate initial NAS message: Service Request\n");
  int size = 0;

  // NAS is security protected if has valid security contexts
  bool security_protected = nas->security_container->ciphering_context && nas->security_container->integrity_context;

  // Set 5GMM plain header
  fgmm_nas_message_plain_t plain = {0};
  plain.header = set_mm_header(FGS_SERVICE_REQUEST, PLAIN_5GS_MSG);
  size += sizeof(plain.header);
  // Set plain FGMM Service Request
  fgs_service_request_msg_t *mm_msg = &plain.mm_msg.service_request;
  // Service Type
  mm_msg->serviceType = SERVICE_TYPE_DATA;
  // NAS key set identifier
  mm_msg->naskeysetidentifier.naskeysetidentifier = NAS_KEY_SET_IDENTIFIER_NOT_AVAILABLE;
  mm_msg->naskeysetidentifier.tsc = NAS_KEY_SET_IDENTIFIER_NATIVE;
  size += 1;
  // 5G-S-TMSI
  size += fill_fgstmsi(&mm_msg->fiveg_s_tmsi, nas->guti);

  if (security_protected) {
    fgmm_nas_msg_security_protected_t sp = {0};

    // Set security protected 5GS NAS message header (see 9.1.1 of 3GPP TS 24.501)
    sp.header.protocol_discriminator = FGS_MOBILITY_MANAGEMENT_MESSAGE;
    sp.header.security_header_type = INTEGRITY_PROTECTED;
    sp.header.sequence_number = nas->security.nas_count_ul & 0xff;
    size += sizeof(sp.header);

    // Payload: plain message
    sp.plain = plain;

    // Allocate NAS message for encoding
    initialNasMsg->nas_data = malloc_or_fail(size * sizeof(*initialNasMsg->nas_data));

    // security protected encoding
    int security_header_len = nas_protected_security_header_encode(initialNasMsg->nas_data, &sp.header, size);
    initialNasMsg->length =
        security_header_len
        + mm_msg_encode(&sp.plain, (uint8_t *)(initialNasMsg->nas_data + security_header_len), size - security_header_len);
    /* ciphering */
    uint8_t buf[initialNasMsg->length - 7];
    nas_stream_cipher_t stream_cipher;
    stream_cipher.context = nas->security_container->ciphering_context;
    AssertFatal(nas->security.nas_count_ul <= 0xffffff, "fatal: NAS COUNT UL too big (todo: fix that)\n");
    stream_cipher.count = nas->security.nas_count_ul;
    stream_cipher.bearer = 1;
    stream_cipher.direction = 0;
    stream_cipher.message = (unsigned char *)(initialNasMsg->nas_data + 7);
    /* length in bits */
    stream_cipher.blength = (initialNasMsg->length - 7) << 3;
    stream_compute_encrypt(nas->security_container->ciphering_algorithm, &stream_cipher, buf);
    memcpy(stream_cipher.message, buf, initialNasMsg->length - 7);
    /* integrity protection */
    uint8_t mac[4];
    stream_cipher.context = nas->security_container->integrity_context;
    stream_cipher.count = nas->security.nas_count_ul++;
    stream_cipher.bearer = 1;
    stream_cipher.direction = 0;
    stream_cipher.message = (unsigned char *)(initialNasMsg->nas_data + 6);
    /* length in bits */
    stream_cipher.blength = (initialNasMsg->length - 6) << 3;
    stream_compute_integrity(nas->security_container->integrity_algorithm, &stream_cipher, mac);
    LOG_D(NAS, "Integrity protected initial NAS message: mac = %x %x %x %x \n", mac[0], mac[1], mac[2], mac[3]);
    for (int i = 0; i < 4; i++)
      initialNasMsg->nas_data[2 + i] = mac[i];
  } else {
    // plain encoding
    // Allocate NAS message for encoding
    initialNasMsg->nas_data = malloc_or_fail(size * sizeof(*initialNasMsg->nas_data));
    initialNasMsg->length = mm_msg_encode(&plain, initialNasMsg->nas_data, size);
    LOG_I(NAS, "PLAIN_5GS_MSG initial NAS message: Service Request with length %d \n", initialNasMsg->length);
  }
}

void generateIdentityResponse(as_nas_info_t *initialNasMsg, uint8_t identitytype, uicc_t *uicc)
{
  int size = sizeof(fgmm_msg_header_t);
  fgmm_nas_message_plain_t plain = {0};

  // Plain 5GMM header
  plain.header = set_mm_header(FGS_IDENTITY_RESPONSE, PLAIN_5GS_MSG);
  size += sizeof(plain.header);

  // set identity response
  fgmm_identity_response_msg *mm_msg = &plain.mm_msg.fgs_identity_response;
  if (identitytype == FGS_MOBILE_IDENTITY_SUCI) {
    size += fill_suci(&mm_msg->fgsmobileidentity, uicc);
  }

  // encode the message
  initialNasMsg->nas_data = malloc_or_fail(size * sizeof(*initialNasMsg->nas_data));

  initialNasMsg->length = mm_msg_encode(&plain, initialNasMsg->nas_data, size);
}

static void generateAuthenticationResp(nr_ue_nas_t *nas, as_nas_info_t *initialNasMsg, uint8_t *buf)
{
  derive_ue_keys(buf, nas);
  OctetString res;
  res.length = 16;
  res.value = calloc(1, 16);
  memcpy(res.value, nas->security.res, 16);

  int size = sizeof(fgmm_msg_header_t);
  fgmm_nas_message_plain_t plain = {0};

  // Plain 5GMM header
  plain.header = set_mm_header(FGS_AUTHENTICATION_RESPONSE, PLAIN_5GS_MSG);
  size += sizeof(plain.header);

  // set response parameter
  fgs_authentication_response_msg *mm_msg = &plain.mm_msg.fgs_auth_response;
  mm_msg->authenticationresponseparameter.res = res;
  size += 18;
  // encode the message
  initialNasMsg->nas_data = malloc_or_fail(size * sizeof(*initialNasMsg->nas_data));

  initialNasMsg->length = mm_msg_encode(&plain, initialNasMsg->nas_data, size);
  // Free res value after encode
  free(res.value);
}

int nas_itti_kgnb_refresh_req(instance_t instance, const uint8_t kgnb[32])
{
  MessageDef *message_p;
  message_p = itti_alloc_new_message(TASK_NAS_NRUE, instance, NAS_KENB_REFRESH_REQ);
  memcpy(NAS_KENB_REFRESH_REQ(message_p).kenb, kgnb, sizeof(NAS_KENB_REFRESH_REQ(message_p).kenb));
  return itti_send_msg_to_task(TASK_RRC_NRUE, instance, message_p);
}

static void generateSecurityModeComplete(nr_ue_nas_t *nas, as_nas_info_t *initialNasMsg)
{
  int size = sizeof(fgmm_msg_header_t);
  fgmm_nas_msg_security_protected_t nas_msg = {0};
  nas_stream_cipher_t stream_cipher;
  uint8_t mac[NAS_INTEGRITY_SIZE];
  // set security protected header
  fgs_nas_message_security_header_t *sp = &nas_msg.header;
  sp->protocol_discriminator = FGS_MOBILITY_MANAGEMENT_MESSAGE;
  sp->security_header_type = INTEGRITY_PROTECTED_AND_CIPHERED_WITH_NEW_SECU_CTX;
  sp->sequence_number = nas->security.nas_count_ul & 0xff;
  size += 7;

  // Plain 5GMM Security Mode Complete msg
  fgmm_nas_message_plain_t *plain = &nas_msg.plain;

  // Plain 5GMM header
  plain->header = set_mm_header(FGS_SECURITY_MODE_COMPLETE, PLAIN_5GS_MSG);
  size += sizeof(plain->header);

  // Plain 5GMM payload
  fgs_security_mode_complete_msg *mm_msg = &plain->mm_msg.fgs_security_mode_complete;
  size += fill_imeisv(&mm_msg->fgsmobileidentity, nas->uicc);

  mm_msg->fgsnasmessagecontainer.nasmessagecontainercontents.value = nas->registration_request_buf;
  mm_msg->fgsnasmessagecontainer.nasmessagecontainercontents.length = nas->registration_request_len;
  size += (nas->registration_request_len + 2);

  // encode the message
  initialNasMsg->nas_data = malloc_or_fail(size * sizeof(*initialNasMsg->nas_data));

  int security_header_len = nas_protected_security_header_encode(initialNasMsg->nas_data, sp, size);

  initialNasMsg->length =
      security_header_len
      + mm_msg_encode(plain, (uint8_t *)(initialNasMsg->nas_data + security_header_len), size - security_header_len);

  /* ciphering */
  uint8_t buf[initialNasMsg->length - 7];
  stream_cipher.context = nas->security_container->ciphering_context;
  AssertFatal(nas->security.nas_count_ul <= 0xffffff, "fatal: NAS COUNT UL too big (todo: fix that)\n");
  stream_cipher.count = nas->security.nas_count_ul;
  stream_cipher.bearer = 1;
  stream_cipher.direction = 0;
  stream_cipher.message = (unsigned char *)(initialNasMsg->nas_data + 7);
  /* length in bits */
  stream_cipher.blength = (initialNasMsg->length - 7) << 3;
  stream_compute_encrypt(nas->security_container->ciphering_algorithm, &stream_cipher, buf);
  memcpy(stream_cipher.message, buf, initialNasMsg->length - 7);

  /* integrity protection */
  stream_cipher.context = nas->security_container->integrity_context;
  stream_cipher.count = nas->security.nas_count_ul++;
  stream_cipher.bearer = 1;
  stream_cipher.direction = 0;
  stream_cipher.message = (unsigned char *)(initialNasMsg->nas_data + 6);
  /* length in bits */
  stream_cipher.blength = (initialNasMsg->length - 6) << 3;

  stream_compute_integrity(nas->security_container->integrity_algorithm, &stream_cipher, mac);

  printf("mac %x %x %x %x \n", mac[0], mac[1], mac[2], mac[3]);
  for (int i = 0; i < 4; i++) {
    initialNasMsg->nas_data[2 + i] = mac[i];
  }
}

static void handle_security_mode_command(nr_ue_nas_t *nas, as_nas_info_t *initialNasMsg, uint8_t *pdu, int pdu_length)
{
  /* retrieve integrity and ciphering algorithms  */
  AssertFatal(pdu_length > 10, "nas: bad pdu\n");
  int ciphering_algorithm = (pdu[10] >> 4) & 0x0f;
  int integrity_algorithm = pdu[10] & 0x0f;

  uint8_t *kamf = nas->security.kamf;
  uint8_t *knas_enc = nas->security.knas_enc;
  uint8_t *knas_int = nas->security.knas_int;

  /* derive keys */
  derive_knas(0x01, ciphering_algorithm, kamf, knas_enc);
  derive_knas(0x02, integrity_algorithm, kamf, knas_int);

  printf("knas_int: ");
  for (int i = 0; i < 16; i++) {
    printf("%x ", knas_int[i]);
  }
  printf("\n");

  printf("knas_enc: ");
  for (int i = 0; i < 16; i++) {
    printf("%x ", knas_enc[i]);
  }
  printf("\n");

  /* todo: stream_security_container_delete() is not called anywhere, deal with that */
  nas->security_container = stream_security_container_init(ciphering_algorithm, integrity_algorithm, knas_enc, knas_int);

  nas_itti_kgnb_refresh_req(nas->UE_id, nas->security.kgnb);
  generateSecurityModeComplete(nas, initialNasMsg);
}

static void generateRegistrationComplete(nr_ue_nas_t *nas,
                                         as_nas_info_t *initialNasMsg,
                                         SORTransparentContainer *sortransparentcontainer)
{
  int length = 0;
  nas_stream_cipher_t stream_cipher;
  uint8_t mac[NAS_INTEGRITY_SIZE];
  fgmm_nas_msg_security_protected_t sp = {0};

  // set security protected header
  sp.header.protocol_discriminator = FGS_MOBILITY_MANAGEMENT_MESSAGE;
  sp.header.security_header_type = INTEGRITY_PROTECTED_AND_CIPHERED;
  sp.header.message_authentication_code = 0;
  sp.header.sequence_number = nas->security.nas_count_ul & 0xff;
  length = 7;
  // set plain 5GMM header
  sp.plain.header = set_mm_header(FGS_REGISTRATION_COMPLETE, PLAIN_5GS_MSG);
  length += sizeof(sp.plain.header);

  registration_complete_msg *mm_msg = &sp.plain.mm_msg.registration_complete;
  if (sortransparentcontainer) {
    mm_msg->sortransparentcontainer = sortransparentcontainer;
    length += sortransparentcontainer->sortransparentcontainercontents.length;
  }

  // encode the message
  initialNasMsg->nas_data = malloc_or_fail(length * sizeof(*initialNasMsg->nas_data));
  initialNasMsg->length = length;
  // encode security protected header
  int encoded = nas_protected_security_header_encode(initialNasMsg->nas_data, &sp.header, length);
  if (encoded < 0) {
    LOG_E(NAS, "generateRegistrationComplete: failed to encode security protected header\n");
    return;
  }
  // encode 5GMM plain header
  encoded = _nas_mm_msg_encode_header(&sp.plain.header, initialNasMsg->nas_data + encoded, length - encoded);
  if (encoded < 0) {
    LOG_E(NAS, "generateRegistrationComplete: failed to encode 5GMM plain header\n");
    return;
  }

  encode_registration_complete(mm_msg, initialNasMsg->nas_data + encoded, length - encoded);

  /* ciphering */
  uint8_t buf[initialNasMsg->length - 7];
  stream_cipher.context = nas->security_container->ciphering_context;
  AssertFatal(nas->security.nas_count_ul <= 0xffffff, "fatal: NAS COUNT UL too big (todo: fix that)\n");
  stream_cipher.count = nas->security.nas_count_ul;
  stream_cipher.bearer = 1;
  stream_cipher.direction = 0;
  stream_cipher.message = (unsigned char *)(initialNasMsg->nas_data + 7);
  /* length in bits */
  stream_cipher.blength = (initialNasMsg->length - 7) << 3;
  stream_compute_encrypt(nas->security_container->ciphering_algorithm, &stream_cipher, buf);
  memcpy(stream_cipher.message, buf, initialNasMsg->length - 7);

  /* integrity protection */
  stream_cipher.context = nas->security_container->integrity_context;
  stream_cipher.count = nas->security.nas_count_ul++;
  stream_cipher.bearer = 1;
  stream_cipher.direction = 0;
  stream_cipher.message = (unsigned char *)(initialNasMsg->nas_data + 6);
  /* length in bits */
  stream_cipher.blength = (initialNasMsg->length - 6) << 3;
  stream_compute_integrity(nas->security_container->integrity_algorithm, &stream_cipher, mac);

  printf("mac %x %x %x %x \n", mac[0], mac[1], mac[2], mac[3]);
  for (int i = 0; i < 4; i++) {
    initialNasMsg->nas_data[2 + i] = mac[i];
  }
}

/**
 * @brief Capture IPv4 PDU Session Address
 */
static int capture_ipv4_addr(const uint8_t *addr, char *ip, size_t len)
{
  return snprintf(ip, len, "%d.%d.%d.%d", addr[0], addr[1], addr[2], addr[3]);
}

/**
 * @brief Capture IPv6 PDU Session Address
 */
static int capture_ipv6_addr(const uint8_t *addr, char *ip, size_t len)
{
  // 24.501 Sec 9.11.4.10: "an interface identifier for the IPv6 link local
  // address": link local starts with fe80::, and only the last 64bits are
  // given (middle is zero)
  return snprintf(ip,
                  len,
                  "fe80::%02x%02x:%02x%02x:%02x%02x:%02x%02x",
                  addr[0],
                  addr[1],
                  addr[2],
                  addr[3],
                  addr[4],
                  addr[5],
                  addr[6],
                  addr[7]);
}

/**
 * @brief Process PDU Session Address in PDU Session Establishment Accept message
 *        and configure the tun interface
 */
static void process_pdu_session_addr(pdu_session_establishment_accept_msg_t *msg)
{
  uint8_t *addr = msg->pdu_addr_ie.pdu_addr_oct;

  switch (msg->pdu_addr_ie.pdu_type) {
    case PDU_SESSION_TYPE_IPV4: {
      char ip[20];
      capture_ipv4_addr(&addr[0], ip, sizeof(ip));
      tun_config(1, ip, NULL, "oaitun_ue");
      setup_ue_ipv4_route(1, ip, "oaitun_ue");
    } break;

    case PDU_SESSION_TYPE_IPV6: {
      char ipv6[40];
      capture_ipv6_addr(addr, ipv6, sizeof(ipv6));
      tun_config(1, NULL, ipv6, "oaitun_ue");
    } break;

    case PDU_SESSION_TYPE_IPV4V6: {
      char ipv6[40];
      capture_ipv6_addr(addr, ipv6, sizeof(ipv6));
      char ipv4[20];
      capture_ipv4_addr(&addr[IPv6_INTERFACE_ID_LENGTH], ipv4, sizeof(ipv4));
      tun_config(1, ipv4, ipv6, "oaitun_ue");
      setup_ue_ipv4_route(1, ipv4, "oaitun_ue");
    } break;

    default:
      LOG_E(NAS, "Unknown PDU Session Address type %d\n", msg->pdu_addr_ie.pdu_type);
      break;
  }
}

/**
 * @brief Handle PDU Session Establishment Accept and process decoded message
 */
static void handle_pdu_session_accept(uint8_t *pdu_buffer, uint32_t msg_length)
{
  pdu_session_establishment_accept_msg_t msg = {0};
  int size = 0;
  int decoded = 0;

  // Security protected NAS header (7 bytes)
  fgs_nas_message_security_header_t sec_nas_hdr = {0};
  if ((decoded = decode_5gs_security_protected_header(&sec_nas_hdr, pdu_buffer, msg_length)) < 0) {
    LOG_E(NAS, "decode_5gs_security_protected_header failure in PDU Session Establishment Accept decoding\n");
    return;
  }
  size += decoded;

  // decode plain 5GMM message header
  fgmm_msg_header_t mm_header = {0};
  if ((decoded = decode_5gmm_msg_header(&mm_header, pdu_buffer + size, msg_length - size)) < 0) {
    LOG_E(NAS, "decode_5gmm_msg_header failure in PDU Session Establishment Accept decoding\n");
    return;
  }
  size += decoded;

  /* Process container (5GSM message) */
  // Payload container type and spare (1 octet)
  size++;
  // Payload container length
  uint16_t iei_len = 0;
  GET_SHORT(pdu_buffer + size, iei_len);
  size += sizeof(iei_len);
  // decode plain 5GSM message header
  fgsm_msg_header_t sm_header = {0};
  if ((decoded = decode_5gsm_msg_header(&sm_header, pdu_buffer + size, msg_length - size)) < 0) {
    LOG_E(NAS, "decode_5gsm_msg_header failure in PDU Session Establishment Accept decoding\n");
    return;
  }
  size += decoded;

  // decode PDU Session Establishment Accept
  if (!decode_pdu_session_establishment_accept_msg(&msg, pdu_buffer + size, msg_length))
    LOG_E(NAS, "decode_pdu_session_establishment_accept_msg failure\n");

  // process PDU Session
  process_pdu_session_addr(&msg);
  set_qfi_pduid(msg.qos_rules.rule->qfi, sm_header.pdu_session_id);
}

/**
 * @brief Handle DL NAS Transport and process piggybacked 5GSM messages
 */
static void handleDownlinkNASTransport(uint8_t *pdu_buffer, uint32_t msg_length)
{
  uint8_t msg_type = *(pdu_buffer + 16);
  if (msg_type == FGS_PDU_SESSION_ESTABLISHMENT_ACC) {
    LOG_A(NAS, "Received PDU Session Establishment Accept in DL NAS Transport\n");
    handle_pdu_session_accept(pdu_buffer, msg_length);
  } else {
    LOG_E(NAS, "Received unexpected message in DLinformationTransfer %d\n", msg_type);
  }
}

static void generateDeregistrationRequest(nr_ue_nas_t *nas, as_nas_info_t *initialNasMsg, const nas_deregistration_req_t *req)
{
  fgmm_nas_msg_security_protected_t sp_msg = {0};
  fgs_nas_message_security_header_t *sp_header = &sp_msg.header;
  sp_header->protocol_discriminator = FGS_MOBILITY_MANAGEMENT_MESSAGE;
  sp_header->security_header_type = INTEGRITY_PROTECTED_AND_CIPHERED;
  sp_header->message_authentication_code = 0;
  sp_header->sequence_number = nas->security.nas_count_ul & 0xff;
  int size = sizeof(sp_msg.header);

  // Plain 5GMM header
  sp_msg.plain.header = set_mm_header(FGS_DEREGISTRATION_REQUEST_UE_ORIGINATING, INTEGRITY_PROTECTED_AND_CIPHERED_WITH_NEW_SECU_CTX);
  size += sizeof(sp_msg.plain.header);

  // Plain 5GMM
  fgs_deregistration_request_ue_originating_msg *mm_msg = &sp_msg.plain.mm_msg.fgs_deregistration_request_ue_originating;
  mm_msg->deregistrationtype.switchoff = NORMAL_DEREGISTRATION;
  mm_msg->deregistrationtype.reregistration_required = REREGISTRATION_NOT_REQUIRED;
  mm_msg->deregistrationtype.access_type = TGPP_ACCESS;
  mm_msg->naskeysetidentifier.naskeysetidentifier = 1;
  size += 1;
  size += fill_guti(&mm_msg->fgsmobileidentity, nas->guti);

  // encode the message
  initialNasMsg->nas_data = calloc_or_fail(size, sizeof(*initialNasMsg->nas_data));
  int security_header_len = nas_protected_security_header_encode(initialNasMsg->nas_data, sp_header, size);

  initialNasMsg->length =
      security_header_len
      + mm_msg_encode(&sp_msg.plain, (uint8_t *)(initialNasMsg->nas_data + security_header_len), size - security_header_len);

  nas_stream_cipher_t stream_cipher;

  /* ciphering */
  uint8_t buf[initialNasMsg->length - 7];
  stream_cipher.context = nas->security_container->ciphering_context;
  AssertFatal(nas->security.nas_count_ul <= 0xffffff, "fatal: NAS COUNT UL too big (todo: fix that)\n");
  stream_cipher.count = nas->security.nas_count_ul;
  stream_cipher.bearer = 1;
  stream_cipher.direction = 0;
  stream_cipher.message = (unsigned char *)(initialNasMsg->nas_data + 7);
  /* length in bits */
  stream_cipher.blength = (initialNasMsg->length - 7) << 3;
  stream_compute_encrypt(nas->security_container->ciphering_algorithm, &stream_cipher, buf);
  memcpy(stream_cipher.message, buf, initialNasMsg->length - 7);

  /* integrity protection */
  stream_cipher.context = nas->security_container->integrity_context;
  stream_cipher.count = nas->security.nas_count_ul++;
  stream_cipher.bearer = 1;
  stream_cipher.direction = 0;
  stream_cipher.message = (unsigned char *)(initialNasMsg->nas_data + 6);
  /* length in bits */
  stream_cipher.blength = (initialNasMsg->length - 6) << 3;
  uint8_t mac[NAS_INTEGRITY_SIZE];
  stream_compute_integrity(nas->security_container->integrity_algorithm, &stream_cipher, mac);

  printf("mac %x %x %x %x \n", mac[0], mac[1], mac[2], mac[3]);
  for (int i = 0; i < 4; i++) {
    initialNasMsg->nas_data[2 + i] = mac[i];
  }
}

static void generatePduSessionEstablishRequest(nr_ue_nas_t *nas, as_nas_info_t *initialNasMsg, nas_pdu_session_req_t *pdu_req)
{
  int size = 0;

  // setup pdu session establishment request
  uint16_t req_length = 7;
  uint8_t *req_buffer = malloc(req_length);
  pdu_session_establishment_request_msg pdu_session_establish;
  pdu_session_establish.protocoldiscriminator = FGS_SESSION_MANAGEMENT_MESSAGE;
  pdu_session_establish.pdusessionid = pdu_req->pdusession_id;
  pdu_session_establish.pti = 1;
  pdu_session_establish.pdusessionestblishmsgtype = FGS_PDU_SESSION_ESTABLISHMENT_REQ;
  pdu_session_establish.maxdatarate = 0xffff;
  pdu_session_establish.pdusessiontype = pdu_req->pdusession_type;
  encode_pdu_session_establishment_request(&pdu_session_establish, req_buffer);

  nas_stream_cipher_t stream_cipher;
  uint8_t mac[NAS_INTEGRITY_SIZE];

  // 5GMM security protected message
  fgmm_nas_msg_security_protected_t sp_msg = {0};
  // 5GMM security protected message header
  fgs_nas_message_security_header_t *sp_header = &sp_msg.header;
  sp_header->protocol_discriminator = FGS_MOBILITY_MANAGEMENT_MESSAGE;
  sp_header->security_header_type = INTEGRITY_PROTECTED_AND_CIPHERED;
  sp_header->sequence_number = nas->security.nas_count_ul & 0xff;

  size += 7;

  fgmm_nas_message_plain_t *plain = &sp_msg.plain;

  // Plain 5GMM header
  plain->header = set_mm_header(FGS_UPLINK_NAS_TRANSPORT, PLAIN_5GS_MSG);
  size += sizeof(plain->header);

  fgs_uplink_nas_transport_msg *mm_msg = &plain->mm_msg.uplink_nas_transport;
  mm_msg->payloadcontainertype.iei = 0;
  mm_msg->payloadcontainertype.type = 1;
  size += 1;
  mm_msg->fgspayloadcontainer.payloadcontainercontents.length = req_length;
  mm_msg->fgspayloadcontainer.payloadcontainercontents.value = req_buffer;
  size += (2 + req_length);
  mm_msg->pdusessionid = pdu_req->pdusession_id;
  mm_msg->requesttype = 1;
  size += 3;
  const bool has_nssai_sd = pdu_req->sd != 0xffffff; // 0xffffff means "no SD", TS 23.003
  const size_t nssai_len = has_nssai_sd ? 4 : 1;
  mm_msg->snssai.length = nssai_len;
  // Fixme: it seems there are a lot of memory errors in this: this value was on the stack,
  //  but pushed  in a itti message to another thread
  //  this kind of error seems in many places in 5G NAS
  mm_msg->snssai.value = calloc(1, nssai_len);
  mm_msg->snssai.value[0] = pdu_req->sst;
  if (has_nssai_sd)
    INT24_TO_BUFFER(pdu_req->sd, &mm_msg->snssai.value[1]);
  size += 1 + 1 + nssai_len;
  int dnnSize = strlen(nas->uicc->dnnStr);
  mm_msg->dnn.value = calloc(1, dnnSize + 1);
  mm_msg->dnn.length = dnnSize + 1;
  mm_msg->dnn.value[0] = dnnSize;
  memcpy(mm_msg->dnn.value + 1, nas->uicc->dnnStr, dnnSize);
  size += (1 + 1 + dnnSize + 1);

  // encode the message
  initialNasMsg->nas_data = malloc_or_fail(size * sizeof(*initialNasMsg->nas_data));
  int security_header_len = nas_protected_security_header_encode(initialNasMsg->nas_data, sp_header, size);

  initialNasMsg->length =
      security_header_len
      + mm_msg_encode(plain, (uint8_t *)(initialNasMsg->nas_data + security_header_len), size - security_header_len);

  // Free allocated memory after encode
  free(req_buffer);
  free(mm_msg->dnn.value);
  free(mm_msg->snssai.value);

  /* ciphering */
  uint8_t buf[initialNasMsg->length - 7];
  stream_cipher.context = nas->security_container->ciphering_context;
  AssertFatal(nas->security.nas_count_ul <= 0xffffff, "fatal: NAS COUNT UL too big (todo: fix that)\n");
  stream_cipher.count = nas->security.nas_count_ul;
  stream_cipher.bearer = 1;
  stream_cipher.direction = 0;
  stream_cipher.message = (unsigned char *)(initialNasMsg->nas_data + 7);
  /* length in bits */
  stream_cipher.blength = (initialNasMsg->length - 7) << 3;
  stream_compute_encrypt(nas->security_container->ciphering_algorithm, &stream_cipher, buf);
  memcpy(stream_cipher.message, buf, initialNasMsg->length - 7);

  /* integrity protection */
  stream_cipher.context = nas->security_container->integrity_context;
  stream_cipher.count = nas->security.nas_count_ul++;
  stream_cipher.bearer = 1;
  stream_cipher.direction = 0;
  stream_cipher.message = (unsigned char *)(initialNasMsg->nas_data + 6);
  /* length in bits */
  stream_cipher.blength = (initialNasMsg->length - 6) << 3;
  stream_compute_integrity(nas->security_container->integrity_algorithm, &stream_cipher, mac);

  printf("mac %x %x %x %x \n", mac[0], mac[1], mac[2], mac[3]);
  for (int i = 0; i < 4; i++) {
    initialNasMsg->nas_data[2 + i] = mac[i];
  }
}

static uint8_t get_msg_type(uint8_t *pdu_buffer, uint32_t length)
{
  if (pdu_buffer == NULL)
    goto error;

  /* get security header type */
  if (length < 2)
    goto error;

  int security_header_type = pdu_buffer[1];

  if (security_header_type == 0) {
    /* plain NAS message */
    if (length < 3)
      goto error;
    return pdu_buffer[2];
  }

  if (length < 10)
    goto error;

  int msg_type = pdu_buffer[9];

  if (msg_type == FGS_DOWNLINK_NAS_TRANSPORT) {
    if (length < 17)
      goto error;

    msg_type = pdu_buffer[16];
  }

  return msg_type;

error:
  LOG_E(NAS, "[UE] Received invalid downlink message\n");
  return 0;
}

static void send_nas_uplink_data_req(nr_ue_nas_t *nas, const as_nas_info_t *initial_nas_msg)
{
  MessageDef *msg = itti_alloc_new_message(TASK_NAS_NRUE, nas->UE_id, NAS_UPLINK_DATA_REQ);
  ul_info_transfer_req_t *req = &NAS_UPLINK_DATA_REQ(msg);
  req->UEid = nas->UE_id;
  req->nasMsg.nas_data = (uint8_t *)initial_nas_msg->nas_data;
  req->nasMsg.length = initial_nas_msg->length;
  itti_send_msg_to_task(TASK_RRC_NRUE, nas->UE_id, msg);
}

static void send_nas_detach_req(nr_ue_nas_t *nas, bool wait_release)
{
  MessageDef *msg = itti_alloc_new_message(TASK_NAS_NRUE, nas->UE_id, NAS_DETACH_REQ);
  nas_detach_req_t *req = &NAS_DETACH_REQ(msg);
  req->wait_release = wait_release;
  itti_send_msg_to_task(TASK_RRC_NRUE, nas->UE_id, msg);
}

static void request_default_pdusession(nr_ue_nas_t *nas)
{
  MessageDef *message_p = itti_alloc_new_message(TASK_NAS_NRUE, nas->UE_id, NAS_PDU_SESSION_REQ);
  NAS_PDU_SESSION_REQ(message_p).pdusession_id = 10; /* first or default pdu session */
  NAS_PDU_SESSION_REQ(message_p).pdusession_type = 0x91; // 0x91 = IPv4, 0x92 = IPv6, 0x93 = IPv4v6
  NAS_PDU_SESSION_REQ(message_p).sst = nas->uicc->nssai_sst;
  NAS_PDU_SESSION_REQ(message_p).sd = nas->uicc->nssai_sd;
  itti_send_msg_to_task(TASK_NAS_NRUE, nas->UE_id, message_p);
}

static int get_user_nssai_idx(const nr_nas_msg_snssai_t allowed_nssai[8], const nr_ue_nas_t *nas)
{
  for (int i = 0; i < 8; i++) {
    const nr_nas_msg_snssai_t *nssai = allowed_nssai + i;
    if ((nas->uicc->nssai_sst == nssai->sst) && (nas->uicc->nssai_sd == nssai->sd))
      return i;
  }
  return -1;
}

void *nas_nrue_task(void *args_p)
{
  while (1) {
    nas_nrue(NULL);
  }
}

static void process_guti(Guti5GSMobileIdentity_t *guti, nr_ue_nas_t *nas)
{
  AssertFatal(guti->typeofidentity == FGS_MOBILE_IDENTITY_5G_GUTI,
              "registration accept 5GS Mobile Identity is not GUTI, but %d\n",
              guti->typeofidentity);
  nas->guti = malloc_or_fail(sizeof(*nas->guti));
  *nas->guti = *guti;
}

static void handle_registration_accept(nr_ue_nas_t *nas, const uint8_t *pdu_buffer, uint32_t msg_length)
{
  LOG_I(NAS, "[UE] Received REGISTRATION ACCEPT message\n");
  registration_accept_msg msg = {0};
  fgs_nas_message_security_header_t sp_header = {0};
  const uint8_t *end = pdu_buffer + msg_length;
  // security protected header
  int decoded = decode_5gs_security_protected_header(&sp_header, pdu_buffer, msg_length);
  if (!decoded) {
    LOG_E(NAS, "NAS Registration Accept: failed to decode security protected header\n");
    return;
  }
  pdu_buffer += decoded;
  // plain header
  fgmm_msg_header_t mm_header = {0};
  if ((decoded = decode_5gmm_msg_header(&mm_header, pdu_buffer, end - pdu_buffer)) < 0) {
    LOG_E(NAS, "Failed to decode NAS Registration Accept\n");
    return;
  }
  if (mm_header.message_type != FGS_REGISTRATION_ACCEPT) {
    LOG_E(NAS, "Failed to process NAS Registration Accept: wrong message type\n");
    return;
  }
  pdu_buffer += decoded;
  // plain payload
  if ((decoded = decode_registration_accept(&msg, pdu_buffer, end - pdu_buffer)) < 0) {
    LOG_E(NAS, "Failed to decode NAS Registration Accept\n");
    return;
  }
  pdu_buffer += decoded;
  // process GUTI
  if (msg.guti) {
    process_guti(&msg.guti->guti, nas);
    free(msg.guti);
  } else {
    LOG_W(NAS, "no GUTI in registration accept\n");
  }

  as_nas_info_t initialNasMsg = {0};
  generateRegistrationComplete(nas, &initialNasMsg, NULL);
  if (initialNasMsg.length > 0) {
    send_nas_uplink_data_req(nas, &initialNasMsg);
    LOG_I(NAS, "Send NAS_UPLINK_DATA_REQ message(RegistrationComplete)\n");
  }
  if (get_user_nssai_idx(msg.nas_allowed_nssai, nas) < 0) {
    LOG_E(NAS, "NSSAI parameters not match with allowed NSSAI. Couldn't request PDU session.\n");
  } else {
    request_default_pdusession(nas);
  }
}

void *nas_nrue(void *args_p)
{
  // Wait for a message or an event
  MessageDef *msg_p;
  itti_receive_msg(TASK_NAS_NRUE, &msg_p);

  if (msg_p != NULL) {
    nr_ue_nas_t *nas = get_ue_nas_info(msg_p->ittiMsgHeader.destinationInstance);

    switch (ITTI_MSG_ID(msg_p)) {
      case INITIALIZE_MESSAGE:

        break;

      case TERMINATE_MESSAGE:
        itti_exit_task();
        break;

      case MESSAGE_TEST:
        break;

      case NAS_CELL_SELECTION_CNF:
        LOG_I(NAS,
              "[UE %ld] Received %s: errCode %u, cellID %u, tac %u\n",
              nas->UE_id,
              ITTI_MSG_NAME(msg_p),
              NAS_CELL_SELECTION_CNF(msg_p).errCode,
              NAS_CELL_SELECTION_CNF(msg_p).cellID,
              NAS_CELL_SELECTION_CNF(msg_p).tac);
        // as_stmsi_t s_tmsi={0, 0};
        // as_nas_info_t nas_info;
        // plmn_t plmnID={0, 0, 0, 0};
        // generateRegistrationRequest(&nas_info);
        // nr_nas_itti_nas_establish_req(0, AS_TYPE_ORIGINATING_SIGNAL, s_tmsi, plmnID, nas_info.data, nas_info.length, 0);
        break;

      case NAS_CELL_SELECTION_IND:
        LOG_I(NAS,
              "[UE %ld] Received %s: cellID %u, tac %u\n",
              nas->UE_id,
              ITTI_MSG_NAME(msg_p),
              NAS_CELL_SELECTION_IND(msg_p).cellID,
              NAS_CELL_SELECTION_IND(msg_p).tac);

        /* TODO not processed by NAS currently */
        break;

      case NAS_PAGING_IND:
        LOG_I(NAS, "[UE %ld] Received %s: cause %u\n", nas->UE_id, ITTI_MSG_NAME(msg_p), NAS_PAGING_IND(msg_p).cause);

        /* TODO not processed by NAS currently */
        break;

      case NAS_PDU_SESSION_REQ: {
        as_nas_info_t pduEstablishMsg = {0};
        nas_pdu_session_req_t *pduReq = &NAS_PDU_SESSION_REQ(msg_p);
        generatePduSessionEstablishRequest(nas, &pduEstablishMsg, pduReq);
        if (pduEstablishMsg.length > 0) {
          send_nas_uplink_data_req(nas, &pduEstablishMsg);
          LOG_I(NAS, "Send NAS_UPLINK_DATA_REQ message(PduSessionEstablishRequest)\n");
        }
        break;
      }

      case NAS_CONN_ESTABLI_CNF: {
        LOG_I(NAS,
              "[UE %ld] Received %s: errCode %u, length %u\n",
              nas->UE_id,
              ITTI_MSG_NAME(msg_p),
              NAS_CONN_ESTABLI_CNF(msg_p).errCode,
              NAS_CONN_ESTABLI_CNF(msg_p).nasMsg.length);

        uint8_t *pdu_buffer = NAS_CONN_ESTABLI_CNF(msg_p).nasMsg.nas_data;
        int pdu_length = NAS_CONN_ESTABLI_CNF(msg_p).nasMsg.length;

        security_state_t security_state = nas_security_rx_process(nas, pdu_buffer, pdu_length);
        if (security_state != NAS_SECURITY_INTEGRITY_PASSED && security_state != NAS_SECURITY_NO_SECURITY_CONTEXT) {
          LOG_E(NAS, "NAS integrity failed, discard incoming message: security state is %s\n", security_state_info[security_state].text);
          break;
        }

        int msg_type = get_msg_type(pdu_buffer, pdu_length);

        if (msg_type == FGS_REGISTRATION_ACCEPT) {
          handle_registration_accept(nas, pdu_buffer, pdu_length);
        } else if (msg_type == FGS_PDU_SESSION_ESTABLISHMENT_ACC) {
          handle_pdu_session_accept(pdu_buffer, pdu_length);
        }

        // Free NAS buffer memory after use (coming from RRC)
        free(pdu_buffer);
        break;
      }

      case NR_NAS_CONN_RELEASE_IND:
        LOG_I(NAS, "[UE %ld] Received %s: cause %u\n", nas->UE_id, ITTI_MSG_NAME(msg_p), NR_NAS_CONN_RELEASE_IND(msg_p).cause);
        // TODO handle connection release
        if (nas->termination_procedure) {
          /* the following is not clean, but probably necessary: we need to give
           * time to RLC to Ack the SRB1 PDU which contained the RRC release
           * message. Hence, we just below wait some time, before finally
           * unblocking the nr-uesoftmodem, which will terminate the process. */
          usleep(100000);
          itti_wait_tasks_unblock(); /* will unblock ITTI to stop nr-uesoftmodem */
        }
        break;

      case NAS_UPLINK_DATA_CNF:
        LOG_I(NAS,
              "[UE %ld] Received %s: UEid %u, errCode %u\n",
              nas->UE_id,
              ITTI_MSG_NAME(msg_p),
              NAS_UPLINK_DATA_CNF(msg_p).UEid,
              NAS_UPLINK_DATA_CNF(msg_p).errCode);

        break;

      case NAS_DEREGISTRATION_REQ: {
        LOG_I(NAS, "[UE %ld] Received %s\n", nas->UE_id, ITTI_MSG_NAME(msg_p));
        nas_deregistration_req_t *req = &NAS_DEREGISTRATION_REQ(msg_p);
        if (nas->guti) {
          if (req->cause == AS_DETACH) {
            nas->termination_procedure = true;
            send_nas_detach_req(nas, true);
          }
          as_nas_info_t initialNasMsg = {0};
          generateDeregistrationRequest(nas, &initialNasMsg, req);
          send_nas_uplink_data_req(nas, &initialNasMsg);
        } else {
          LOG_W(NAS, "No GUTI, cannot trigger deregistration request.\n");
          if (req->cause == AS_DETACH)
            send_nas_detach_req(nas, false);
        }
      } break;

      case NAS_DOWNLINK_DATA_IND: {
        LOG_I(NAS,
              "[UE %ld] Received %s: length %u , buffer %p\n",
              nas->UE_id,
              ITTI_MSG_NAME(msg_p),
              NAS_DOWNLINK_DATA_IND(msg_p).nasMsg.length,
              NAS_DOWNLINK_DATA_IND(msg_p).nasMsg.nas_data);
        as_nas_info_t initialNasMsg = {0};

        uint8_t *pdu_buffer = NAS_DOWNLINK_DATA_IND(msg_p).nasMsg.nas_data;
        int pdu_length = NAS_DOWNLINK_DATA_IND(msg_p).nasMsg.length;
        int msg_type = get_msg_type(pdu_buffer, pdu_length);

        security_state_t security_state = nas_security_rx_process(nas, pdu_buffer, pdu_length);
        /* special cases accepted without protection */
        if (security_state == NAS_SECURITY_UNPROTECTED) {
          /* for the moment, only FGS_DEREGISTRATION_ACCEPT_UE_ORIGINATING is accepted */
          if (msg_type == FGS_DEREGISTRATION_ACCEPT_UE_ORIGINATING)
            security_state = NAS_SECURITY_INTEGRITY_PASSED;
        }

        if (security_state != NAS_SECURITY_INTEGRITY_PASSED && security_state != NAS_SECURITY_NO_SECURITY_CONTEXT) {
          LOG_E(NAS, "NAS integrity failed, discard incoming message\n");
          break;
        }

        switch (msg_type) {
          case FGS_IDENTITY_REQUEST:
            generateIdentityResponse(&initialNasMsg, *(pdu_buffer + 3), nas->uicc);
            break;
          case FGS_AUTHENTICATION_REQUEST:
            generateAuthenticationResp(nas, &initialNasMsg, pdu_buffer);
            break;
          case FGS_SECURITY_MODE_COMMAND:
            handle_security_mode_command(nas, &initialNasMsg, pdu_buffer, pdu_length);
            break;
          case FGS_DOWNLINK_NAS_TRANSPORT:
            handleDownlinkNASTransport(pdu_buffer, pdu_length);
            break;
          case FGS_REGISTRATION_ACCEPT:
            handle_registration_accept(nas, pdu_buffer, pdu_length);
            break;
          case FGS_DEREGISTRATION_ACCEPT_UE_ORIGINATING:
            LOG_I(NAS, "received deregistration accept\n");
            break;
          case FGS_PDU_SESSION_ESTABLISHMENT_ACC:
            handle_pdu_session_accept(pdu_buffer, pdu_length);
            break;
          case FGS_PDU_SESSION_ESTABLISHMENT_REJ:
            LOG_E(NAS, "Received PDU Session Establishment reject\n");
            break;
          case FGS_REGISTRATION_REJECT:
            LOG_E(NAS, "Received Registration reject cause: %s\n", cause_text_info[pdu_buffer[17]].text);
            exit(1);
            break;
          default:
            LOG_W(NR_RRC, "unknown message type %d\n", msg_type);
            break;
        }
        // Free NAS buffer memory after use (coming from RRC)
        free(pdu_buffer);

        if (initialNasMsg.length > 0)
          send_nas_uplink_data_req(nas, &initialNasMsg);
      } break;

      default:
        LOG_E(NAS, "[UE %ld] Received unexpected message %s\n", nas->UE_id, ITTI_MSG_NAME(msg_p));
        break;
    }

    int result = itti_free(ITTI_MSG_ORIGIN_ID(msg_p), msg_p);
    AssertFatal(result == EXIT_SUCCESS, "Failed to free memory (%d)!\n", result);
  }
  return NULL;
}

void nas_init_nrue(int num_ues) {
  for (int i = 0; i < num_ues; i++) {
    (void)get_ue_nas_info(i);
  }
}
