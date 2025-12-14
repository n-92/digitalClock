#include <Arduino.h>
#include "Waveshare_ST7262_LVGL.h"
#include <lvgl.h>

// --- Global Variables ---
int current_hour = 12;
int current_min = 0;
int current_sec = 0;

int current_day = 25;   
int current_month = 9;  // October

const char * month_names[] = {
    "JAN", "FEB", "MAR", "APR", "MAY", "JUN", 
    "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"
};

// --- UI Objects ---
lv_obj_t * label_time;
lv_obj_t * label_date;
lv_obj_t * bar_seconds;

// --- Helper Functions ---
void update_date_ui() {
    lv_label_set_text_fmt(label_date, "%s %d", month_names[current_month], current_day);
}

void next_day() {
    current_day++;
    if (current_day > 31) {
        current_day = 1;
        current_month++;
        if (current_month > 11) current_month = 0; 
    }
    update_date_ui();
}

// --- Touch Callbacks ---
static void hour_event_cb(lv_event_t * e) {
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        current_hour++;
        if (current_hour > 23) current_hour = 0;
        lv_label_set_text_fmt(label_time, "%02d:%02d", current_hour, current_min);
    }
}

static void min_event_cb(lv_event_t * e) {
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        current_min++;
        if (current_min > 59) current_min = 0;
        current_sec = 0; 
        lv_label_set_text_fmt(label_time, "%02d:%02d", current_hour, current_min);
        lv_bar_set_value(bar_seconds, 0, LV_ANIM_OFF);
    }
}

static void date_event_cb(lv_event_t * e) {
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        next_day();
    }
}

// --- Timer ---
void clock_timer_cb(lv_timer_t * timer) {
    current_sec++;
    if (current_sec >= 60) {
        current_sec = 0;
        current_min++;
        if (current_min >= 60) {
            current_min = 0;
            current_hour++;
            if (current_hour >= 24) {
                current_hour = 0;
                next_day();
            }
        }
    }
    
    lv_label_set_text_fmt(label_time, "%02d:%02d", current_hour, current_min);
    lv_bar_set_value(bar_seconds, current_sec, LV_ANIM_OFF);
}

// --- Setup ---
void setup() {
    Serial.begin(115200);
    lcd_init();
    lvgl_port_lock(-1);

    // 1. Background
    lv_obj_t * screen = lv_scr_act();
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);

    // 2. Main Card
    lv_obj_t * card = lv_obj_create(screen);
    lv_obj_set_size(card, 400, 250);
    lv_obj_center(card);
    lv_obj_set_style_bg_color(card, lv_color_hex(0x111111), 0);
    lv_obj_set_style_border_color(card, lv_color_hex(0x00D2FF), 0); 
    lv_obj_set_style_border_width(card, 2, 0);
    lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);

    // 3. Time Label (FIXED: NO ZOOM)
    label_time = lv_label_create(card);
    lv_label_set_text(label_time, "12:00");
    lv_obj_set_style_text_color(label_time, lv_color_white(), 0);
    
    // We use standard font size 20. It will be smaller, but it will APPEAR.
    lv_obj_set_style_text_font(label_time, &lv_font_montserrat_20, 0); 
    
    // I REMOVED the "transform_zoom" line here.
    
    lv_obj_align(label_time, LV_ALIGN_CENTER, 0, -30);

    // 4. Date Label
    label_date = lv_label_create(card);
    update_date_ui(); 
    lv_obj_set_style_text_color(label_date, lv_color_hex(0x00D2FF), 0);
    lv_obj_set_style_text_font(label_date, &lv_font_montserrat_14, 0);
    lv_obj_align(label_date, LV_ALIGN_BOTTOM_MID, 0, -55);
    lv_obj_add_flag(label_date, LV_OBJ_FLAG_CLICKABLE); 
    lv_obj_add_event_cb(label_date, date_event_cb, LV_EVENT_CLICKED, NULL);

    // 5. Seconds Bar
    bar_seconds = lv_bar_create(card);
    lv_obj_set_size(bar_seconds, 300, 10);
    lv_obj_align(bar_seconds, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_bar_set_range(bar_seconds, 0, 60);
    lv_obj_set_style_bg_color(bar_seconds, lv_color_hex(0x333333), LV_PART_MAIN);
    lv_obj_set_style_bg_color(bar_seconds, lv_color_hex(0x00D2FF), LV_PART_INDICATOR);

    // 6. Touch Buttons (Invisible)
    lv_obj_t * btn_hour = lv_btn_create(card);
    lv_obj_set_size(btn_hour, 140, 100);
    lv_obj_align(btn_hour, LV_ALIGN_CENTER, -80, -30);
    lv_obj_set_style_bg_opa(btn_hour, 0, 0); 
    lv_obj_set_style_shadow_width(btn_hour, 0, 0);
    lv_obj_add_event_cb(btn_hour, hour_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * btn_min = lv_btn_create(card);
    lv_obj_set_size(btn_min, 140, 100);
    lv_obj_align(btn_min, LV_ALIGN_CENTER, 80, -30);
    lv_obj_set_style_bg_opa(btn_min, 0, 0);
    lv_obj_set_style_shadow_width(btn_min, 0, 0);
    lv_obj_add_event_cb(btn_min, min_event_cb, LV_EVENT_CLICKED, NULL);
    
    // 7. Instructions
    lv_obj_t * hint = lv_label_create(card);
    lv_label_set_text(hint, "TAP TIME TO SET  |  TAP DATE TO CHANGE");
    lv_obj_set_style_text_color(hint, lv_color_hex(0x555555), 0);
    lv_obj_set_style_text_font(hint, &lv_font_montserrat_10, 0);
    lv_obj_align(hint, LV_ALIGN_TOP_MID, 0, 5);

    lv_timer_create(clock_timer_cb, 1000, NULL);
    lvgl_port_unlock();
}

void loop() {
    delay(1000);
}