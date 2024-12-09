#pragma once
#include "esp_http_server.h"
#include "esp_event.h"

#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include "eWifi.h"
#include "eStore.h"

#define BUFF_HTTP_RECV_LEN 150

typedef struct {
    const char* asm_start;
    const char* asm_end;
    char resp_type[20];

}static_ctx_handler;

typedef struct {
    httpd_uri_t uri;
    static_ctx_handler static_ctx;

}uri_ctx_hanlder;

extern httpd_handle_t WebServer;

bool get_int_param_value(const char *input, const char *key, int *value);

bool get_float_param_value(const char *input, const char *key, float *value);

esp_err_t static_handler(httpd_req_t *req);

void set_custom_uri_handlers(uri_ctx_hanlder *uri_ctx_handlers, size_t uris_size);

void start_webserver(uint16_t max_uri);
