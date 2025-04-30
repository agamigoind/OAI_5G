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

/*! \file openair2/COMMON/mqtt_paramdef.h
 * \brief definition of configuration parameters for MQTT
 * \author
 * \date 2022
 * \version 0.1
 * \company EURECOM
 * \email:
 * \note
 * \warning
 */

#ifndef __MQTT_PARAMDEF__H__
#define __MQTT_PARAMDEF__H__

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/* PRS configuration section names */
#define CONFIG_STRING_MQTT_CONFIG                           "mqtt_config"


/* Global parameters */

/* MQTT parameters */
#define CONFIG_STRING_MQTT_BROKER_ADDR                       "MqttBrokerAddr"
#define CONFIG_STRING_MQTT_TOPIC_NAME                        "MqttTopicName"
#define CONFIG_STRING_MQTT_CLIENT_ID                         "MqttClientId"
#define CONFIG_STRING_MQTT_TRP_ID                            "MqttTrpId"
/*----------------------------------------------------------------------------------------------------------------------------------------------------*/
/*                                            PRS configuration parameters                                                                             */
/*   optname                                         helpstr   paramflags    XXXptr              defXXXval                  type           numelt     */
/*----------------------------------------------------------------------------------------------------------------------------------------------------*/
#define MQTT_PARAMS_DESC { \
{CONFIG_STRING_MQTT_BROKER_ADDR,                          NULL,      0,         strptr:NULL,           defstrval:NULL,         TYPE_STRING,     0},          \
{CONFIG_STRING_MQTT_TOPIC_NAME,                           NULL,      0,         strptr:NULL,           defstrval:NULL,         TYPE_STRING,     0},          \
{CONFIG_STRING_MQTT_CLIENT_ID,                            NULL,      0,         strptr:NULL,           defstrval:NULL,         TYPE_STRING,     0},          \
{CONFIG_STRING_MQTT_TRP_ID,                               NULL,      0,         uptr:NULL,             defuintval:0,           TYPE_UINT,       0}           \
}

#define MQTT_BROKER_ADDR_IDX                              0
#define MQTT_TOPIC_NAME_IDX                               1
#define MQTT_CLIENT_ID_IDX                                2
#define MQTT_TRP_ID_IDX                                   3
/*----------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif

