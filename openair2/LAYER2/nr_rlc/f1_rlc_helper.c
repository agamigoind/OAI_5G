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

#include "NR_MAC_gNB/nr_mac_gNB.h"
#include "NR_MAC_gNB/mac_proto.h"
#include "f1_rlc_helper.h"

void send_initial_ul_rrc_message(int rnti, const uint8_t *sdu, sdu_size_t sdu_len)
{
  gNB_MAC_INST *mac = RC.nrmac[0];
  NR_UE_info_t *UE = find_nr_UE(&mac->UE_info, rnti);
  NR_SCHED_ENSURE_LOCKED(&mac->sched_lock);

  uint8_t du2cu[1024];
  int encoded = encode_cellGroupConfig(UE->CellGroup, du2cu, sizeof(du2cu));

  DevAssert(mac->f1_config.setup_req != NULL);
  AssertFatal(mac->f1_config.setup_req->num_cells_available == 1, "can handle only one cell\n");
  const f1ap_initial_ul_rrc_message_t ul_rrc_msg = {
    .plmn = mac->f1_config.setup_req->cell[0].info.plmn,
    .nr_cellid = mac->f1_config.setup_req->cell[0].info.nr_cellid,
    .gNB_DU_ue_id = rnti,
    .crnti = rnti,
    .rrc_container = (uint8_t *) sdu,
    .rrc_container_length = sdu_len,
    .du2cu_rrc_container = (uint8_t *) du2cu,
    .du2cu_rrc_container_length = encoded
  };
  mac->mac_rrc.initial_ul_rrc_message_transfer(0, &ul_rrc_msg);
}
