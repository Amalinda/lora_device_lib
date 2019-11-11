/* Copyright (c) 2019 Cameron Harper
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * */

#ifndef __LORA_FRAME_H
#define __LORA_FRAME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* types **************************************************************/

enum lora_frame_type {
    FRAME_TYPE_JOIN_REQ = 0,
    FRAME_TYPE_JOIN_ACCEPT,
    FRAME_TYPE_DATA_UNCONFIRMED_UP,
    FRAME_TYPE_DATA_UNCONFIRMED_DOWN,
    FRAME_TYPE_DATA_CONFIRMED_UP,
    FRAME_TYPE_DATA_CONFIRMED_DOWN,    
    FRAME_TYPE_REJOIN_REQ
};

struct lora_frame_data {
    
    enum lora_frame_type type;
    
    uint32_t devAddr;
    uint16_t counter;
    bool ack;
    bool adr;
    bool adrAckReq;
    bool pending;

    const uint8_t *opts;
    uint8_t optsLen;

    uint8_t port;
    
    const uint8_t *data;
    uint8_t dataLen;
    
    uint32_t mic;
};

struct lora_frame_data_offset {

    uint8_t opts;
    uint8_t data;
};

enum lora_frame_rejoin_type {
        
    LORA_REJOIN_TYPE_1,
    LORA_REJOIN_TYPE_2,
    LORA_REJOIN_TYPE_3
};

struct lora_frame_rejoin_request {
    
    enum lora_frame_rejoin_type type;
    uint32_t netID;
    uint8_t devEUI[8U];
    uint16_t rjCount;    
    uint32_t mic;
};

struct lora_frame_join_request {
    
    uint8_t joinEUI[8U];
    uint8_t devEUI[8U];
    uint16_t devNonce;    
    uint32_t mic;
};

struct lora_frame_down {

    enum lora_frame_type type;
    
    /* join accept */
    uint32_t joinNonce;
    uint32_t netID;
    uint32_t devAddr;
    uint8_t rx1DataRateOffset;
    uint8_t rx2DataRate;
    uint8_t rxDelay;
    bool optNeg;
    uint8_t *cfList;
    uint8_t cfListLen;
    
    /* data */
    /*uint32_t devAddr;*/
    uint16_t counter;
    bool ack;
    bool adr;
    bool adrAckReq;
    bool pending;

    uint8_t *opts;
    uint8_t optsLen;

    uint8_t port;
    
    uint8_t *data;
    uint8_t dataLen;
    
    uint32_t mic;    
};

/* function prototypes ************************************************/

void LDL_Frame_updateMIC(void *msg, uint8_t len, uint32_t mic);
uint8_t LDL_Frame_putData(const struct lora_frame_data *f, void *out, uint8_t max, struct lora_frame_data_offset *off);
uint8_t LDL_Frame_putJoinRequest(const struct lora_frame_join_request *f, void *out, uint8_t max);
uint8_t LDL_Frame_putRejoinRequest(const struct lora_frame_rejoin_request *f, void *out, uint8_t max);
bool LDL_Frame_peek(const void *in, uint8_t len, enum lora_frame_type *type);
bool LDL_Frame_decode(struct lora_frame_down *f, void *in, uint8_t len);

uint8_t LDL_Frame_sizeofJoinAccept(bool withCFList);
uint8_t LDL_Frame_getPhyPayloadSize(uint8_t dataLen, uint8_t optsLen);
uint8_t LDL_Frame_phyOverhead(void);
uint8_t LDL_Frame_dataOverhead(void);

#ifdef __cplusplus
}
#endif

#endif
