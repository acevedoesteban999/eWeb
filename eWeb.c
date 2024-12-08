#include "eWifi.h"
#include "eWeb.h"

httpd_handle_t WebServer = NULL; 
const char* _login_asm_start = NULL;
const char* _login_asm_end = NULL;
char redirect_404[MAX_404_BUFFER_SIZE];


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

// Login(GET)
esp_err_t login_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, _login_asm_start, _login_asm_end - _login_asm_start);
    return ESP_OK;
}

// Login (POST)
esp_err_t login_post_handler(httpd_req_t *req) {
    char buff[BUFF_LEN];
    int ret = httpd_req_recv(req, buff, sizeof(buff) - 1);
    if (ret <= 0) {
        // Manejo de error
        return ESP_FAIL;
    }
    buff[ret] = '\0'; 

    // Extraer nombre de usuario y contraseña
    char *username = strtok(buff, "&");
    char *password = strtok(NULL, "&");
    
    username = username + strlen("username="); // Saltar "username="
    password = password + strlen("password="); // Saltar "password="
    
    if (authenticate_user(username, password)) {
        httpd_resp_set_hdr(req, "Set-Cookie", find_user_by_username(username)->session_token); // Establecer cookie
        httpd_resp_set_status(req, "302 Found");
        httpd_resp_set_hdr(req, "Location", "/home");
        httpd_resp_send(req, NULL, 0);
    } else {
        httpd_resp_send(req, "Authentication Failed", HTTPD_RESP_USE_STRLEN);
    }
    return ESP_OK;
}

// Logout (get)
esp_err_t logout_handler(httpd_req_t *req) {
    char session_token[TOKEN_LEN];
    httpd_req_get_hdr_value_str(req, "Cookie", session_token, TOKEN_LEN);
    if(logout_user(session_token)) {
        httpd_resp_set_status(req, "302 Found");
        httpd_resp_set_hdr(req, "Location", "/login.html");
        httpd_resp_send(req, NULL, 0);
    }
    return ESP_OK;
}

// Static  (get)
esp_err_t static_handler(httpd_req_t *req) {
    static_ctx_handler*ctx = (static_ctx_handler *)req->user_ctx;
    httpd_resp_set_type(req, ctx->resp_type);
    httpd_resp_set_hdr(req, "Cache-Control", "public, max-age=86400");
    httpd_resp_send(req, ctx->asm_start, ctx->asm_end - ctx->asm_start );
    return ESP_OK;
}

// Error 404
esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    httpd_resp_set_status(req, "302 Temporary Redirect");
    httpd_resp_set_hdr(req, "Location", redirect_404);
    httpd_resp_send(req, "Redirect to the captive portal", HTTPD_RESP_USE_STRLEN);

    ESP_LOGW("", "Redirecting to: %s",redirect_404);
    return ESP_OK;
}

//Insert Handlers into WebServer
void set_custom_uri_handlers(httpd_uri_t*uri_handlers,size_t uris_size){

    for(unsigned i =0; i < uris_size; i++)
        httpd_register_uri_handler(WebServer, &uri_handlers[i]);
}

// Required char* to login start and end EMBED_FILE
void set_main_uri_handler(const char*__login_asm_start,const char*__login_asm_end,const char*__redirect_404){
    _login_asm_start = __login_asm_start;
    _login_asm_end = __login_asm_end;
    if (strlen(__redirect_404) < MAX_404_BUFFER_SIZE)
        strcpy(redirect_404,__redirect_404);
    else
        strcpy(redirect_404,"/login.html");
    httpd_uri_t uri;
    uri.uri = "/login.html";
    uri.method = HTTP_GET;
    uri.handler = login_handler;
    uri.user_ctx = NULL;
    httpd_register_uri_handler(WebServer, &uri);
    
    uri.uri = "/login.html";
    uri.method = HTTP_POST;
    uri.handler = login_post_handler;
    uri.user_ctx = NULL;
    httpd_register_uri_handler(WebServer, &uri);
    
    uri.uri = "/logout";
    uri.method = HTTP_GET;
    uri.handler = logout_handler;
    uri.user_ctx = NULL;
    httpd_register_uri_handler(WebServer, &uri);

    
    httpd_register_err_handler(WebServer, HTTPD_404_NOT_FOUND, http_404_error_handler);
}

void set_static_uri_handler(httpd_uri_t*uri_handlers,size_t uris_size){
    
}

void start_webserver(uint16_t max_uri) {

    wifi_init_softap();
    
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = max_uri;
    
    ESP_ERROR_CHECK(httpd_start(&WebServer, &config));
}
