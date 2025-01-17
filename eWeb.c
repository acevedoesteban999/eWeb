#include "eWeb.h"

httpd_handle_t WebServer = NULL; 

bool eweb_get_bool_urlencoded(const char *input, const char *key, bool *value){
    int bool_int;
    if(eweb_get_int_urlencoded(input,key,&bool_int)){
        *value = (bool) bool_int;
        return true;
    }
    char bool_str[6];
    if(eweb_get_string_urlencoded(input,key,bool_str,6)){
        if(strcmp(bool_str,"true") == 0){
            *value = true;
            return true;
        }
        else if(strcmp(bool_str,"false") == 0){
            *value = false;
            return true;
        };
    }
    return false;
}

bool eweb_get_string_urlencoded(const char *input, const char *key, char *value, uint max_size) {
    char pattern[50];
    snprintf(pattern, sizeof(pattern), "%s=", key);
    char *pos = strstr(input, pattern);
    if (pos) {
        pos += strlen(pattern); 
        char *end = strchr(pos, '&'); 
        size_t len = end ? (size_t)(end - pos) : strlen(pos);
        if (len <= 0 || len >= max_size) 
            return false;
        strncpy(value, pos, len);
        value[len] = '\0';
        return true;
    }
    return false;
}

bool eweb_get_int_urlencoded(const char *input, const char *key, int *value) {
    char pattern[50];
    snprintf(pattern, sizeof(pattern), "%s=", key);

    char *pos = strstr(input, pattern);
    if (pos) {
        pos += strlen(pattern);
        if (sscanf(pos, "%i", value) > 0) {
            return true;
        }
    }
    return false;
}


bool eweb_get_float_urlencoded(const char *input, const char *key, float *value){
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


esp_err_t eweb_send_resp_try_chunk(httpd_req_t *req,char*buff,size_t buff_len){
    if (buff_len > MAX_CONTENT_SIZE) {
        httpd_resp_set_hdr(req, "Transfer-Encoding", "chunked");
        size_t sent = 0;
        while (sent < buff_len) {
            size_t chunk_size = (buff_len - sent > SHUNK_SIZE) ? SHUNK_SIZE : buff_len - sent;
            esp_err_t res = httpd_resp_send_chunk(req, buff + sent, chunk_size);
            if (res != ESP_OK) 
                return res;

            sent += chunk_size;
        }
        return httpd_resp_send_chunk(req, NULL, 0);
    }
    return httpd_resp_send(req, buff, buff_len );    
}

// STATIC HTML(GET)
esp_err_t eweb_static_html_handler(httpd_req_t *req) {
    static_ctx_handler*html = (static_ctx_handler *)req->user_ctx;
    httpd_resp_set_type(req, "text/html");
    return eweb_send_resp_try_chunk(req,html->asm_start , html->asm_end - html->asm_start);
}

// Static  (GET)
esp_err_t eweb_static_handler(httpd_req_t *req) {
    static_ctx_handler*ctx = (static_ctx_handler *)req->user_ctx;
    httpd_resp_set_type(req, ctx->resp_type);
    httpd_resp_set_hdr(req, "Cache-Control", "public, max-age=86400");
    return eweb_send_resp_try_chunk(req,ctx->asm_start , ctx->asm_end - ctx->asm_start);
}

void eweb_insert_ctx_into_uri(uri_ctx_hanlder*uri){
    if(uri->has_ctx)
        uri->uri.user_ctx = &uri->static_ctx;
}

//Insert Handlers into WebServer
void eweb_set_uri_hanlders(uri_ctx_hanlder*uri_ctx_handlers,size_t uris_size){

    for(unsigned i =0; i < uris_size; i++)
        httpd_register_uri_handler(WebServer, &uri_ctx_handlers[i].uri);
}

//char *buff = malloc(req->content_len + 1);
bool eweb_get_all_data_request(httpd_req_t *req,char*buffer){
        
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

void eweb_init(uint16_t max_uri) {

    ewifi_init();
    
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = max_uri;
    
    ESP_ERROR_CHECK(httpd_start(&WebServer, &config));
}

void eweb_preapare_uri_hanlders(uri_ctx_hanlder*static_uris,size_t uri_handler_len) {
    for(size_t i =0;i<uri_handler_len;i++)
        eweb_insert_ctx_into_uri(&static_uris[i]);
}



esp_err_t eweb_check_condicional_function(httpd_req_t *req){
    static_ctx_handler *ctx = (static_ctx_handler *)req->user_ctx;
    if ( ctx && ctx->uri_condicional_function && ctx->uri_execution_function) {
        if(ctx->uri_condicional_function(req))
            return ctx->uri_execution_function(req);
        else{
            httpd_resp_set_status(req, "400 Bad Request");
            httpd_resp_set_hdr(req, "Content-Type", "text");
            httpd_resp_send(req, "No validate Conditional Function", HTTPD_RESP_USE_STRLEN);
            return ESP_FAIL;
        }    

    } else {
        httpd_resp_set_status(req, "500 Internal Server Error");
        httpd_resp_set_hdr(req, "Content-Type", "text");
        httpd_resp_send(req, "No conditional function defined", HTTPD_RESP_USE_STRLEN);
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t eweb_call_excecution_function(httpd_req_t *req){
    static_ctx_handler *ctx = (static_ctx_handler *)req->user_ctx;
    if ( ctx && ctx->uri_execution_function) {
        return ctx->uri_execution_function(req); 
    } else {
        httpd_resp_set_status(req, "500 Internal Server Error");
        httpd_resp_set_hdr(req, "Content-Type", "text");
        httpd_resp_send(req, "No conditional function defined", HTTPD_RESP_USE_STRLEN);
    }
    return ESP_OK;
}