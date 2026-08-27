#include "adapters/BlynkConfig.h"
#include <Blynk/BlynkApi.h> 
#include "BlynkAdapter.h"
#include "ports/IRepoPort.h"


BLYNK_WRITE(V0) { if (g_blynkAdapter) g_blynkAdapter->handleVirtualPin(1, param.asInt()); }
BLYNK_WRITE(V1) { if (g_blynkAdapter) g_blynkAdapter->handleVirtualPin(2, param.asInt()); }
BLYNK_WRITE(V2) { if (g_blynkAdapter) g_blynkAdapter->handleVirtualPin(3, param.asInt()); }
BLYNK_WRITE(V3) { if (g_blynkAdapter) g_blynkAdapter->handleVirtualPin(4, param.asInt()); }
BLYNK_WRITE(V4) { if (g_blynkAdapter) g_blynkAdapter->handleVirtualPin(5, param.asInt()); }
BLYNK_WRITE(V5) { if (g_blynkAdapter) g_blynkAdapter->handleVirtualPin(6, param.asInt()); }
BLYNK_WRITE(V6) { if (g_blynkAdapter) g_blynkAdapter->handleVirtualPin(7, param.asInt()); }
BLYNK_WRITE(V7) { if (g_blynkAdapter) g_blynkAdapter->handleVirtualPin(8, param.asInt()); }