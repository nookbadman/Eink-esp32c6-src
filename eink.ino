#include <GxEPD2_BW.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <SPI.h>

// กำหนดขา SPI สำหรับ ESP32-C3 Super Mini
#define EPD_SCK   4
#define EPD_MISO  5   // ไม่ได้ต่อใช้งานจริง แต่ใส่ไว้ตามพารามิเตอร์ SPI
#define EPD_MOSI  6
#define EPD_CS    7
#define EPD_DC    3
#define EPD_RST   2
#define EPD_BUSY  1

// ไดรเวอร์จอ E-Ink 2.13" ขาว-ดำ (รุ่นยอดนิยม Waveshare / Good Display ชิป SSD1680)
// หากจอเป็นรุ่นอื่น สามารถสลับเปลี่ยนคลาสไดรเวอร์ได้
GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> display(
  GxEPD2_213_B74(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY)
);

void helloWorld() {
  display.setRotation(1);                     // หมุนจอเป็นแนวนอน (0-3)
  display.setFont(&FreeSansBold9pt7b);         // เลือกฟอนต์
  display.setTextColor(GxEPD_BLACK);          // สีตัวอักษรสีดำ

  int16_t tbx, tby; 
  uint16_t tbw, tbh;
  const char text[] = "Hello World!";
  
  // คำนวณขนาดข้อความเพื่อจัดกึ่งกลางหน้าจอ
  display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;

  // ใช้ Page Drawing Loop ตามมาตรฐาน GxEPD2 เพื่อประหยัดแรม
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);          // พื้นหลังสีขาว
    display.setCursor(x, y - 10);
    display.print(text);

    display.setFont();                        // ฟอนต์เริ่มต้นสำหรับคำอธิบาย
    display.setCursor(x - 10, y + 20);
    display.print("ESP32-C3 & E-Ink 2.13\"");
    
    // วาดกรอบสี่เหลี่ยมรอบจอ
    display.drawRect(5, 5, display.width() - 10, display.height() - 10, GxEPD_BLACK);
  } while (display.nextPage());
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // กำหนดขา SPI ของ ESP32-C3
  SPI.begin(EPD_SCK, EPD_MISO, EPD_MOSI, EPD_CS);

  // เริ่มต้นการทำงานของจอ (115200 baud สำหรับ debug serial)
  display.init(115200);

  // แสดงผลข้อความ
  helloWorld();

  // สั่งพักหน้าจอ (Deep Sleep ของจอ) เพื่อถนอมเม็ดหมึกและประหยัดไฟ
  display.hibernate();
  Serial.println("Display updated and entered hibernate mode.");
}

void loop() {
  // หน้าจอ E-Ink จะคงภาพเดิมไว้ตลอดแม้ไม่มีไฟเลี้ยง จึงปล่อย loop ว่างไว้ได้เลย
}