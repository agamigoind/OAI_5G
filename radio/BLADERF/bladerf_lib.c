/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.
 * See the NOTICE file distributed with this work for additional information.
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "bladerf_lib.h"
#include "PHY/sse_intrin.h"

//! Number of BladeRF devices
int num_devices = 0;

/*!
 * BladeRF Initialization function
 *
 * \param device RF frontend parameters set by the application
 * \returns 0 on success
 */
int trx_brf_init(openair0_device *device) {
    return 0;
}

/*!
 * Start BladeRF
 * \param device Hardware to use
 * \returns 0 on success
 */
int trx_brf_start(openair0_device *device) {
    brf_state_t *brf = (brf_state_t *)device->priv;
    int status;

    brf->meta_tx.flags = 0;

    if ((status = bladerf_sync_config(brf->dev, BLADERF_MODULE_TX, BLADERF_FORMAT_SC16_Q11_META, brf->num_buffers, brf->buffer_size, brf->num_transfers, 100)) != 0) {
        fprintf(stderr, "Failed to configure TX sync interface: %s\n", bladerf_strerror(status));
        abort();
    }
    if ((status = bladerf_sync_config(brf->dev, BLADERF_MODULE_RX, BLADERF_FORMAT_SC16_Q11_META, brf->num_buffers, brf->buffer_size, brf->num_transfers, 100)) != 0) {
        fprintf(stderr, "Failed to configure RX sync interface: %s\n", bladerf_strerror(status));
        abort();
    }
    if ((status = bladerf_enable_module(brf->dev, BLADERF_MODULE_TX, true)) != 0) {
        fprintf(stderr, "Failed to enable TX module: %s\n", bladerf_strerror(status));
        abort();
    }
    if ((status = bladerf_enable_module(brf->dev, BLADERF_MODULE_RX, true)) != 0) {
        fprintf(stderr, "Failed to enable RX module: %s\n", bladerf_strerror(status));
        abort();
    }

    return 0;
}

/*!
 * Terminate BladeRF operation
 * \param device Hardware to use
 */
void trx_brf_end(openair0_device *device) {
    int status;
    brf_state_t *brf = (brf_state_t *)device->priv;

    if ((status = bladerf_enable_module(brf->dev, BLADERF_MODULE_RX, false)) != 0) {
        fprintf(stderr, "Failed to disable RX module: %s\n", bladerf_strerror(status));
    }
    if ((status = bladerf_enable_module(brf->dev, BLADERF_MODULE_TX, false)) != 0) {
        fprintf(stderr, "Failed to disable TX module: %s\n", bladerf_strerror(status));
    }
    bladerf_close(brf->dev);
}

/*!
 * Initialize Openair BladeRF device
 * \param device Hardware to use
 * \param openair0_cfg RF frontend parameters
 * \returns 0 on success
 */
int device_init(openair0_device *device, openair0_config_t *openair0_cfg) {
    int status;
    brf_state_t *brf = (brf_state_t *)malloc(sizeof(brf_state_t));
    memset(brf, 0, sizeof(brf_state_t));

    openair0_cfg->samples_per_packet = 2048;
    openair0_cfg->tx_sample_advance = 0;

    brf->num_buffers = 128;
    brf->buffer_size = (unsigned int)openair0_cfg->samples_per_packet;
    brf->num_transfers = 16;
    brf->rx_timeout_ms = 100;
    brf->tx_timeout_ms = 100;
    brf->sample_rate = (unsigned int)openair0_cfg->sample_rate;

    if ((status = bladerf_open(&brf->dev, "")) != 0) {
        fprintf(stderr, "Failed to open BladeRF device: %s\n", bladerf_strerror(status));
        free(brf);
        return -1;
    }

    printf("[BRF] Device initialized with sample rate %u\n", brf->sample_rate);

    device->Mod_id = num_devices++;
    device->type = BLADERF_DEV;
    device->trx_start_func = trx_brf_start;
    device->trx_end_func = trx_brf_end;
    device->priv = (void *)brf;

    return 0;
}

/*!
 * Handle BladeRF errors
 * \param status Error status
 * \returns Status
 */
int brf_error(int status) {
    fprintf(stderr, "[BRF] Error: %s\n", bladerf_strerror(status));
    return status;
}

