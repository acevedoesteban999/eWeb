#include "eWeb.h"

httpd_handle_t WebServer = NULL; 



bool get_int_param_value(const char *input, const char *key, int *value) {
    char pattern[20];
    snprintf(pattern, sizeof(pattern), "%s=", key);

    char *pos = strstr(input, pattern);
    if (pos) {
        pos += strlen(pattern);
        if (sscanf(pos, "%d", value) > 0) {
            return true;
        }
    }
    return false;
}

bool get_float_param_value(const char *input, const char *key, float *value){
    char pattern[20];
    snprintf(pattern, sizeof(pattern), "%s=", key);

    char *pos = strstr(input, pattern);
    if (pos) {
        pos += strlen(pattern);
        if (sscanf(pos, "%f", value) > 0) {
            return true;
        }
    }
    return false;
}

// Static  (GET)
esp_err_t static_handler(httpd_req_t *req) {
    static_ctx_handler*ctx = (static_ctx_handler *)req->user_ctx;
    httpd_resp_set_type(req, ctx->resp_type);
    httpd_resp_set_hdr(req, "Cache-Control", "public, max-age=86400");
    httpd_resp_send(req, ctx->asm_start, ctx->asm_end - ctx->asm_start );
    return ESP_OK;
}

//Insert Handlers into WebServer
void set_custom_uri_handlers(uri_ctx_hanlder*uri_ctx_handlers,size_t uris_size){

    for(unsigned i =0; i < uris_size; i++)
        httpd_register_uri_handler(WebServer, &uri_ctx_handlers[i].uri);
}

void start_webserver(uint16_t max_uri) {

    wifi_init_softap();
    
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = max_uri;
    
    ESP_ERROR_CHECK(httpd_start(&WebServer, &config));
}
