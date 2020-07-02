MCU = STM32F303

SRC+=backlight_driver.c

#Build Options
#change yes to no to disable
BOOTMAGIC_ENABLE = lite         # Virtual DIP switch configuration(+1000)
MOUSEKEY_ENABLE = no            # Mouse keys(+4700)
EXTRAKEY_ENABLE = yes           # Audio control and System control(+450)
CONSOLE_ENABLE = yes             # Console for debug(+400)
COMMAND_ENABLE = yes            # Commands for debug and configuration
#Do not enable SLEEP_LED_ENABLE.it uses the same timer as BACKLIGHT_ENABLE
SLEEP_LED_ENABLE = no           # Breathing sleep LED during USB suspend
#if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
NKRO_ENABLE = yes               # USB Nkey Rollover
BACKLIGHT_ENABLE = yes          # Enable keyboard backlight functionality
BACKLIGHT_DRIVER = custom       # Enable custom backlight driver
RGBLIGHT_ENABLE = yes           # Enable keyboard RGB underglow
RGBLIGHT_DRIVER = pwm           # Enable hardware PWM for RGB underglow
RGB_MATRIX_ENABLE = no          # RGB Matrix support
MIDI_ENABLE = no                # MIDI support (+2400 to 4200, depending on config)
UNICODE_ENABLE = no             # Unicode
BLUETOOTH_ENABLE = no           # Enable Bluetooth with the Adafruit EZ-Key HID
AUDIO_ENABLE = no               # Audio output on port C6
FAUXCLICKY_ENABLE = no          # Use buzzer to emulate clicky switches
HD44780_ENABLE = no             # Enable support for HD44780 based LCDs (+400)
ENCODER_ENABLE = yes            # Enable encoder support

# Enter lower-power sleep mode when on the ChibiOS idle thread
OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE
