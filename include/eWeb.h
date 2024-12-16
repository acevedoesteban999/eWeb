#pragma once
#include "esp_http_server.h"
#include "esp_event.h"

#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include "eWifi.h"
#include "eStore.h"

typedef struct {
    const char* asm_start;
    const char* asm_end;
    char resp_type[20];

}static_ctx_handler;

typedef struct {
    httpd_uri_t uri;
    bool has_ctx;
    static_ctx_handler static_ctx;
}uri_ctx_hanlder;

extern httpd_handle_t WebServer;

bool get_string_urlencoded_request(const char * input, const char * key, char * value, uint size);

bool get_int_urlencoded_request(const char *input, const char *key, int *value);

bool get_float_urlencoded_request(const char *input, const char *key, float *value);

// bool get_string_json_request(const char *input, const char *key, char *value, uint size);

// bool get_uint_json_request(const char *input, const char *key, uint *value);

// bool get_int_json_request(const char *input, const char *key, int *value);

// bool get_float_json_request(const char *input, const char *key, float *value);

esp_err_t static_handler(httpd_req_t *req);

void insert_ctx_into_uri(uri_ctx_hanlder *uri);

void set_custom_uri_handlers(uri_ctx_hanlder *uri_ctx_handlers, size_t uris_size);

bool get_all_data_request(httpd_req_t *req, char *buffer);

void start_webserver(uint16_t max_uri);
