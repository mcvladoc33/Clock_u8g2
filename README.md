### Опис проекту


Проект простого **годинника** та демо **меню** на **ардуіно** з використанням **spi** oled дисплею **ssd1309** 128X64 та бібліотеки **U8G2**.

![photo_5449809012221668954_y](https://github.com/user-attachments/assets/6e92726d-78c9-4567-ae13-bdf7885b72d4)
![photo_5449809012221668955_y](https://github.com/user-attachments/assets/e6068d7c-b105-48e9-9833-fdf5cb1bf81b)

### Симуляція у wokwi


![Screenshot 2025-01-07 234253](https://github.com/user-attachments/assets/c74f2952-24c6-435b-a5f0-c92a6fa40729)
![Screenshot 2025-01-07 234556](https://github.com/user-attachments/assets/da71d950-3f31-4e5a-a9cc-f27fae102bf6)

У симуляції бере участь дисплей **ssd1306** з **i2c** з'єднанням. Також симуляція досить повільна, але принципи роботи пристрою передає. У коді можна обирати дисплей шляхом розкоментування відповідної стрічки.

```c
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* reset=*/ U8X8_PIN_NONE);

U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
```
