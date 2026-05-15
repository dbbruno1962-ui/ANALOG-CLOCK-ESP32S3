#pragma once

void ui_init(void);
void ui_update(void);

/* Q8 */
void q8_enter_ap_mode(const char* ip, const char* pass);
bool isQ8Active(void);