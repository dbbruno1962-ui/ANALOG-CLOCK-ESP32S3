#pragma once

#include "config.h"

/* =====================================================
 * DEFAULT CALIBRATION
 * ===================================================== */

#define TEMP_OFFSET_DHT11 (0.0f)
#define HUM_OFFSET_DHT11  (0.0f)

/* =====================================================
 * EMILIA
 * T_err(-1.9f) um_err(+9.0f)
 * ===================================================== */

#ifdef USER_FIN_EMILIA

#undef TEMP_OFFSET_DHT11
#undef HUM_OFFSET_DHT11

#define TEMP_OFFSET_DHT11 (-1.9f)
#define HUM_OFFSET_DHT11  (+9.0f)

#endif

/* =====================================================
 * ANTONELLA
 * T_err(-3.3f) um_err(+7.0f)
 * ===================================================== */

#ifdef USER_FIN_ANTONELLA

#undef TEMP_OFFSET_DHT11
#undef HUM_OFFSET_DHT11

#define TEMP_OFFSET_DHT11 (-3.3f)
#define HUM_OFFSET_DHT11  (+7.0f)

#endif

/* =====================================================
 * TOMMASO
 * T_err(-1.4f) um_err(+3.0f)
 * ===================================================== */

#ifdef USER_FIN_TOMMASO

#undef TEMP_OFFSET_DHT11
#undef HUM_OFFSET_DHT11

#define TEMP_OFFSET_DHT11 (-1.4f)
#define HUM_OFFSET_DHT11  (+3.0f)

#endif

/* =====================================================
 * BRUNO
 * T_err(-2.3f) um_err(+1.2f)
 * ===================================================== */

#ifdef USER_FIN_BRUNO

#undef TEMP_OFFSET_DHT11
#undef HUM_OFFSET_DHT11

#define TEMP_OFFSET_DHT11 (-2.3f)
#define HUM_OFFSET_DHT11  (+1.2f)

#endif
