/*#pragma once

//#include <Arduino.h>
#include <time.h>
#include <lwip/apps/sntp.h>
#include "include/dns/lookup.h"

class NTPClass {
public:
    NTPClass() { }

    ~NTPClass() {
        sntp_stop();
    }

    void begin(ip_addr_t server, int timeout = 3600) {
        (void) timeout;
        sntp_stop();
        if (server != 0) {
            sntp_setserver(0, &server);
            sntp_setoperatingmode(SNTP_OPMODE_POLL);
            sntp_init();
            _running = true;
        }
    }

    void begin(ip_addr_t s1, ip_addr_t s2, int timeout = 3600) {
        (void) timeout;
        sntp_stop();
        if (s1.isSet()) {
            sntp_setserver(0, &s1);
        }
        if (s2.isSet()) {
            sntp_setserver(1, &s2);
        }
        sntp_setoperatingmode(SNTP_OPMODE_POLL);
        sntp_init();
        _running = true;
    }

    void begin(const char *server, int timeout = 3600) {
        ip_addr_t addr;
        if (get_ip_by_name(server, addr)) {
            begin(addr, timeout);
        }
        _running = true;
    }

    void begin(const char *s1, const char *s2, int timeout = 3600) {
        ip_addr_t a1, a2;
        if (get_ip_by_name(s1, a1)) {
            if (get_ip_by_name(s2, a2)) {
                begin(a1, a2, timeout);
            } else {
                begin(a1, timeout);
            }
        }
        _running = true;
    }

    bool waitSet(uint32_t timeout = 10000) {
        return waitSet(nullptr, timeout);
    }

    bool waitSet(void (*cb)(), uint32_t timeout = 10000) {
        if (!running()) {
            begin("pool.ntp.org");
        }
        uint32_t start = millis();
        while ((time(nullptr) < 10000000) && (millis() - start < timeout)) {
            delay(100);
            if (cb) {
                cb();
            }
        }
        return time(nullptr) < 10000000;
    }

    bool running() {
        return _running;
    }

private:
    bool _running = false;
};

extern NTPClass NTP;*/