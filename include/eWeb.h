#pragma once
#include "esp_http_server.h"
#include "esp_event.h"

#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include "eWifi.h"
#include "eStore.h"
#include "eSTR.h"
#include "eFree.h"

#define MAX_CONTENT_SIZE 2048
#define SHUNK_SIZE 1024



#define EWEB_HANDLERS(handler_statics) \
    {{"/css/styles.min.css", HTTP_GET, handler_statics, NULL},true,{styles_min_css_asm_start, styles_min_css_asm_end, "text/css",NULL,NULL}}, \
    {{"/css/milligram/normalize.min.css", HTTP_GET, handler_statics, NULL},true,{normalize_min_css_asm_start, normalize_min_css_asm_end, "text/css",NULL,NULL}}, \
    {{"/js/index.min.js", HTTP_GET, handler_statics, NULL},true,{index_min_js_asm_start, index_min_js_asm_end, "text/javascript",NULL,NULL}} \
    


// Modificar las macros para usar eSTR
#define EWEB_GENERATE_REPLACEMENT_BUFFER(str_addr, efree_addr,format, ...) \
    do { \
        if (!estr_append_format(str_addr, true, format, __VA_ARGS__)) { \
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Internal Server Error"); \
            efree_free(efree_addr); \
            return ESP_FAIL; \
        } \
    } while (false)

#define EWEB_CHECK_INT_URLENCODED(req, buff, key, var_addrr, efree_addr) \
    do { \
        if (!eweb_get_int_urlencoded(buff, key, var_addrr)) { \
            eSTR error_str; \
            estr_init(&error_str); \
            estr_append_format(&error_str, true, "Error: Missing integer parameter: '%s'", key); \
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, error_str.ptr_char); \
            estr_free(&error_str); \
            efree_free(efree_addr); \
            return ESP_FAIL; \
        } \
    } while (0)

#define EWEB_CHECK_FLOAT_URLENCODED(req, buff, key, var_addrr , efree_addr ) \
    do { \
        if (!eweb_get_float_urlencoded(buff, key, var_addrr)) { \
            eSTR error_str; \
            estr_init(&error_str); \
            estr_append_format(&error_str, true, "Error: Missing float parameter: '%s'", key); \
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, error_str.ptr_char); \
            estr_free(&error_str); \
            efree_free(efree_addr); \
            return ESP_FAIL; \
        } \
    } while (0)

#define EWEB_CHECK_BOOL_URLENCODED(req, buff, key, var_addrr , efree_addr) \
    do { \
        if (!eweb_get_bool_urlencoded(buff, key, var_addrr)) { \
            eSTR error_str; \
            estr_init(&error_str); \
            estr_append_format(&error_str, true, "Error: Missing boolean parameter: '%s'", key); \
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, error_str.ptr_char); \
            estr_free(&error_str); \
            efree_free(efree_addr); \
            return ESP_FAIL; \
        } \
    } while (0)



#define EWEB_CHECK_STR_URLENCODED(req, input_buff, key, str_addr, efree_addr) \
    do { \
        if (!eweb_get_str_urlencoded(input_buff, key, str_addr)) { \
            eSTR error_str; \
            estr_init(&error_str); \
            estr_append_format(&error_str, true, "Error: Missing string parameter: '%s'", key); \
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, error_str.ptr_char); \
            estr_free(&error_str); \
            efree_free(efree_addr); \
            return ESP_FAIL; \
        } \
    } while (0)

#define EWEB_GET_DATA_REQUEST_STR(req, str_addr, efree_addr) \
    do { \
        if (!eweb_get_data_request_str(req, str_addr)) { \
            httpd_resp_send_err((req), HTTPD_500_INTERNAL_SERVER_ERROR, "Internal Server Error"); \
            efree_free(efree_addr); \
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

extern const char normalize_min_css_asm_start[] asm("_binary_normalize_min_css_start");
extern const char normalize_min_css_asm_end[] asm("_binary_normalize_min_css_end");

extern const char index_min_js_asm_start[] asm("_binary_index_min_js_start");
extern const char index_min_js_asm_end[] asm("_binary_index_min_js_end");

extern httpd_handle_t WebServer;


bool eweb_get_bool_urlencoded(const char *input, const char *key, bool *value);

bool eweb_get_string_urlencoded(const char *input, const char *key, char *value, uint size);

bool eweb_get_str_urlencoded(const char *input, const char *key, eSTR *str);

bool eweb_get_int_urlencoded(const char *input, const char *key, int *value);

int eweb_isvalidchar(char c);

bool eweb_get_float_urlencoded(const char *input, const char *key, float *value);

esp_err_t eweb_send_resp_buff(httpd_req_t *req, const char *buff, size_t buff_len);

// bool get_string_json_request(const char *input, const char *key, char *value, uint size);

// bool get_uint_json_request(const char *input, const char *key, uint *value);

// bool get_int_json_request(const char *input, const char *key, int *value);

// bool get_float_json_request(const char *input, const char *key, float *value);

esp_err_t eweb_send_resp_ui_str(httpd_req_t *req, eSTR* str);


esp_err_t eweb_static_html_handler(httpd_req_t *req);

esp_err_t eweb_static_handler(httpd_req_t *req);

void eweb_insert_ctx_into_uri(uri_ctx_hanlder *uri);

void eweb_set_uri_hanlders(uri_ctx_hanlder *uri_ctx_handlers, size_t uris_size);

bool eweb_get_data_request_str(httpd_req_t *req, eSTR *str);

void eweb_init(uint16_t max_uri);

void eweb_preapare_uri_hanlders(uri_ctx_hanlder *static_uris, size_t uri_handler_len);

bool eweb_check_condicional_function(httpd_req_t *req);

esp_err_t eweb_call_excecution_function(httpd_req_t *req);

bool eweb_add_str_urlencoded(eSTR *str, const char *key, const char *value, bool ampersand, bool is_optimized_for_memory);