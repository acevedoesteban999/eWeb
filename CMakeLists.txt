idf_component_register(
    SRCS
        "eAuth.c"
        "eWeb.c"
    INCLUDE_DIRS 
        "include"
    REQUIRES  
        esp_http_server
        eStore
        eWifi
)
