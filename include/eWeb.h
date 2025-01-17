#pragma once
#include "esp_http_server.h"
#include "esp_event.h"

#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include "eWifi.h"
#include "eStore.h"

#define MAX_CONTENT_SIZE 2048
#define SHUNK_SIZE 1024



#define EWEB_HANDLERS(handler_statics) \
    {{"/css/styles.min.css", HTTP_GET, handler_statics, NULL},true,{styles_min_css_asm_start, styles_min_css_asm_end, "text/css",NULL,NULL}}, \
    {{"/css/milligram/milligram.min.css", HTTP_GET, handler_statics, NULL},true,{milligram_min_css_asm_start, milligram_min_css_asm_end, "text/css",NULL,NULL}}, \
    {{"/css/milligram/normalize.min.css", HTTP_GET, handler_statics, NULL},true,{normalize_min_css_asm_start, normalize_min_css_asm_end, "text/css",NULL,NULL}}, \
    {{"/js/index.min.js", HTTP_GET, handler_statics, NULL},true,{index_min_js_asm_start, index_min_js_asm_end, "text/javascript",NULL,NULL}} \
    

//-2 ? Why?
#define BYTES_END_BUFFER 2  

#define EWEB_GENERATE_REPLACEMENT_BUFFER(_buffer, _format, ...)  \
    do { \
        size_t buffer_size = snprintf(NULL, 0, _format, __VA_ARGS__); \
        _buffer = calloc(buffer_size + 1, sizeof(char)); \
        if (_buffer == NULL) { \
            return ESP_FAIL; \
        } \
        snprintf(_buffer, buffer_size + 1, _format, __VA_ARGS__); \
         \
        buffer[ strlen(buffer) - BYTES_END_BUFFER ] = '\0'; \
    } while (false)


#define EWEB_CHECK_PARAMETER_URLENCODED(req,function,buff, key, var) \
    do { \
        if (!(function(buff, key, &var))) { \
            char error_msg[100]; \
            snprintf(error_msg, sizeof(error_msg), "Error: Missing parameter: '%s'", key); \
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, error_msg); \
            return ESP_FAIL; \
        } \
    } while (0)

typedef struct {
    const char* asm_start;
    const char* asm_end;
    char resp_type[20];
    esp_err_t (*uri_execution_function)(httpd_req_t *req);
    bool (*uri_condicional_function)(httpd_req_t *req);
}static_ctx_handler;

typedef struct {
    httpd_uri_t uri;
    bool has_ctx;
    static_ctx_handler static_ctx;
}uri_ctx_hanlder;

extern const char styles_min_css_asm_start[] asm("_binary_styles_min_css_start");
extern const char styles_min_css_asm_end[] asm("_binary_styles_min_css_end");

extern const char milligram_min_css_asm_start[] asm("_binary_milligram_min_css_start");
extern const char milligram_min_css_asm_end[] asm("_binary_milligram_min_css_end");

extern const char normalize_min_css_asm_start[] asm("_binary_normalize_min_css_start");
extern const char normalize_min_css_asm_end[] asm("_binary_normalize_min_css_end");

extern const char index_min_js_asm_start[] asm("_binary_index_min_js_start");
extern const char index_min_js_asm_end[] asm("_binary_index_min_js_end");

extern httpd_handle_t WebServer;


bool eweb_get_bool_urlencoded(const char *input, const char *key, bool *value);

bool eweb_get_string_urlencoded(const char *input, const char *key, char *value, uint size);

bool eweb_get_int_urlencoded(const char *input, const char *key, int *value);

bool eweb_get_float_urlencoded(const char *input, const char *key, float *value);

// bool get_string_json_request(const char *input, const char *key, char *value, uint size);

// bool get_uint_json_request(const char *input, const char *key, uint *value);

// bool get_int_json_request(const char *input, const char *key, int *value);

// bool get_float_json_request(const char *input, const char *key, float *value);

esp_err_t eweb_send_resp_try_chunk(httpd_req_t *req, char *buff, size_t buff_len);

esp_err_t eweb_static_html_handler(httpd_req_t *req);

esp_err_t eweb_static_handler(httpd_req_t *req);

void eweb_insert_ctx_into_uri(uri_ctx_hanlder *uri);

void eweb_set_uri_hanlders(uri_ctx_hanlder *uri_ctx_handlers, size_t uris_size);

bool eweb_get_all_data_request(httpd_req_t *req, char *buffer);

void eweb_init(uint16_t max_uri);

void eweb_preapare_uri_hanlders(uri_ctx_hanlder *static_uris, size_t uri_handler_len);

esp_err_t eweb_check_condicional_function(httpd_req_t *req);

esp_err_t eweb_call_excecution_function(httpd_req_t *req);
