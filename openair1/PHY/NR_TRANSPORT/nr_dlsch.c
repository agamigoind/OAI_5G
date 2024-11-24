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

/*! \file nr_dlsch.c
 * \brief Top-level routines for transmission of the PDSCH 38211 v 15.2.0
 * \author Guy De Souza
 * \date 2018
 * \version 0.1
 * \company Eurecom
 * \email: desouza@eurecom.fr
 * \note
 * \warning
 */

#include "nr_dlsch.h"
#include "nr_dci.h"
#include "nr_sch_dmrs.h"
#include "PHY/MODULATION/nr_modulation.h"
#include "PHY/NR_REFSIG/dmrs_nr.h"
#include "PHY/NR_REFSIG/ptrs_nr.h"
#include "common/utils/LOG/vcd_signal_dumper.h"
#include "common/utils/nr/nr_common.h"
#include "executables/softmodem-common.h"
#include "SCHED_NR/sched_nr.h"

//#define DEBUG_DLSCH
//#define DEBUG_DLSCH_MAPPING


static void nr_pdsch_codeword_scrambling(uint8_t *in, uint32_t size, uint8_t q, uint32_t Nid, uint32_t n_RNTI, uint32_t *out)
{
  nr_codeword_scrambling(in, size, q, Nid, n_RNTI, out);
}

void nr_generate_pdsch(processingData_L1tx_t *msgTx, int frame, int slot)
{
  PHY_VARS_gNB *gNB = msgTx->gNB;
  const int16_t amp = gNB->TX_AMP;
  NR_DL_FRAME_PARMS *frame_parms = &gNB->frame_parms;
  time_stats_t *dlsch_encoding_stats=&gNB->dlsch_encoding_stats;
  time_stats_t *dlsch_scrambling_stats=&gNB->dlsch_scrambling_stats;
  time_stats_t *dlsch_modulation_stats=&gNB->dlsch_modulation_stats;
  time_stats_t *tinput=&gNB->tinput;
  time_stats_t *tprep=&gNB->tprep;
  time_stats_t *tparity=&gNB->tparity;
  time_stats_t *toutput=&gNB->toutput;
  time_stats_t *dlsch_rate_matching_stats=&gNB->dlsch_rate_matching_stats;
  time_stats_t *dlsch_interleaving_stats=&gNB->dlsch_interleaving_stats;
  time_stats_t *dlsch_segmentation_stats=&gNB->dlsch_segmentation_stats;

  for (int dlsch_id=0; dlsch_id<msgTx->num_pdsch_slot; dlsch_id++) {
    NR_gNB_DLSCH_t *dlsch = msgTx->dlsch[dlsch_id];

    NR_DL_gNB_HARQ_t *harq = &dlsch->harq_process;
    nfapi_nr_dl_tti_pdsch_pdu_rel15_t *rel15 = &harq->pdsch_pdu.pdsch_pdu_rel15;
    const int layerSz = frame_parms->N_RB_DL * NR_SYMBOLS_PER_SLOT * NR_NB_SC_PER_RB;
    const int dmrs_Type = rel15->dmrsConfigType;
    const int nb_re_dmrs = rel15->numDmrsCdmGrpsNoData * (rel15->dmrsConfigType == NFAPI_NR_DMRS_TYPE1 ? 6 : 4);
    const int amp_dmrs = (int)((double)amp * sqrt(rel15->numDmrsCdmGrpsNoData)); // 3GPP TS 38.214 Section 4.1: Table 4.1-1
    LOG_D(PHY,"pdsch: BWPStart %d, BWPSize %d, rbStart %d, rbsize %d\n",
          rel15->BWPStart,rel15->BWPSize,rel15->rbStart,rel15->rbSize);
    const int n_dmrs = (rel15->BWPStart + rel15->rbStart + rel15->rbSize) * nb_re_dmrs;

    const int dmrs_symbol_map = rel15->dlDmrsSymbPos; // single DMRS: 010000100 Double DMRS 110001100
    const int xOverhead = 0;
    const int nb_re =
        (12 * rel15->NrOfSymbols - nb_re_dmrs * get_num_dmrs(rel15->dlDmrsSymbPos) - xOverhead) * rel15->rbSize * rel15->nrOfLayers;
    const int Qm = rel15->qamModOrder[0];
    const int encoded_length = nb_re * Qm;

    /* PTRS */
    uint16_t dlPtrsSymPos = 0;
    int n_ptrs = 0;
    uint32_t ptrsSymbPerSlot = 0;
    if(rel15->pduBitmap & 0x1) {
      set_ptrs_symb_idx(&dlPtrsSymPos,
                        rel15->NrOfSymbols,
                        rel15->StartSymbolIndex,
                        1 << rel15->PTRSTimeDensity,
                        rel15->dlDmrsSymbPos);
      n_ptrs = (rel15->rbSize + rel15->PTRSFreqDensity - 1) / rel15->PTRSFreqDensity;
      ptrsSymbPerSlot = get_ptrs_symbols_in_slot(dlPtrsSymPos, rel15->StartSymbolIndex, rel15->NrOfSymbols);
    }
    harq->unav_res = ptrsSymbPerSlot * n_ptrs;

    /// CRC, coding, interleaving and rate matching
    AssertFatal(harq->pdu!=NULL,"harq->pdu is null\n");
    unsigned char output[rel15->rbSize * NR_SYMBOLS_PER_SLOT * NR_NB_SC_PER_RB * Qm * rel15->nrOfLayers] __attribute__((aligned(64)));
    bzero(output,rel15->rbSize * NR_SYMBOLS_PER_SLOT * NR_NB_SC_PER_RB * Qm * rel15->nrOfLayers);
    start_meas(dlsch_encoding_stats);

    if (nr_dlsch_encoding(gNB,
                          frame,
                          slot,
                          harq,
                          frame_parms,
                          output,
                          tinput,
                          tprep,
                          tparity,
                          toutput,
                          dlsch_rate_matching_stats,
                          dlsch_interleaving_stats,
                          dlsch_segmentation_stats) == -1)
      return;
    stop_meas(dlsch_encoding_stats);
#ifdef DEBUG_DLSCH
    printf("PDSCH encoding:\nPayload:\n");
    for (int i=0; i<harq->B>>7; i++) {
      for (int j=0; j<16; j++)
	printf("0x%02x\t", harq->pdu[(i<<4)+j]);
      printf("\n");
    }
    printf("\nEncoded payload:\n");
    for (int i=0; i<encoded_length>>3; i++) {
      for (int j=0; j<8; j++)
	printf("%d", output[(i<<3)+j]);
      printf("\t");
    }
    printf("\n");
#endif

    if (IS_SOFTMODEM_DLSIM)
      memcpy(harq->f, output, encoded_length);

    c16_t mod_symbs[rel15->NrOfCodewords][encoded_length] __attribute__((aligned(64)));
    for (int codeWord = 0; codeWord < rel15->NrOfCodewords; codeWord++) {
      /// scrambling
      start_meas(dlsch_scrambling_stats);
      uint32_t scrambled_output[(encoded_length>>5)+4]; // modulator acces by 4 bytes in some cases
      memset(scrambled_output, 0, sizeof(scrambled_output));
      if ( encoded_length > rel15->rbSize * NR_SYMBOLS_PER_SLOT * NR_NB_SC_PER_RB * Qm * rel15->nrOfLayers) abort();
      nr_pdsch_codeword_scrambling(output, encoded_length, codeWord, rel15->dataScramblingId, rel15->rnti, scrambled_output);

#ifdef DEBUG_DLSCH
      printf("PDSCH scrambling:\n");
      for (int i=0; i<encoded_length>>8; i++) {
        for (int j=0; j<8; j++)
          printf("0x%08x\t", scrambled_output[(i<<3)+j]);
        printf("\n");
      }
#endif

      stop_meas(dlsch_scrambling_stats);
      /// Modulation
      start_meas(dlsch_modulation_stats);
      nr_modulation(scrambled_output, encoded_length, Qm, (int16_t *)mod_symbs[codeWord]);
      VCD_SIGNAL_DUMPER_DUMP_FUNCTION_BY_NAME(VCD_SIGNAL_DUMPER_FUNCTIONS_gNB_PDSCH_MODULATION, 0);
      stop_meas(dlsch_modulation_stats);
#ifdef DEBUG_DLSCH
      printf("PDSCH Modulation: Qm %d(%d)\n", Qm, nb_re);
      for (int i = 0; i < nb_re; i += 8) {
        for (int j=0; j<8; j++) {
          printf("%d %d\t", mod_symbs[codeWord][i + j].r, mod_symbs[codeWord][i + j].i);
        }
        printf("\n");
      }
#endif
    }
    /// Resource mapping
    
    // Non interleaved VRB to PRB mapping
    uint16_t start_sc = frame_parms->first_carrier_offset + (rel15->rbStart+rel15->BWPStart)*NR_NB_SC_PER_RB;
    if (start_sc >= frame_parms->ofdm_symbol_size)
      start_sc -= frame_parms->ofdm_symbol_size;

    const uint32_t txdataF_offset = slot * frame_parms->samples_per_slot_wCP;
    c16_t txdataF_precoding[rel15->nrOfLayers][NR_NUMBER_OF_SYMBOLS_PER_SLOT][frame_parms->ofdm_symbol_size] __attribute__((aligned(64)));;

#ifdef DEBUG_DLSCH_MAPPING
    printf("PDSCH resource mapping started (start SC %d\tstart symbol %d\tN_PRB %d\tnb_re %d,nb_layers %d)\n",
           start_sc,
           rel15->StartSymbolIndex,
           rel15->rbSize,
           nb_re,
           rel15->nrOfLayers);
#endif

    AssertFatal(n_dmrs, "n_dmrs can't be 0\n");
    c16_t mod_dmrs[n_dmrs] __attribute__((aligned(64)));
    unsigned int cur_re0=0;
    start_meas(&gNB->dlsch_resource_mapping_stats);
      // Loop Over OFDM symbols:
    for (int l_symbol = rel15->StartSymbolIndex; l_symbol < rel15->StartSymbolIndex + rel15->NrOfSymbols; l_symbol++) {
      int8_t l_prime = 0; // single symbol layer 0
      int8_t l_overline = get_l0(rel15->dlDmrsSymbPos);

      uint32_t dmrs_idx = 0;
#ifdef DEBUG_DLSCH_MAPPING
      printf("PDSCH resource mapping symbol %d\n",l_symbol);
#endif 
        /// DMRS QPSK modulation
      if ((dmrs_symbol_map & (1 << l_symbol))) { // DMRS time occasion
        // The reference point for is subcarrier -1 of the lowest-numbered resource block in CORESET 0 if the corresponding
        // PDCCH is associated with CORESET -1 and Type0-PDCCH common search space and is addressed to SI-RNTI
        // 2GPP TS 38.211 V15.8.0 Section 7.4.1.1.2 Mapping to physical resources
        if (l_symbol == (l_overline + 1)) // take into account the double DMRS symbols
          l_prime = 1;
        else if (l_symbol > (l_overline + 1)) { // new DMRS pair
          l_overline = l_symbol;
          l_prime = 0;
        }
        /// DMRS QPSK modulation
        NR_DL_FRAME_PARMS *fp = &gNB->frame_parms;
#ifdef DEBUG_DLSCH_MAPPING
        printf("dlDmrsScramblingId %d, SCID %d slot %d l_symbol %d\n", rel15->dlDmrsScramblingId, rel15->SCID, slot, l_symbol);
#endif
            const uint32_t *gold =
            nr_gold_pdsch(fp->N_RB_DL, fp->symbols_per_slot, rel15->dlDmrsScramblingId, rel15->SCID, slot, l_symbol);
        // Qm = 1 as DMRS is QPSK modulated
        nr_modulation(gold, n_dmrs * DMRS_MOD_ORDER, DMRS_MOD_ORDER, (int16_t *)mod_dmrs);

#ifdef DEBUG_DLSCH_MAPPING
        printf("DMRS modulation (symbol %d, %d symbols, type %d):\n", l_symbol, n_dmrs, dmrs_Type);
        for (int i = 0; i < n_dmrs / 2; i += 8) {
          for (int j = 0; j < 8; j++) {
            printf("%d %d\t", mod_dmrs[i + j].r, mod_dmrs[i + j].i);
          }
          printf("\n");
        }
#endif
      }
 
    uint32_t cur_re=cur_re0;
    c16_t tx_layers[rel15->nrOfLayers][layerSz] __attribute__((aligned(64)));
    if (l_symbol == rel15->StartSymbolIndex) nr_layer_mapping(rel15->NrOfCodewords, encoded_length, mod_symbs, rel15->nrOfLayers, layerSz, nb_re, tx_layers);
    dmrs_idx = rel15->rbStart;
    if (rel15->rnti != SI_RNTI)
    dmrs_idx += rel15->BWPStart;
    dmrs_idx *= dmrs_Type == NFAPI_NR_DMRS_TYPE1 ? 6 : 4;
    if (dmrs_idx>0) memmove(mod_dmrs,mod_dmrs+dmrs_idx,rel15->rbSize*(NFAPI_NR_DMRS_TYPE1 ? 6 : 4)*sizeof(c16_t));
    for (int layer = 0; layer < rel15->nrOfLayers; layer++) {
      uint8_t k_prime = 0;
      uint16_t n = 0;
      cur_re = cur_re0; 




        /* calculate if current symbol is PTRS symbols */
        int ptrs_idx = 0;
        int ptrs_symbol = 0;
        c16_t mod_ptrs[max(n_ptrs, 1)] __attribute__((aligned(64))); //max only to please sanitizer, that kills if 0 even if it is not a error
        if(rel15->pduBitmap & 0x1) {
          ptrs_symbol = is_ptrs_symbol(l_symbol, dlPtrsSymPos);
          if(ptrs_symbol) {
            /* PTRS QPSK Modulation for each OFDM symbol in a slot */
            LOG_D(PHY, "Doing ptrs modulation for symbol %d, n_ptrs %d\n", l_symbol, n_ptrs);
            NR_DL_FRAME_PARMS *fp = &gNB->frame_parms;
            const uint32_t *gold =
                nr_gold_pdsch(fp->N_RB_DL, fp->symbols_per_slot, rel15->dlDmrsScramblingId, rel15->SCID, slot, l_symbol);
            nr_modulation(gold, n_ptrs * DMRS_MOD_ORDER, DMRS_MOD_ORDER, (int16_t *)mod_ptrs);
          }
        }
        uint16_t k = start_sc;
	
        if (ptrs_symbol) {
            uint8_t is_ptrs_re = 0;
            for (int i=0; i<rel15->rbSize*NR_NB_SC_PER_RB; i++) {
            /* check for PTRS symbol and set flag for PTRS RE */
                is_ptrs_re = is_ptrs_subcarrier(k,
                                                rel15->rnti,
                                                rel15->PTRSFreqDensity,
                                                rel15->rbSize,
                                                rel15->PTRSReOffset,
                                                start_sc,
                                                frame_parms->ofdm_symbol_size);
                if (is_ptrs_re) {
                   /* check if cuurent RE is PTRS RE*/
                   uint16_t beta_ptrs = 1;
                   txdataF_precoding[layer][l_symbol][k] = c16mulRealShift(mod_ptrs[ptrs_idx], beta_ptrs * amp, 15);
#ifdef DEBUG_DLSCH_MAPPING
                   printf("ptrs_idx %d\t l %d \t k %d \t k_prime %d \t n %d \t txdataF: %d %d, mod_ptrs: %d %d\n",
                          ptrs_idx,
                          l_symbol,
                          k,
                          k_prime,
                          n,
                          txdataF_precoding[layer][l_symbol][k].r,
                          txdataF_precoding[layer][l_symbol][k].i,
                          mod_ptrs[ptrs_idx].r,
                          mod_ptrs[ptrs_idx].i);
#endif
                   ptrs_idx++;
                }
		else {
                  txdataF_precoding[layer][l_symbol][k] = c16mulRealShift(tx_layers[layer][cur_re], amp, 15);
#ifdef DEBUG_DLSCH_MAPPING
                  printf("re %u\t l %d\t layer %d\t k %d \t txdataF: %d %d\n",
                         cur_re,
                         l_symbol,layer,
                         k,
                         txdataF_precoding[layer][l_symbol][k].r,
                         txdataF_precoding[layer][l_symbol][k].i);
#endif
		}
                cur_re++;
                if (++k >= frame_parms->ofdm_symbol_size)
                  k -= frame_parms->ofdm_symbol_size;
	    }
        } else if (dmrs_symbol_map & (1 << l_symbol)) {
            /* Map DMRS Symbol */
            int dmrs_port = get_dmrs_port(layer, rel15->dmrsPorts);

#ifdef DEBUG_DLSCH_MAPPING
              int Wt[2], Wf[2];
              get_Wt(Wt, dmrs_port, dmrs_Type);
              get_Wf(Wf, dmrs_port, dmrs_Type);
              const int8_t delta = get_delta(dmrs_port, dmrs_Type);
	      uint8_t dmrs_symbol = l_overline + l_prime;


            printf("DMRS Type %d params for layer %d : numDmrsCdmGrpsNoData %d Wt %d %d \t Wf %d %d \t delta %d \t l_prime %d \t l0 %d\tDMRS symbol %d\n",
                   1 + dmrs_Type,
                   layer,
	           rel15->numDmrsCdmGrpsNoData,
                   Wt[0],
                   Wt[1],
                   Wf[0],
                   Wf[1],
                   delta,
                   l_prime,
                   l_overline,
                   dmrs_symbol); 
#endif
            c16_t dmrs_mux[rel15->rbSize*NR_NB_SC_PER_RB] __attribute__((aligned(64)));

            int upper_limit=rel15->rbSize*NR_NB_SC_PER_RB;
            int remaining_re = 0;
            if (start_sc + upper_limit > frame_parms->ofdm_symbol_size) {
              remaining_re = upper_limit + start_sc - frame_parms->ofdm_symbol_size;
              upper_limit = frame_parms->ofdm_symbol_size - start_sc;
            }
	    c16_t *txF = txdataF_precoding[layer][l_symbol];

            if (rel15->numDmrsCdmGrpsNoData==2 && dmrs_Type == 0 && (dmrs_port&3) == 0 && l_prime == 0) {
#ifdef DEBUG_DLSCH_MAPPING
		   printf("doing DMRS pattern for port 0 : d0 0 d1 0 ... dNm2 0 dNm1 0 (ul %d, rr %d)\n",upper_limit,remaining_re);
#endif 
		   simde__m256i d0,d2,d3,zeros=simde_mm256_xor_si256(d0,d0),amp_dmrs256=simde_mm256_set1_epi16(amp_dmrs);
		   int i,j;
		   for (i=0,j=0;i<(rel15->rbSize*NR_NB_SC_PER_RB)>>4;i++) {
		      d0 = simde_mm256_mulhrs_epi16(((simde__m256i *)mod_dmrs)[i],amp_dmrs256);
		      d2 = simde_mm256_unpacklo_epi32(d0,zeros);
		      d3 = simde_mm256_unpackhi_epi32(d0,zeros);  
                      ((simde__m256i *)dmrs_mux)[j++] = _mm256_permute2x128_si256(d2,d3,32) ; 
                      ((simde__m256i *)dmrs_mux)[j++] = _mm256_permute2x128_si256(d2,d3,49) ; 
                     
		   }
		   if ((i<<4) != (rel15->rbSize*NR_NB_SC_PER_RB)) {
                       for (int i2=(((rel15->rbSize*NR_NB_SC_PER_RB)>>4)<<3),j2=(((rel15->rbSize*NR_NB_SC_PER_RB)>>4)<<4) ; i2< (rel15->rbSize*NR_NB_SC_PER_RB)>>1 ;i2++) {
                          dmrs_mux[j2].r = (((mod_dmrs[i2].r * amp_dmrs) >> 14) + 1) >> 1;
                          dmrs_mux[j2].i = (((mod_dmrs[i2].i * amp_dmrs) >> 14) + 1) >> 1;j2++;
                          dmrs_mux[j2].r = 0;
                          dmrs_mux[j2].i = 0;j2++;
		       }
		   }
		   memcpy(&txF[start_sc],dmrs_mux,sizeof(c16_t)*upper_limit);
		   if (remaining_re) memcpy(&txF[0],&dmrs_mux[upper_limit],sizeof(c16_t)*remaining_re);   
#ifdef DEBUG_DLSCH_MAPPING
                   for (int re=0;re<rel15->rbSize*NR_NB_SC_PER_RB;re+=2)

                         printf("dmrs_idx %u  (%d %d)*%d\t l %d \t layer %d \t k %d \t k_prime %d \t n %d \t txdataF: %d %d\n", 
                                re>>1,mod_dmrs[re>>1].r,mod_dmrs[re>>1].i,amp_dmrs,
                                l_symbol,layer,
                                (re+start_sc)%frame_parms->ofdm_symbol_size,
                                k_prime,
                                re,
                                txdataF_precoding[layer][l_symbol][(re+start_sc)%frame_parms->ofdm_symbol_size].r,
                                txdataF_precoding[layer][l_symbol][(re+start_sc)%frame_parms->ofdm_symbol_size].i);
#endif
            }
            else if (rel15->numDmrsCdmGrpsNoData==2 && dmrs_Type == 0 && (dmrs_port&3) == 1 && l_prime == 0) {
#ifdef DEBUG_DLSCH_MAPPING
                   printf("doing DMRS pattern for port 1 : d0 0 -d1 0 ...dNm2 0 -dNm1 0\n");  
#endif
		   simde__m256i d0,d2,d3,zeros=simde_mm256_xor_si256(d0,d0),amp_dmrs256=simde_mm256_set_epi16(-amp_dmrs,-amp_dmrs,amp_dmrs,amp_dmrs,-amp_dmrs,-amp_dmrs,amp_dmrs,amp_dmrs,-amp_dmrs,-amp_dmrs,amp_dmrs,amp_dmrs,-amp_dmrs,-amp_dmrs,amp_dmrs,amp_dmrs);
		   int i,j;
		   for (i=0,j=0;i<(rel15->rbSize*NR_NB_SC_PER_RB)>>4;i++) {
		      d0 = simde_mm256_mulhrs_epi16(((simde__m256i *)mod_dmrs)[i],amp_dmrs256);
		      d2 = simde_mm256_unpacklo_epi32(d0,zeros);
		      d3 = simde_mm256_unpackhi_epi32(d0,zeros);  
                      ((simde__m256i *)dmrs_mux)[j++] = _mm256_permute2x128_si256(d2,d3,32) ; 
                      ((simde__m256i *)dmrs_mux)[j++] = _mm256_permute2x128_si256(d2,d3,49) ; 
		   }
		   if ((i<<4) != (rel15->rbSize*NR_NB_SC_PER_RB)) {
                       for (int i2=(((rel15->rbSize*NR_NB_SC_PER_RB)>>4)<<3),j2=(((rel15->rbSize*NR_NB_SC_PER_RB)>>4)<<4) ; i2< (rel15->rbSize*NR_NB_SC_PER_RB)>>1 ;i2+=2) {
                          dmrs_mux[j2].r = (((mod_dmrs[i2].r * amp_dmrs) >> 14) + 1) >> 1;
                          dmrs_mux[j2].i = (((mod_dmrs[i2].i * amp_dmrs) >> 14) + 1) >> 1;j2++;
                          dmrs_mux[j2].r = 0;
                          dmrs_mux[j2].i = 0;j2++;
                          dmrs_mux[j2].r = -(((mod_dmrs[i2+1].r * amp_dmrs) >> 14) + 1) >> 1;
                          dmrs_mux[j2].i = -(((mod_dmrs[i2+1].i * amp_dmrs) >> 14) + 1) >> 1;j2++;
                          dmrs_mux[j2].r = 0;
                          dmrs_mux[j2].i = 0;j2++;
		       }
		   }
		   memcpy(&txF[start_sc],dmrs_mux,sizeof(c16_t)*upper_limit);
		   if (remaining_re) memcpy(&txF[0],&dmrs_mux[upper_limit],sizeof(c16_t)*remaining_re);   
#ifdef DEBUG_DLSCH_MAPPING
                   for (int re=0;re<rel15->rbSize*NR_NB_SC_PER_RB;re+=2)

                         printf("dmrs_idx %u \t l %d \t layer %d \t k %d \t k_prime %d \t n %d \t txdataF: %d %d\n", 
                                re>>1,
                                l_symbol,layer,
                                (re+start_sc)%frame_parms->ofdm_symbol_size,
                                k_prime,
                                re,
                                txdataF_precoding[layer][l_symbol][(re+start_sc)%frame_parms->ofdm_symbol_size].r,
                                txdataF_precoding[layer][l_symbol][(re+start_sc)%frame_parms->ofdm_symbol_size].i);
#endif
            }
            else if (rel15->numDmrsCdmGrpsNoData==2 && dmrs_Type == 0 && (dmrs_port&3) == 2 && l_prime == 0) {
#ifdef DEBUG_DLSCH_MAPPING
                   printf("doing DMRS pattern for port 2 : 0 d0 0 d1 ... 0 dNm2 0 dNm1\n");
#endif  
		   simde__m256i d0,d2,d3,zeros=simde_mm256_xor_si256(d0,d0),amp_dmrs256=simde_mm256_set1_epi16(amp_dmrs);
		   int i,j;
		   for (i=0,j=0;i<(rel15->rbSize*NR_NB_SC_PER_RB)>>4;i++) {
		      d0 = simde_mm256_mulhrs_epi16(((simde__m256i *)mod_dmrs)[i],amp_dmrs256);
		      d2 = simde_mm256_unpacklo_epi32(zeros,d0);
		      d3 = simde_mm256_unpackhi_epi32(zeros,d0);  
                      ((simde__m256i *)dmrs_mux)[j++] = _mm256_permute2x128_si256(d2,d3,32) ; 
                      ((simde__m256i *)dmrs_mux)[j++] = _mm256_permute2x128_si256(d2,d3,49) ; 
		   }
		   if ((i<<4) != (rel15->rbSize*NR_NB_SC_PER_RB)) {
                       for (int i2=(((rel15->rbSize*NR_NB_SC_PER_RB)>>4)<<3),j2=(((rel15->rbSize*NR_NB_SC_PER_RB)>>4)<<4) ; i2< (rel15->rbSize*NR_NB_SC_PER_RB)>>1 ;i2++) {
                          dmrs_mux[j2].r = 0;
                          dmrs_mux[j2].i = 0;j2++;
                          dmrs_mux[j2].r = (((mod_dmrs[i2].r * amp_dmrs) >> 14) + 1) >> 1;
                          dmrs_mux[j2].i = (((mod_dmrs[i2].i * amp_dmrs) >> 14) + 1) >> 1;j2++;
		       }
		   }
		   memcpy(&txF[start_sc],dmrs_mux,sizeof(c16_t)*upper_limit);
	  	   if (remaining_re) memcpy(&txF[0],&dmrs_mux[upper_limit],sizeof(c16_t)*remaining_re);   
#ifdef DEBUG_DLSCH_MAPPING
                   for (int re=0;re<rel15->rbSize*NR_NB_SC_PER_RB;re+=2)

                         printf("dmrs_idx %u \t l %d \t layer %d \t k %d \t k_prime %d \t n %d \t txdataF: %d %d\n", 
                                re>>1,
                                l_symbol,layer,
                                (re+start_sc+1)%frame_parms->ofdm_symbol_size,
                                k_prime,
                                re,
                                txdataF_precoding[layer][l_symbol][(re+start_sc+1)%frame_parms->ofdm_symbol_size].r,
                                txdataF_precoding[layer][l_symbol][(re+start_sc+1)%frame_parms->ofdm_symbol_size].i);
#endif
            }
            else if (rel15->numDmrsCdmGrpsNoData==2 && dmrs_Type == 0 && (dmrs_port&3) == 3 && l_prime == 0) {
#ifdef DEBUG_DLSCH_MAPPING
                   printf("doing DMRS pattern for port 3 : 0 d0 0 -d1 ... 0 dNm2 0 -dNm1\n");
#endif  
		   simde__m256i d0,d2,d3,zeros=simde_mm256_xor_si256(d0,d0),amp_dmrs256=simde_mm256_set_epi16(-amp_dmrs,-amp_dmrs,amp_dmrs,amp_dmrs,-amp_dmrs,-amp_dmrs,amp_dmrs,amp_dmrs,-amp_dmrs,-amp_dmrs,amp_dmrs,amp_dmrs,-amp_dmrs,-amp_dmrs,amp_dmrs,amp_dmrs);
		   int i,j;
		   for (i=0,j=0;i<(rel15->rbSize*NR_NB_SC_PER_RB)>>4;i++) {
		      d0 = simde_mm256_mulhrs_epi16(((simde__m256i *)mod_dmrs)[i],amp_dmrs256);
		      d2 = simde_mm256_unpacklo_epi32(zeros,d0);
		      d3 = simde_mm256_unpackhi_epi32(zeros,d0);  
                      ((simde__m256i *)dmrs_mux)[j++] = _mm256_permute2x128_si256(d2,d3,32) ; 
                      ((simde__m256i *)dmrs_mux)[j++] = _mm256_permute2x128_si256(d2,d3,49) ; 
		   }
		   if ((i<<4) != (rel15->rbSize*NR_NB_SC_PER_RB)) {
                       for (int i2=(((rel15->rbSize*NR_NB_SC_PER_RB)>>4)<<3),j2=(((rel15->rbSize*NR_NB_SC_PER_RB)>>4)<<4) ; i2< (rel15->rbSize*NR_NB_SC_PER_RB)>>1 ;i2+=2) {
                          dmrs_mux[j2].r = 0;
                          dmrs_mux[j2].i = 0;j2++;
                          dmrs_mux[j2].r = (((mod_dmrs[i2].r * amp_dmrs) >> 14) + 1) >> 1;;
                          dmrs_mux[j2].i = (((mod_dmrs[i2].i * amp_dmrs) >> 14) + 1) >> 1;j2++;
                          dmrs_mux[j2].r = 0;
                          dmrs_mux[j2].i = 0;j2++;
                          dmrs_mux[j2].r = -(((mod_dmrs[i2+1].r * amp_dmrs) >> 14) + 1) >> 1;;
                          dmrs_mux[j2].i = -(((mod_dmrs[i2+1].i * amp_dmrs) >> 14) + 1) >> 1;j2++;
		       }
		   }
		   memcpy(&txF[start_sc],dmrs_mux,sizeof(c16_t)*upper_limit);
		   if (remaining_re) memcpy(&txF[0],&dmrs_mux[upper_limit],sizeof(c16_t)*remaining_re);   
#ifdef DEBUG_DLSCH_MAPPING
                   for (int re=0;re<rel15->rbSize*NR_NB_SC_PER_RB;re+=2)

                         printf("dmrs_idx %u \t l %d \t layer %d \t k %d \t k_prime %d \t n %d \t txdataF: %d %d\n", 
                                re>>1,
                                l_symbol,layer,
                                (re+start_sc+1)%frame_parms->ofdm_symbol_size,
                                k_prime,
                                re>>1,
                                txdataF_precoding[layer][l_symbol][(re+start_sc+1)%frame_parms->ofdm_symbol_size].r,
                                txdataF_precoding[layer][l_symbol][(re+start_sc+1)%frame_parms->ofdm_symbol_size].i);
#endif
            }
	    else if (rel15->numDmrsCdmGrpsNoData==1 && dmrs_Type == 0 && (dmrs_port&3) == 0 && l_prime == 0) {
#ifdef DEBUG_DLSCH_MAPPING
		   printf("doing DMRS pattern for port 0 : d0 X0 d1 X1 ... dNm2 XNm2 dNm1 XNm1\n");
#endif 
		   simde__m256i d0,d1,d2,d3,amp_dmrs256=simde_mm256_set1_epi16(amp_dmrs),amp256=simde_mm256_set1_epi16(amp);
                   c16_t *txlc = &tx_layers[layer][cur_re];
		   int i,j;
		   for (i=0,j=0;i<(rel15->rbSize*NR_NB_SC_PER_RB)>>4;i++) {
		      d0 = simde_mm256_mulhrs_epi16(((simde__m256i *)mod_dmrs)[i],amp_dmrs256);
		      d1 = simde_mm256_mulhrs_epi16(((simde__m256i *)txlc)[i],amp256);
		      d2 = simde_mm256_unpacklo_epi32(d0,d1);
		      d3 = simde_mm256_unpackhi_epi32(d0,d1);  
                      ((simde__m256i *)dmrs_mux)[j++] = _mm256_permute2x128_si256(d2,d3,32) ; 
                      ((simde__m256i *)dmrs_mux)[j++] = _mm256_permute2x128_si256(d2,d3,49) ; 
		   }
		   if ((i<<4) != (rel15->rbSize*NR_NB_SC_PER_RB)) {
                       for (int i2=(((rel15->rbSize*NR_NB_SC_PER_RB)>>4)<<3),j2=(((rel15->rbSize*NR_NB_SC_PER_RB)>>4)<<4) ; i2< (rel15->rbSize*NR_NB_SC_PER_RB)>>1 ;i2++) {
                          dmrs_mux[j2].r = (((mod_dmrs[i2].r * amp_dmrs) >> 14) + 1) >> 1;
                          dmrs_mux[j2].i = (((mod_dmrs[i2].i * amp_dmrs) >> 14) + 1) >> 1;j2++;
                          dmrs_mux[j2].r = (((txlc[i2].r * amp) >> 14) + 1) >> 1;
                          dmrs_mux[j2].i = (((txlc[i2].i * amp) >> 14) + 1) >> 1;j2++;
		       }
		   }
		   memcpy(&txF[start_sc],dmrs_mux,sizeof(c16_t)*upper_limit);
		   if (remaining_re) memcpy(&txF[0],&dmrs_mux[upper_limit],sizeof(c16_t)*remaining_re);   
		   cur_re+=(rel15->rbSize*NR_NB_SC_PER_RB)>>1;
#ifdef DEBUG_DLSCH_MAPPING
                   for (int re=0;re<rel15->rbSize*NR_NB_SC_PER_RB;re+=2) {

                         printf("dmrs_idx %u \t l %d \t layer %d \t k %d \t k_prime %d \t n %d \t txdataF: %d %d\n", 
                                re>>1,
                                l_symbol,layer,
                                (re+start_sc)%frame_parms->ofdm_symbol_size,
                                k_prime,
                                re>>1,
                                txdataF_precoding[layer][l_symbol][(re+start_sc)%frame_parms->ofdm_symbol_size].r,
                                txdataF_precoding[layer][l_symbol][(re+start_sc)%frame_parms->ofdm_symbol_size].i);
                         printf("data %u \t l %d \t layer %d \t k %d \t k_prime %d \t n %d \t txdataF: %d %d\n", 
                                re>>1,
                                l_symbol,layer,
                                (re+start_sc)%frame_parms->ofdm_symbol_size,
                                k_prime,
                                re>>1,
                                txdataF_precoding[layer][l_symbol][(re+start_sc+1)%frame_parms->ofdm_symbol_size].r,
                                txdataF_precoding[layer][l_symbol][(re+start_sc+1)%frame_parms->ofdm_symbol_size].i);
		   }
#endif
            }
            else if (rel15->numDmrsCdmGrpsNoData==1 && dmrs_Type == 0 && (dmrs_port&3) == 1 && l_prime == 0) {
#ifdef DEBUG_DLSCH_MAPPING
                   printf("doing DMRS pattern for port 1 : d0 X0 -d1 X1 ...dNm2 XNm2 -dNm1 XNm1\n");  
#endif
		   simde__m256i d0,d1,d2,d3,amp_dmrs256=simde_mm256_set_epi16(-amp_dmrs,-amp_dmrs,amp_dmrs,amp_dmrs,-amp_dmrs,-amp_dmrs,amp_dmrs,amp_dmrs,-amp_dmrs,-amp_dmrs,amp_dmrs,amp_dmrs,-amp_dmrs,-amp_dmrs,amp_dmrs,amp_dmrs),amp256=simde_mm256_set1_epi16(amp);
                   c16_t *txlc = &tx_layers[layer][cur_re];
		   int i,j;
		   for (i=0,j=0;i<(rel15->rbSize*NR_NB_SC_PER_RB)>>4;i++) {
		      d0 = simde_mm256_mulhrs_epi16(((simde__m256i *)mod_dmrs)[i],amp_dmrs256);
		      d1 = simde_mm256_mulhrs_epi16(((simde__m256i *)txlc)[i],amp256);
		      d2 = simde_mm256_unpacklo_epi32(d0,d1);
		      d3 = simde_mm256_unpackhi_epi32(d0,d1);  
                      ((simde__m256i *)dmrs_mux)[j++] = _mm256_permute2x128_si256(d2,d3,32) ; 
                      ((simde__m256i *)dmrs_mux)[j++] = _mm256_permute2x128_si256(d2,d3,49) ; 
		   }
		   if ((i<<4) != (rel15->rbSize*NR_NB_SC_PER_RB)) {
                       for (int i2=(((rel15->rbSize*NR_NB_SC_PER_RB)>>4)<<3),j2=(((rel15->rbSize*NR_NB_SC_PER_RB)>>4)<<4) ; i2< (rel15->rbSize*NR_NB_SC_PER_RB)>>1 ;i2+=2) {
                          dmrs_mux[j2].r = (((mod_dmrs[i2].r * amp_dmrs) >> 14) + 1) >> 1;
                          dmrs_mux[j2].i = (((mod_dmrs[i2].i * amp_dmrs) >> 14) + 1) >> 1;j2++;
                          dmrs_mux[j2].r = (((txlc[i2].r * amp) >> 14) + 1) >> 1;
                          dmrs_mux[j2].i = (((txlc[i2].i * amp) >> 14) + 1) >> 1;j2++;
                          dmrs_mux[j2].r = -(((mod_dmrs[i2+1].r * amp_dmrs) >> 14) + 1) >> 1;
                          dmrs_mux[j2].i = -(((mod_dmrs[i2+1].i * amp_dmrs) >> 14) + 1) >> 1;j2++;
                          dmrs_mux[j2].r = (((txlc[i2+1].r * amp) >> 14) + 1) >> 1;
                          dmrs_mux[j2].i = (((txlc[i2+1].i * amp) >> 14) + 1) >> 1;;j2++;
		       }
		   }
		   memcpy(&txF[start_sc],dmrs_mux,sizeof(c16_t)*upper_limit);
		   if (remaining_re) memcpy(&txF[0],&dmrs_mux[upper_limit],sizeof(c16_t)*remaining_re);   
		   cur_re+=(rel15->rbSize*NR_NB_SC_PER_RB)>>1;
#ifdef DEBUG_DLSCH_MAPPING
                   for (int re=0;re<rel15->rbSize*NR_NB_SC_PER_RB;re+=2) {

                         printf("dmrs_idx %u \t l %d \t layer %d \t k %d \t k_prime %d \t n %d \t txdataF: %d %d\n", 
                                re>>1,
                                l_symbol,layer,
                                (re+start_sc)%frame_parms->ofdm_symbol_size,
                                k_prime,
                                re>>1,
                                txdataF_precoding[layer][l_symbol][(re+start_sc)%frame_parms->ofdm_symbol_size].r,
                                txdataF_precoding[layer][l_symbol][(re+start_sc)%frame_parms->ofdm_symbol_size].i);
                         printf("data %u \t l %d \t layer %d \t k %d \t k_prime %d \t n %d \t txdataF: %d %d\n", 
                                re>>1,
                                l_symbol,layer,
                                (re+start_sc)%frame_parms->ofdm_symbol_size,
                                k_prime,
                                re>>1,
                                txdataF_precoding[layer][l_symbol][(re+start_sc+1)%frame_parms->ofdm_symbol_size].r,
                                txdataF_precoding[layer][l_symbol][(re+start_sc+1)%frame_parms->ofdm_symbol_size].i);
		   }
#endif
            }
            else if (rel15->numDmrsCdmGrpsNoData==1 && dmrs_Type == 0 && (dmrs_port&3) == 2 && l_prime == 0) {
#ifdef DEBUG_DLSCH_MAPPING
                   printf("doing DMRS pattern for port 2 : X0 d0 X1 d1 ... XNm2 dNm2 XNm1 dNm1\n");
#endif  
		   simde__m256i d0,d1,d2,d3,amp_dmrs256=simde_mm256_set1_epi16(amp_dmrs),amp256=simde_mm256_set1_epi16(amp);
                   c16_t *txlc = &tx_layers[layer][cur_re];
		   int i,j;
		   for (i=0,j=0;i<(rel15->rbSize*NR_NB_SC_PER_RB)>>4;i++) {
		      d1 = simde_mm256_mulhrs_epi16(((simde__m256i *)mod_dmrs)[i],amp_dmrs256);
		      d0 = simde_mm256_mulhrs_epi16(((simde__m256i *)txlc)[i],amp256);
		      d2 = simde_mm256_unpacklo_epi32(d0,d1);
		      d3 = simde_mm256_unpackhi_epi32(d0,d1);  
                      ((simde__m256i *)dmrs_mux)[j++] = _mm256_permute2x128_si256(d2,d3,32) ; 
                      ((simde__m256i *)dmrs_mux)[j++] = _mm256_permute2x128_si256(d2,d3,49) ; 
		   }
		   if ((i<<4) != (rel15->rbSize*NR_NB_SC_PER_RB)) {
                       for (int i2=(((rel15->rbSize*NR_NB_SC_PER_RB)>>4)<<3),j2=(((rel15->rbSize*NR_NB_SC_PER_RB)>>4)<<4) ; i2< (rel15->rbSize*NR_NB_SC_PER_RB)>>1 ;i2++) {
                          dmrs_mux[j2].r = (((txlc[i2].r * amp) >> 14) + 1) >> 1;
                          dmrs_mux[j2].i = (((txlc[i2].i * amp) >> 14) + 1) >> 1;j2++;
                          dmrs_mux[j2].r = (((mod_dmrs[i2].r * amp_dmrs) >> 14) + 1) >> 1;
                          dmrs_mux[j2].i = (((mod_dmrs[i2].i * amp_dmrs) >> 14) + 1) >> 1;j2++;
		       }
		   }
		   memcpy(&txF[start_sc],dmrs_mux,sizeof(c16_t)*upper_limit);
		   if (remaining_re) memcpy(&txF[0],&dmrs_mux[upper_limit],sizeof(c16_t)*remaining_re);   
		   cur_re+=(rel15->rbSize*NR_NB_SC_PER_RB)>>1;
#ifdef DEBUG_DLSCH_MAPPING
                   for (int re=0;re<rel15->rbSize*NR_NB_SC_PER_RB;re+=2) {

                         printf("re %u \t l %d \t layer %d \t k %d \t k_prime %d \t n %d \t txdataF: %d %d\n", 
                                re>>1,
                                l_symbol,layer,
                                (re+start_sc)%frame_parms->ofdm_symbol_size,
                                k_prime,
                                re>>1,
                                txdataF_precoding[layer][l_symbol][(re+start_sc)%frame_parms->ofdm_symbol_size].r,
                                txdataF_precoding[layer][l_symbol][(re+start_sc)%frame_parms->ofdm_symbol_size].i);
                         printf("dmrs_idx %u \t l %d \t layer %d \t k %d \t k_prime %d \t n %d \t txdataF: %d %d\n", 
                                re>>1,
                                l_symbol,layer,
                                (re+start_sc)%frame_parms->ofdm_symbol_size,
                                k_prime,
                                re>>1,
                                txdataF_precoding[layer][l_symbol][(re+start_sc+1)%frame_parms->ofdm_symbol_size].r,
                                txdataF_precoding[layer][l_symbol][(re+start_sc+1)%frame_parms->ofdm_symbol_size].i);
		   }
#endif
            }
            else if (rel15->numDmrsCdmGrpsNoData==1 && dmrs_Type == 0 && (dmrs_port&3) == 3 && l_prime == 0) {
#ifdef DEBUG_DLSCH_MAPPING
                   printf("doing DMRS pattern for port 3 : X0 d0 X1 -d1 ... XNm2 dNm2 XNm1 -dNm1\n");
#endif  
		   simde__m256i d0,d1,d2,d3,amp_dmrs256=simde_mm256_set_epi16(-amp_dmrs,-amp_dmrs,amp_dmrs,amp_dmrs,-amp_dmrs,-amp_dmrs,amp_dmrs,amp_dmrs,-amp_dmrs,-amp_dmrs,amp_dmrs,amp_dmrs,-amp_dmrs,-amp_dmrs,amp_dmrs,amp_dmrs),amp256=simde_mm256_set1_epi16(amp);
                   c16_t *txlc = &tx_layers[layer][cur_re];
		   int i,j;
		   for (i=0,j=0;i<(rel15->rbSize*NR_NB_SC_PER_RB)>>4;i++) {
		      d1 = simde_mm256_mulhrs_epi16(((simde__m256i *)mod_dmrs)[i],amp_dmrs256);
		      d0 = simde_mm256_mulhrs_epi16(((simde__m256i *)txlc)[i],amp256);
		      d2 = simde_mm256_unpacklo_epi32(d0,d1);
		      d3 = simde_mm256_unpackhi_epi32(d0,d1);  
                      ((simde__m256i *)dmrs_mux)[j++] = _mm256_permute2x128_si256(d2,d3,32) ; 
                      ((simde__m256i *)dmrs_mux)[j++] = _mm256_permute2x128_si256(d2,d3,49) ; 
		   }
		   if ((i<<4) != (rel15->rbSize*NR_NB_SC_PER_RB)) {
                       for (int i2=(((rel15->rbSize*NR_NB_SC_PER_RB)>>4)<<3),j2=(((rel15->rbSize*NR_NB_SC_PER_RB)>>4)<<4) ; i2< (rel15->rbSize*NR_NB_SC_PER_RB)>>1 ;i2+=2) {
                          dmrs_mux[j2].r = (((txlc[i2].r * amp) >> 14) + 1) >> 1;
                          dmrs_mux[j2].i = (((txlc[i2].i * amp) >> 14) + 1) >> 1;j2++;
                          dmrs_mux[j2].r = (((mod_dmrs[i2].r * amp_dmrs) >> 14) + 1) >> 1;
                          dmrs_mux[j2].i = (((mod_dmrs[i2].i * amp_dmrs) >> 14) + 1) >> 1;j2++;
                          dmrs_mux[j2].r = (((txlc[i2+1].r * amp) >> 14) + 1) >> 1;
                          dmrs_mux[j2].i = (((txlc[i2+1].i * amp) >> 14) + 1) >> 1;j2++;
                          dmrs_mux[j2].r = -(((mod_dmrs[i2+1].r * amp_dmrs) >> 14) + 1) >> 1;
                          dmrs_mux[j2].i = -(((mod_dmrs[i2+1].i * amp_dmrs) >> 14) + 1) >> 1;j2++;
		       }
		   }
		   memcpy(&txF[start_sc],dmrs_mux,sizeof(c16_t)*upper_limit);
		   if (remaining_re) memcpy(&txF[0],&dmrs_mux[upper_limit],sizeof(c16_t)*remaining_re);   
		   cur_re+=(rel15->rbSize*NR_NB_SC_PER_RB)>>1;
#ifdef DEBUG_DLSCH_MAPPING
                   for (int re=0;re<rel15->rbSize*NR_NB_SC_PER_RB;re+=2) {

                         printf("re %u \t l %d \t layer %d \t k %d \t k_prime %d \t n %d \t txdataF: %d %d\n", 
                                re>>1,
                                l_symbol,layer,
                                (re+start_sc)%frame_parms->ofdm_symbol_size,
                                k_prime,
                                re>>1,
                                txdataF_precoding[layer][l_symbol][(re+start_sc)%frame_parms->ofdm_symbol_size].r,
                                txdataF_precoding[layer][l_symbol][(re+start_sc)%frame_parms->ofdm_symbol_size].i);
                         printf("dmrs_idx %u \t l %d \t layer %d \t k %d \t k_prime %d \t n %d \t txdataF: %d %d\n", 
                                re>>1,
                                l_symbol,layer,
                                (re+start_sc)%frame_parms->ofdm_symbol_size,
                                k_prime,
                                re>>1,
                                txdataF_precoding[layer][l_symbol][(re+start_sc+1)%frame_parms->ofdm_symbol_size].r,
                                txdataF_precoding[layer][l_symbol][(re+start_sc+1)%frame_parms->ofdm_symbol_size].i);
		   }
#endif
            }
            else {
              // DMRS params for this dmrs port
              int Wt[2], Wf[2];
              get_Wt(Wt, dmrs_port, dmrs_Type);
              get_Wf(Wf, dmrs_port, dmrs_Type);
              const int8_t delta = get_delta(dmrs_port, dmrs_Type);

              for (int i=0; i<rel15->rbSize*NR_NB_SC_PER_RB; i++) {
                   if ((k == ((start_sc + get_dmrs_freq_idx(n, k_prime, delta, dmrs_Type)) % (frame_parms->ofdm_symbol_size)))) {
                      txdataF_precoding[layer][l_symbol][k] = c16mulRealShift(mod_dmrs[dmrs_idx], Wt[l_prime] * Wf[k_prime] * amp_dmrs, 15);
#ifdef DEBUG_DLSCH_MAPPING
                      printf("dmrs_idx %u \t l %d \t layer %d \t k %d \t k_prime %d \t n %d \t txdataF: %d %d\n", 
                             dmrs_idx,
                             l_symbol,layer,
                             k,
                             k_prime,
                             n,
                             txdataF_precoding[layer][l_symbol][k].r,
                             txdataF_precoding[layer][l_symbol][k].i);
#endif
                       dmrs_idx++;
                       k_prime++;
                       k_prime&=1;
                       n+=(k_prime)?0:1;
                    }
            /* Map PTRS Symbol */
            /* Map DATA Symbol */
                    else if (allowed_xlsch_re_in_dmrs_symbol(k,
                                                             start_sc,
                                                             frame_parms->ofdm_symbol_size,
                                                             rel15->numDmrsCdmGrpsNoData,
                                                             dmrs_Type)) {
		      txdataF_precoding[layer][l_symbol][k] = c16mulRealShift(tx_layers[layer][cur_re], amp, 15);
#ifdef DEBUG_DLSCH_MAPPING
		      printf("re %u\t l %d \t k %d \t txdataF: %d %d\n",
		              cur_re,
		              l_symbol,
		              k,
		              txdataF_precoding[layer][l_symbol][k].r,
		              txdataF_precoding[layer][l_symbol][k].i);
#endif
		      cur_re++;
		    }
                    /* mute RE */
                    else {
                       txdataF_precoding[layer][l_symbol][k] = (c16_t){0};
                    }
                    if (++k >= frame_parms->ofdm_symbol_size)
                        k -= frame_parms->ofdm_symbol_size;
              } //RE loop
	    } // generic DMRS case
        } else { // no PTRS or DMRS in this symbol
          // Loop Over SCs:
          int upper_limit=rel15->rbSize*NR_NB_SC_PER_RB;
          int remaining_re = 0;
          if (start_sc + upper_limit > frame_parms->ofdm_symbol_size) {
            remaining_re = upper_limit + start_sc - frame_parms->ofdm_symbol_size;
            upper_limit = frame_parms->ofdm_symbol_size - start_sc;
          }

#ifdef USE128BIT            
            simde__m128i *txF = (simde__m128i *)&txdataF_precoding[layer][l_symbol][start_sc];

            simde__m128i *txl = (simde__m128i *)&tx_layer[cur_re];
            simde__m128i amp64 = simde_mm_set1_epi16(amp);
            int i;
            for (i = 0; i < (upper_limit >> 2); i++) {
              const simde__m128i txL = simde_mm_loadu_si128(txl + i);
              simde_mm_storeu_si128(txF + i, simde_mm_mulhrs_epi16(amp64, txL));
#ifdef DEBUG_DLSCH_MAPPING
              for (int j = 0; j < 4; j++)
                printf("re %u\t l %d \t k %d \t txdataF: %d %d\n",
                       cur_re + 4 * i + j,
                       l_symbol,
                       start_sc + 4 * i + j,
                       txdataF_precoding[layer][l_symbol][start_sc + 4 * i + j].r,
                       txdataF_precoding[layer][l_symbol][start_sc + 4 * i + j].i);
#endif
              /* handle this, mute RE */
              /*else {
                txdataF_precoding[layer][((l*frame_parms->ofdm_symbol_size + k)<<1) ] = 0;
                txdataF_precoding[layer][((l*frame_parms->ofdm_symbol_size + k)<<1) + 1] = 0;
                }*/
            }
            if (i * 4 != upper_limit) {
              c16_t *txFc = &txdataF_precoding[layer][l_symbol][start_sc];
              c16_t *txlc = &tx_layer[cur_re];
              for (i = (upper_limit >> 2) << 2; i < upper_limit; i++) {
                txFc[i].r = (((txlc[i].r * amp) >> 14) + 1) >> 1;
                txFc[i].i = (((txlc[i].i * amp) >> 14) + 1) >> 1;
#ifdef DEBUG_DLSCH_MAPPING
                printf("re %u\t l %d \t k %d \t txdataF: %d %d\n", cur_re + i, l_symbol, start_sc + i, txFc[i].r, txFc[i].i);
#endif
              }
            }
            cur_re += upper_limit;
            if (remaining_re > 0) {
              txF = (simde__m128i *)&txdataF_precoding[layer][l_symbol];
              txl = (simde__m128i *)&tx_layer[cur_re];
              int i;
              for (i = 0; i < (remaining_re >> 2); i++) {
                const simde__m128i txL = simde_mm_loadu_si128(txl + i);
                simde_mm_storeu_si128(txF + i, simde_mm_mulhrs_epi16(amp64, txL));
#ifdef DEBUG_DLSCH_MAPPING
                for (int j = 0; j < 4; j++)
                  printf("re %u\t l %d \t k %d \t txdataF: %d %d\n",
                         cur_re + 4 * i + j,
                         l_symbol,
                         4 * i + j,
                         txdataF_precoding[layer][l_symbol][4 * i + j].r,
                         txdataF_precoding[layer][l_symbol][4 * i + j].i);
#endif
                /* handle this, mute RE */
                 /*else {
                   txdataF_precoding[layer][((l*frame_parms->ofdm_symbol_size + k)<<1)    ] = 0;
                   txdataF_precoding[layer][((l*frame_parms->ofdm_symbol_size + k)<<1) + 1] = 0;
                   }*/
              } // RE loop, second part
              if (i * 4 != remaining_re) {
                c16_t *txFc = txdataF_precoding[layer][l_symbol];
                c16_t *txlc = &tx_layer[cur_re];
                for (i = (remaining_re >> 2) << 2; i < remaining_re; i++) {
                  txFc[i].r = (((txlc[i].r * amp) >> 14) + 1) >> 1;
                  txFc[i].i = (((txlc[i].i * amp) >> 14) + 1) >> 1;
#ifdef DEBUG_DLSCH_MAPPING
                  printf("re %u\t l %d \t k %d \t txdataF: %d %d\n", cur_re + i, l_symbol, i, txFc[i].r, txFc[i].i);
#endif
                }
              }
            } // remaining_re > 0
#else
            simde__m256i *txF = (simde__m256i *)&txdataF_precoding[layer][l_symbol][start_sc];

            simde__m256i *txl = (simde__m256i *)&tx_layers[layer][cur_re];
            simde__m256i amp64 = simde_mm256_set1_epi16(amp);
            int i;
            for (i = 0; i < (upper_limit >> 3); i++) {
              const simde__m256i txL = simde_mm256_loadu_si256(txl + i);
              simde_mm256_storeu_si256(txF + i, simde_mm256_mulhrs_epi16(amp64, txL));
#ifdef DEBUG_DLSCH_MAPPING
              for (int j = 0; j < 8; j++)
                printf("re %u\t l %d\t layer %d\t k %d \t txL %d %d txdataF: %d %d\n",
                       cur_re + 8 * i + j,
                       l_symbol,layer,
                       start_sc + 8 * i + j,
                       ((c16_t*)&txL)[j].r,
                       ((c16_t*)&txL)[j].i,
                       txdataF_precoding[layer][l_symbol][start_sc + 8 * i + j].r,
                       txdataF_precoding[layer][l_symbol][start_sc + 8 * i + j].i);
#endif
              /* handle this, mute RE */
              /*else {
                txdataF_precoding[layer][((l*frame_parms->ofdm_symbol_size + k)<<1) ] = 0;
                txdataF_precoding[layer][((l*frame_parms->ofdm_symbol_size + k)<<1) + 1] = 0;
                }*/
            }
            if (i * 8 != upper_limit) {
              c16_t *txFc = &txdataF_precoding[layer][l_symbol][start_sc];
              c16_t *txlc = &tx_layers[layer][cur_re];
              for (i = (upper_limit >> 3) << 3; i < upper_limit; i++) {
                txFc[i].r = (((txlc[i].r * amp) >> 14) + 1) >> 1;
                txFc[i].i = (((txlc[i].i * amp) >> 14) + 1) >> 1;
#ifdef DEBUG_DLSCH_MAPPING
                printf("re %u\t l %d\t layer %d \t k %d \t txL %d %d txdataF: %d %d\n", cur_re + i, l_symbol, layer, start_sc + i, txlc[i].r,txlc[i].i,txFc[i].r, txFc[i].i);
#endif
              }
            }
            cur_re += upper_limit;
            if (remaining_re > 0) {
              txF = (simde__m256i *)&txdataF_precoding[layer][l_symbol];
              txl = (simde__m256i *)&tx_layers[layer][cur_re];
              int i;
              for (i = 0; i < (remaining_re >> 3); i++) {
                const simde__m256i txL = simde_mm256_loadu_si256(txl + i);
                simde_mm256_storeu_si256(txF + i, simde_mm256_mulhrs_epi16(amp64, txL));
#ifdef DEBUG_DLSCH_MAPPING
                for (int j = 0; j < 8; j++)
                  printf("re %u\t l %d\t layer %d\t k %d \t txdataF: %d %d\n",
                         cur_re + 8 * i + j,
                         l_symbol, layer,
                         8 * i + j,
                         txdataF_precoding[layer][l_symbol][8 * i + j].r,
                         txdataF_precoding[layer][l_symbol][8 * i + j].i);
#endif
                /* handle this, mute RE */
                 /*else {
                   txdataF_precoding[layer][((l*frame_parms->ofdm_symbol_size + k)<<1)    ] = 0;
                   txdataF_precoding[layer][((l*frame_parms->ofdm_symbol_size + k)<<1) + 1] = 0;
                   }*/
              } // RE loop, second part
              if (i * 8 != remaining_re) {
                c16_t *txFc = txdataF_precoding[layer][l_symbol];
                c16_t *txlc = &tx_layers[layer][cur_re];
                for (i = (remaining_re >> 3) << 3; i < remaining_re; i++) {
                  txFc[i].r = (((txlc[i].r * amp) >> 14) + 1) >> 1;
                  txFc[i].i = (((txlc[i].i * amp) >> 14) + 1) >> 1;
#ifdef DEBUG_DLSCH_MAPPING
                  printf("re %u\t l %d\t layer %d\t k %d \t txdataF: %d %d\n", cur_re + i, l_symbol, layer, i, txFc[i].r, txFc[i].i);
#endif
                }
              }
            } // remaining_re > 0
#endif
            cur_re += remaining_re;
        } // no DMRS/PTRS in symbol
      } // layer loop
      cur_re0 = cur_re;
    } // symbol loop
    stop_meas(&gNB->dlsch_resource_mapping_stats);

    ///Layer Precoding and Antenna port mapping
    // tx_layers 1-8 are mapped on antenna ports 1000-1007
    // The precoding info is supported by nfapi such as num_prgs, prg_size, prgs_list and pm_idx
    // The same precoding matrix is applied on prg_size RBs, Thus
    //        pmi = prgs_list[rbidx/prg_size].pm_idx, rbidx =0,...,rbSize-1
    // The Precoding matrix:
    // The Codebook Type I
    start_meas(&gNB->dlsch_precoding_stats);
    nfapi_nr_tx_precoding_and_beamforming_t *pb = &rel15->precodingAndBeamforming;
    // beam number in multi-beam scenario (concurrent beams)
    int bitmap = SL_to_bitmap(rel15->StartSymbolIndex, rel15->NrOfSymbols);
    int beam_nb = beam_index_allocation(pb->prgs_list[0].dig_bf_interface_list[0].beam_idx,
                                        &gNB->common_vars,
                                        slot,
                                        frame_parms->symbols_per_slot,
                                        bitmap);

    c16_t **txdataF = gNB->common_vars.txdataF[beam_nb];

    for (int ant = 0; ant < frame_parms->nb_antennas_tx; ant++) {
      for (int l_symbol = rel15->StartSymbolIndex; l_symbol < rel15->StartSymbolIndex + rel15->NrOfSymbols; l_symbol++) {
        uint16_t subCarrier = start_sc;
        const size_t txdataF_offset_per_symbol = l_symbol * frame_parms->ofdm_symbol_size + txdataF_offset;
        int rb = 0;
        while(rb < rel15->rbSize) {
          //get pmi info
          const int pmi = (pb->prg_size > 0) ? (pb->prgs_list[(int)rb / pb->prg_size].pm_idx) : 0;
          const int pmi2 = (rb < (rel15->rbSize - 1) && pb->prg_size > 0) ? (pb->prgs_list[(int)(rb+1)/pb->prg_size].pm_idx) : -1;

          // If pmi of next RB and pmi of current RB are the same, we do 2 RB in a row
          // if pmi differs, or current rb is the end (rel15->rbSize - 1), than we do 1 RB in a row
          const int rb_step = pmi == pmi2 ? 2 : 1;
          const int re_cnt  = NR_NB_SC_PER_RB * rb_step;

          if (pmi == 0) {//unitary Precoding
            if (subCarrier + re_cnt <= frame_parms->ofdm_symbol_size) { // RB does not cross DC
              if (ant < rel15->nrOfLayers)
                 memcpy(&txdataF[ant][txdataF_offset_per_symbol + subCarrier],
                        &txdataF_precoding[ant][l_symbol][subCarrier],
                        re_cnt * sizeof(**txdataF));
              else
                 memset(&txdataF[ant][txdataF_offset_per_symbol + subCarrier],
                        0,
                        re_cnt * sizeof(**txdataF));
            } else { // RB does cross DC
              const int neg_length = frame_parms->ofdm_symbol_size - subCarrier;
              const int pos_length = re_cnt - neg_length;
              if (ant < rel15->nrOfLayers) {
                memcpy(&txdataF[ant][txdataF_offset_per_symbol + subCarrier],
                       &txdataF_precoding[ant][l_symbol][subCarrier],
                       neg_length * sizeof(**txdataF));
                memcpy(&txdataF[ant][txdataF_offset_per_symbol],
                       &txdataF_precoding[ant][l_symbol],
                       pos_length * sizeof(**txdataF));
              } else {
                 memset(&txdataF[ant][txdataF_offset_per_symbol + subCarrier],
                        0,
                        neg_length * sizeof(**txdataF));
                 memset(&txdataF[ant][txdataF_offset_per_symbol],
                        0,
                        pos_length * sizeof(**txdataF));
              }
            }
            subCarrier += re_cnt;
            if (subCarrier >= frame_parms->ofdm_symbol_size) {
              subCarrier -= frame_parms->ofdm_symbol_size;
            }
          }
          else { // non-unitary Precoding
            AssertFatal(frame_parms->nb_antennas_tx > 1, "No precoding can be done with a single antenna port\n");
            //get the precoding matrix weights:
            nfapi_nr_pm_pdu_t *pmi_pdu = &gNB->gNB_config.pmi_list.pmi_pdu[pmi - 1]; // pmi 0 is identity matrix
            AssertFatal(pmi == pmi_pdu->pm_idx, "PMI %d doesn't match to the one in precoding matrix %d\n",
                        pmi, pmi_pdu->pm_idx);
            AssertFatal(ant < pmi_pdu->num_ant_ports, "Antenna port index %d exceeds precoding matrix AP size %d\n",
                        ant, pmi_pdu->num_ant_ports);
            AssertFatal(rel15->nrOfLayers == pmi_pdu->numLayers, "Number of layers %d doesn't match to the one in precoding matrix %d\n",
                        rel15->nrOfLayers, pmi_pdu->numLayers);
            if ((subCarrier + re_cnt) < frame_parms->ofdm_symbol_size) { // within ofdm_symbol_size, use SIMDe
              nr_layer_precoder_simd(rel15->nrOfLayers,
                                     NR_SYMBOLS_PER_SLOT,
                                     frame_parms->ofdm_symbol_size,
                                     txdataF_precoding,
                                     ant,
                                     pmi_pdu,
                                     l_symbol,
                                     subCarrier,
                                     re_cnt,
                                     &txdataF[ant][txdataF_offset_per_symbol]);
              subCarrier += re_cnt;
            } else { // crossing ofdm_symbol_size, use simple arithmetic operations
              for (int i = 0; i < re_cnt; i++) {
                txdataF[ant][txdataF_offset_per_symbol + subCarrier] =
                    nr_layer_precoder_cm(rel15->nrOfLayers,
                                         NR_SYMBOLS_PER_SLOT,
                                         frame_parms->ofdm_symbol_size,
                                         txdataF_precoding,
                                         ant,
                                         pmi_pdu,
                                         l_symbol,
                                         subCarrier);
#ifdef DEBUG_DLSCH_MAPPING
                printf("antenna %d\t l %d \t subCarrier %d \t txdataF: %d %d\n",
                       ant,
                       l_symbol,
                       subCarrier,
                       txdataF[ant][l_symbol * frame_parms->ofdm_symbol_size + subCarrier + txdataF_offset].r,
                       txdataF[ant][l_symbol * frame_parms->ofdm_symbol_size + subCarrier + txdataF_offset].i);
#endif
                if (++subCarrier >= frame_parms->ofdm_symbol_size) {
                  subCarrier -= frame_parms->ofdm_symbol_size;
                }
              }
            } // else{ // crossing ofdm_symbol_size, use simple arithmetic operations
          } // else { // non-unitary Precoding

          rb += rb_step;
        } // RB loop: while(rb < rel15->rbSize)
      } // symbol loop
    } // port loop

    stop_meas(&gNB->dlsch_precoding_stats);

  }// dlsch loop
}

void dump_pdsch_stats(FILE *fd,PHY_VARS_gNB *gNB) {
  for (int i = 0; i < MAX_MOBILES_PER_GNB; i++) {
    NR_gNB_PHY_STATS_t *stats = &gNB->phy_stats[i];
    if (stats->active && stats->frame != stats->dlsch_stats.dump_frame) {
      stats->dlsch_stats.dump_frame = stats->frame;
      fprintf(fd,
              "DLSCH RNTI %x: current_Qm %d, current_RI %d, total_bytes TX %d\n",
              stats->rnti,
              stats->dlsch_stats.current_Qm,
              stats->dlsch_stats.current_RI,
              stats->dlsch_stats.total_bytes_tx);
    }
  }
}
