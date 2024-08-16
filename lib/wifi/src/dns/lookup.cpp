#include "include/dns/lookup.h"

#include <lwip/timeouts.h>
#include <lwip/dns.h>
#include <pico/cyw43_arch.h>

static volatile bool dns_lookup_pending = false;

void lookup_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg) {
    (void) name;
    if (!dns_lookup_pending){
        return;
    }
    if (ipaddr){
        *(ip_addr_t *)callback_arg = *ipaddr;
    }
    dns_lookup_pending = false; // resume hostByName
}

bool lookup_ip(const char* hostname, ip_addr_t& result, int timeout_ms) {
  ip_addr_t temp;//only gets used if the ip address is already cached
  err_t err = dns_gethostbyname(hostname, &temp, lookup_callback, &result);
  if(err == ERR_OK){
    result = temp;
  }
  else if(err == ERR_INPROGRESS){
    dns_lookup_pending = true;
    uint32_t now = to_ms_since_boot(get_absolute_time());
    while (now + timeout_ms > to_ms_since_boot(get_absolute_time()) && dns_lookup_pending){
      sleep_ms(1);
    }
    dns_lookup_pending = false;
    if(result.addr != 0){
      err = ERR_OK;
    }
  }
  if(err == ERR_OK){
    return 1;
  }
  return 0;
}
