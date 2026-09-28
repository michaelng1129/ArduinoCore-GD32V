# ArduinoCore-GD32V

Arduino core for GigaDevice GD32VW553 (RISC-V, Wi-Fi 6 + BLE 5.3).

## 支援的板子

| 板子 | 模組 | Flash | 說明 |
|------|------|-------|------|
| GD32VW553-START | GD32VW553-MINI-I/E (GD32VW553KMQ, QFN32) | 4MB | START V5.0 底板 + MINI 模組 |

## GD32VW553-START 接腳

接腳定義依據 AN154 (Rev 1.3b) Table 1-1/1-2、GD32VW553-MINI Datasheet Table 4-1。

### J1 (2x8)

| Arduino | GPIO | 功能 |
|---------|------|------|
| 0 | PA0 | A0, ADC_IN0, TIMER1_CH0 (PWM), SPI_MOSI |
| 1 | PA1 | A1, ADC_IN1, TIMER1_CH1, SPI_MISO |
| 2 | PA2 | A2, ADC_IN2, TIMER1_CH2, I2C0_SCL, SPI_SCK |
| 3 | PA3 | A3, ADC_IN3, TIMER1_CH3, I2C0_SDA, SPI_NSS |
| 4 | PA4 | A4, ADC_IN4, SPI_MOSI/NSS |
| 5 | PA5 | A5, ADC_IN5, SPI_MISO/SCK |
| 6 | PA6 | UART2_TX (Serial) |
| 7 | PA7 | UART2_RX (Serial) |
| 8 | PB0 | ADC_IN8 |
| 9 | PB15 | GPIO |

### J2 (2x8)

| Arduino | GPIO | 功能 |
|---------|------|------|
| 10 | PA8 | GPIO |
| 11 | PA12 | GPIO |
| 12 | PA13 | JTAG JTMS（與 GD-Link 共用） |
| 13 | PA14 | JTAG JTCK（與 GD-Link 共用） |
| 14 | PA15 | JTAG JTDI（與 GD-Link 共用） |
| 15 | PB3 | JTAG JTDO（與 GD-Link 共用） |
| 16 | PB4 | JTAG JNTRST（與 GD-Link 共用） |
| 17 | PC14 | GPIO |
| 18 | PC15 | GPIO |

### 板載 LED（不在 J1/J2 上）

| Arduino | GPIO | 說明 |
|---------|------|------|
| 19 | PC0 | LED1 / LED_BUILTIN（高電位亮） |
| 20 | PC1 | LED2 |
| 21 | PC2 | LED3 |

注意：
- START 板沒有使用者按鍵（SW1 是 reset，SW2 是保留），因此沒有定義 `PIN_BUTTON`。
- `PC8/BOOT0`、`PB1/BOOT1` 為開機設定腳，未納入接腳映射。
- JTAG 腳需移除 J4 跳線帽才能當一般 GPIO 使用。

### Serial

`Serial` 使用 UART2（PA6 TX / PA7 RX），接到板載 GD-Link 的 USB serial。
注意：SDK 的 log 輸出（LOG_UART）同樣使用 UART2，兩者共用同一個 USB serial。

### analogWrite (PWM)

只有 pin 0（PA0，TIMER1_CH0）支援 `analogWrite`。

## 燒錄

需要 GigaDevice 的 OpenOCD fork（原版 OpenOCD 沒有 `gd32vw55x` flash driver）。

Arduino IDE 會使用 repo 內的 vendor config：
`system/gd32vw55x/sdk/MSDK/projects/eclipse/msdk/openocd_gdlink.cfg`

App 燒錄到 `0x0800A000`（OTA slot，見 AN154）。板子上的 MBL（`0x08000000`）
必須已存在（出廠即有），由它從 OTA slot 啟動 app。

手動燒錄：
```
openocd -f system/gd32vw55x/sdk/MSDK/projects/eclipse/msdk/openocd_gdlink.cfg \
  -c "program build/arduino.avr.gd32vw553_start/Blink.ino.bin verify reset exit 0x0800A000"
```

也可以用 USB 隨身碟模式：將 `image-all.bin` 複製到 GD-Link 的虛擬隨身碟
（見 AN154 第 6 章）。

## 範例

- `examples/Blink` - 閃爍 LED_BUILTIN（PC0）
- `examples/SerialEcho` - Serial 回顯測試（驗證 UART2 RX 中斷）

## 目前限制

- SPI / Wire (I2C) / WiFi / BLE 的 Arduino API 尚未實作。
- `pins_arduino.h` 中的 `PIN_SPI_*` / `PIN_WIRE_*` 僅為預定接腳定義。
