/**
 * Copyright (c) 2014 - 2019, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/* CMK - removed from boards.h to allow more flexibility
   (including boards.h in app_config.h caused issues with nRF's 
    apply_old_config.h being included before sdk_config.h) */

#ifndef BOARDS_INC_H
#define BOARDS_INC_H

#include "nordic_common.h"

#if defined(BOARD_NRF6310)
  #include "nrf6310.h"
#elif defined(BOARD_PCA10000)
  #include "pca10000.h"
#elif defined(BOARD_PCA10001)
  #include "pca10001.h"
#elif defined(BOARD_PCA10002)
  #include "pca10000.h"
#elif defined(BOARD_PCA10003)
  #include "pca10003.h"
#elif defined(BOARD_PCA20006)
  #include "pca20006.h"
#elif defined(BOARD_PCA10028)
  #include "pca10028.h"
#elif defined(BOARD_PCA10031)
  #include "pca10031.h"
#elif defined(BOARD_PCA10036)
  #include "pca10036.h"
#elif defined(BOARD_PCA10040)
  #include "pca10040.h"
#elif defined(BOARD_PCA10056)
  #include "pca10056.h"
#elif defined(BOARD_PCA10100)
  #include "pca10100.h"
#elif defined(BOARD_PCA20020)
  #include "pca20020.h"
#elif defined(BOARD_PCA10059)
  #include "pca10059.h"
#elif defined(BOARD_WT51822)
  #include "wt51822.h"
#elif defined(BOARD_N5DK1)
  #include "n5_starterkit.h"
#elif defined (BOARD_D52DK1)
  #include "d52_starterkit.h"
#elif defined (BOARD_ARDUINO_PRIMO)
  #include "arduino_primo.h"
#elif defined (CUSTOM_BOARD_INC)
  #include STRINGIFY(CUSTOM_BOARD_INC.h)
#elif defined(BOARD_CUSTOM)
  #include "custom_board.h"
#else
#error "Board is not defined"
#endif

#endif // BOARDS_INC_H
