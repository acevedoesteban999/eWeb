idf_component_register(
    SRCS
        "eWeb.c"
    INCLUDE_DIRS 
        "include"
        
    EMBED_FILES
        "src/css/styles.min.css"
        "src/css/milligram/milligram.min.css"
        "src/css/milligram/normalize.min.css" 
        
        "src/js/ajax_form.min.js" 
    REQUIRES  
        esp_http_server
        eStore
        eWifi
)
