// Shake
lv_timer_t *shake_timer = NULL;

// Shake detection
void shake_timer_cb(lv_timer_t *timer) {
    if (board.isGesture(Shake)) {
        static uint32_t last_shake_time = 0;
        uint32_t current_time = lv_tick_get();
        
        if (current_time - last_shake_time > 500) {
            last_shake_time = current_time;

            // if (currentState == State::SHAKE) {
            //     // Use LVGL animation instead of delay loop
            //     lv_anim_t fade_anim;
            //     lv_anim_init(&fade_anim);
            //     lv_anim_set_var(&fade_anim, cameraObject);
            //     lv_anim_set_values(&fade_anim, 0, 255);
            //     lv_anim_set_time(&fade_anim, 2560); // 256 * 10ms = 2.56 seconds
            //     lv_anim_set_exec_cb(&fade_anim, (lv_anim_exec_xcb_t)lv_obj_set_style_opa);
            //     lv_anim_start(&fade_anim);

            //     currentState = State::SHOW_GEN_AI_IMAGE;
            // }
        }
    }
}

void start_shake_detection(void) {
    if (shake_timer) {
        lv_timer_del(shake_timer);
    }
    
    // Check every 50ms for good responsiveness
    shake_timer = lv_timer_create(shake_timer_cb, 50, NULL);
    printf("Shake detection started\n");
}