#include "gui.h"
#include "lock.h"

#include "ssd1306.h"
#include "ssd1306_fonts.h"


extern const uint8_t locked[];
extern const uint8_t kirby[];


void GUI_init(void)
{
	ssd1306_Init();
	GUI_kirby();


}

void GUI_locked(void)
{
	ssd1306_Fill(Black);
	ssd1306_SetCursor(20, 5);
	ssd1306_WriteString("Locked", Font_16x26, White);
	ssd1306_DrawBitmap(50, 35, locked, 30, 30, White);
	ssd1306_UpdateScreen();
}

void GUI_kirby(void) {
	ssd1306_Fill(Black);
    ssd1306_DrawBitmap(0, 0, kirby, 128, 64, White);
    ssd1306_UpdateScreen();
}

void GUI_unlocked(void)
{
	ssd1306_Fill(Black);
	ssd1306_SetCursor(5, 5);
	ssd1306_WriteString("Unlocked", Font_16x26, White);
	ssd1306_UpdateScreen();
}

void GUI_update_password_init(void)
{
	ssd1306_Fill(Black);
	ssd1306_SetCursor(5, 5);
	ssd1306_WriteString("New PW:", Font_16x26, White);
	ssd1306_UpdateScreen();
}

void GUI_update_password(uint8_t *password)
{
	ssd1306_SetCursor(10, 35);
	ssd1306_WriteString((char *)password, Font_7x10, White);
	ssd1306_UpdateScreen();
}

void GUI_update_password_success(void)
{
	ssd1306_SetCursor(5, 35);
	ssd1306_WriteString("Success!", Font_16x26, White);
	ssd1306_UpdateScreen();

}

void GUI_display_key(char key, uint8_t x, uint8_t y) {
    char buffer[2] = {key, '\0'}; // Buffer para el carácter
    ssd1306_SetCursor(x, y);
    ssd1306_WriteString(buffer, Font_11x18, White);
    ssd1306_UpdateScreen();
}

