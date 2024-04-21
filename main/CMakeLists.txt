idf_component_register(
    SRCS "app/app_main.c" "app/lvgl_demo_ui.c"
    SRCS "bsp/bsp_iic.c"
    SRCS "sensor/gxhtc3.c" "sensor/qmc5883l.c" "sensor/qmi8658c.c"
    SRCS "audio.c" 
    SRCS "assets/image_lckfb_logo.c" "assets/image_th_icon.c" "assets/image_spr_icon.c" "assets/image_mic_icon.c" 
    SRCS "assets/image_att_icon.c" "assets/image_comp_icon.c" "assets/image_set_icon.c" "assets/font_alipuhui20" "assets/font_myawesome.c"
    INCLUDE_DIRS "." "app" "assets" "bsp" "sensor"
    EMBED_FILES "sword.pcm")
