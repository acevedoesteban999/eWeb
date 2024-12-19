#include "eWeb.h"

httpd_handle_t WebServer = NULL; 

bool get_bool_urlencoded_request(const char *input, const char *key, bool *value){
    int bool_int;
    if(get_int_urlencoded_request(input,key,&bool_int)){
        *value = (bool) bool_int;
        return true;
    }

    char bool_str[6];
    if(get_string_urlencoded_request(input,key,bool_str,6)){
        if(strcmp(bool_str,"true")){
            *value = true;
            return true;
        }
        else if(strcmp(bool_str,"false")){
            *value = false;
            return true;
        };
    }
    return false;
}

bool get_string_urlencoded_request(const char *input, const char *key, char *value, uint max_size) {
    char pattern[50];
    snprintf(pattern, sizeof(pattern), "%s=", key);

    char *pos = strstr(input, pattern);
    if (pos) {
        pos += strlen(pattern); 
        char *end = strchr(pos, '&'); 
        size_t length = end ? (size_t)(end - pos) : strlen(pos);

        if (length >= max_size) 
            return false;

        strncpy(value, pos, length);
        value[length] = '\0';
        return true;
    }

    return false;
}

bool get_int_urlencoded_request(const char *input, const char *key, int *value) {
    char pattern[50];
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


bool get_float_urlencoded_request(const char *input, const char *key, float *value){
    char pattern[50];
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


// bool get_uint_json_request(const char *input, const char *key, uint *value) {
//     char pattern[50];
//     snprintf(pattern, sizeof(pattern), "\"%s\":", key);

//     char *pos = strstr(input, pattern);
//     if (pos) {
//         pos += strlen(pattern);
//         if (sscanf(pos, "%u", value) > 0) {
//             return true;
//         }
//     }
//     return false;
// }

// bool get_int_json_request(const char *input, const char *key, int *value) {
//     char pattern[50];
//     snprintf(pattern, sizeof(pattern), "\"%s\":", key);

//     char *pos = strstr(input, pattern);
//     if (pos) {
//         pos += strlen(pattern);
//         if (sscanf(pos, "%i", value) > 0) {
//             return true;
//         }
//     }
//     return false;
// }



// bool get_float_json_request(const char *input, const char *key, float *value){
//     char pattern[50];
//     snprintf(pattern, sizeof(pattern), "\"%s\":", key);

//     char *pos = strstr(input, pattern);
//     if (pos) {
//         pos += strlen(pattern);
//         if (sscanf(pos, "%f", value) > 0) {
//             return true;
//         }
//     }
//     return false;
// }

// Static  (GET)
esp_err_t static_handler(httpd_req_t *req) {
    static_ctx_handler*ctx = (static_ctx_handler *)req->user_ctx;
    httpd_resp_set_type(req, ctx->resp_type);
    httpd_resp_set_hdr(req, "Cache-Control", "public, max-age=86400");
    httpd_resp_send(req, ctx->asm_start, ctx->asm_end - ctx->asm_start );
    return ESP_OK;
}

void insert_ctx_into_uri(uri_ctx_hanlder*uri){
    if(uri->has_ctx)
        uri->uri.user_ctx = &uri->static_ctx;
}

//Insert Handlers into WebServer
void set_custom_uri_handlers(uri_ctx_hanlder*uri_ctx_handlers,size_t uris_size){

    for(unsigned i =0; i < uris_size; i++)
        httpd_register_uri_handler(WebServer, &uri_ctx_handlers[i].uri);
}

//char *buff = malloc(req->content_len + 1);
bool get_all_data_request(httpd_req_t *req,char*buffer){
        
        int ret, remaining = req->content_len;
        ret = httpd_req_recv(req, buffer, remaining);
        if (ret <= 0) { 
            if (ret == HTTPD_SOCK_ERR_TIMEOUT)
                httpd_resp_send_408(req); 
            return false;
        }
        
        buffer[ret] = '\0';
        return true;
}

void start_webserver(uint16_t max_uri) {

    wifi_init_softap();
    
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = max_uri;
    
    ESP_ERROR_CHECK(httpd_start(&WebServer, &config));
}
