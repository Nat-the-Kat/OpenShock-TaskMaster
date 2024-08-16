#pragma once
#include <lwip/ip_addr.h>


// Used by WiFiClient to get DNS lookup
bool lookup_ip(const char *hostname, ip_addr_t &result, int timeout_ms = 5000);