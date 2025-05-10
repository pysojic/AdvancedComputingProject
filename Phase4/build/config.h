#pragma once

/* default number of ticks to run */
#define CFG_DEFAULT_TICKS    10000

/* pointer mode: "shared", "pool", or "raw" */
#define CFG_DEFAULT_PTRMODE  "shared"

/* whether to build with alignas(64) in MarketData */
#define CFG_ENABLE_ALIGN64   1

/* whether to build with MemoryPool allocator on by default */
#define CFG_ENABLE_POOL       1

/* container layout: "multimap" or "flat" */
#define CFG_DEFAULT_LAYOUT   "multimap"
