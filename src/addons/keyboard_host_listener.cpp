#include "addons/keyboard_host_listener.h"
#include "gamepad.h"

void KeyboardHostListener::process_kbd_report(
    uint8_t dev_addr,
    hid_keyboard_report_t const *report
) {
    static_cast<void>(dev_addr); // Absolut warnungsfreier Unused-Guard für GCC/Clang

    // Reset/Initialisierung des Gamepad-Status
    preprocess_report();

    bool analogUp    = false;
    bool analogLeft  = false;
    bool analogDown  = false;
    bool analogRight = false;

    for (uint8_t i = 0; i < 6; i++) {
        const uint16_t keycode = static_cast<uint16_t>(report->keycode[i]);

        if (keycode == 0U)
            continue;

        /*
         * =========================================================================
         * ESDF / A1-A4 INTERCEPTION (Explizite Typ-Casts gegen -Wsign-compare)
         * =========================================================================
         */

        // A1 -> Left Analog UP
        if (_keyboard_host_mapButtonA1.isAssigned() &&
            keycode == static_cast<uint16_t>(_keyboard_host_mapButtonA1.key)) {
            analogUp = true;
            continue;
        }

        // A2 -> Left Analog LEFT
        if (_keyboard_host_mapButtonA2.isAssigned() &&
            keycode == static_cast<uint16_t>(_keyboard_host_mapButtonA2.key)) {
            analogLeft = true;
            continue;
        }

        // A3 -> Left Analog DOWN
        if (_keyboard_host_mapButtonA3.isAssigned() &&
            keycode == static_cast<uint16_t>(_keyboard_host_mapButtonA3.key)) {
            analogDown = true;
            continue;
        }

        // A4 -> Left Analog RIGHT
        if (_keyboard_host_mapButtonA4.isAssigned() &&
            keycode == static_cast<uint16_t>(_keyboard_host_mapButtonA4.key)) {
            analogRight = true;
            continue;
        }

        /*
         * =========================================================================
         * REGULÄRES KEYBOARD MAPPING
         * =========================================================================
         */

        _keyboard_host_state.dpad |=
            ((keycode == static_cast<uint16_t>(_keyboard_host_mapDpadUp.key))
                ? _keyboard_host_mapDpadUp.buttonMask : 0U);

        _keyboard_host_state.dpad |=
            ((keycode == static_cast<uint16_t>(_keyboard_host_mapDpadDown.key))
                ? _keyboard_host_mapDpadDown.buttonMask : 0U);

        _keyboard_host_state.dpad |=
            ((keycode == static_cast<uint16_t>(_keyboard_host_mapDpadLeft.key))
                ? _keyboard_host_mapDpadLeft.buttonMask : 0U);

        _keyboard_host_state.dpad |=
            ((keycode == static_cast<uint16_t>(_keyboard_host_mapDpadRight.key))
                ? _keyboard_host_mapDpadRight.buttonMask : 0U);


        _keyboard_host_state.buttons |=
            ((keycode == static_cast<uint16_t>(_keyboard_host_mapButtonB1.key))
                ? _keyboard_host_mapButtonB1.buttonMask : 0U);

        _keyboard_host_state.buttons |=
            ((keycode == static_cast<uint16_t>(_keyboard_host_mapButtonB2.key))
                ? _keyboard_host_mapButtonB2.buttonMask : 0U);

        _keyboard_host_state.buttons |=
            ((keycode == static_cast<uint16_t>(_keyboard_host_mapButtonB3.key))
                ? _keyboard_host_mapButtonB3.buttonMask : 0U);

        _keyboard_host_state.buttons |=
            ((keycode == static_cast<uint16_t>(_keyboard_host_mapButtonB4.key))
                ? _keyboard_host_mapButtonB4.buttonMask : 0U);

        _keyboard_host_state.buttons |=
            ((keycode == static_cast<uint16_t>(_keyboard_host_mapButtonL1.key))
                ? _keyboard_host_mapButtonL1.buttonMask : 0U);

        _keyboard_host_state.buttons |=
            ((keycode == static_cast<uint16_t>(_keyboard_host_mapButtonR1.key))
                ? _keyboard_host_mapButtonR1.buttonMask : 0U);

        _keyboard_host_state.buttons |=
            ((keycode == static_cast<uint16_t>(_keyboard_host_mapButtonL2.key))
                ? _keyboard_host_mapButtonL2.buttonMask : 0U);

        _keyboard_host_state.buttons |=
            ((keycode == static_cast<uint16_t>(_keyboard_host_mapButtonR2.key))
                ? _keyboard_host_mapButtonR2.buttonMask : 0U);

        _keyboard_host_state.buttons |=
            ((keycode == static_cast<uint16_t>(_keyboard_host_mapButtonS1.key))
                ? _keyboard_host_mapButtonS1.buttonMask : 0U);

        _keyboard_host_state.buttons |=
            ((keycode == static_cast<uint16_t>(_keyboard_host_mapButtonS2.key))
                ? _keyboard_host_mapButtonS2.buttonMask : 0U);

        _keyboard_host_state.buttons |=
            ((keycode == static_cast<uint16_t>(_keyboard_host_mapButtonL3.key))
                ? _keyboard_host_mapButtonL3.buttonMask : 0U);

        _keyboard_host_state.buttons |=
            ((keycode == static_cast<uint16_t>(_keyboard_host_mapButtonR3.key))
                ? _keyboard_host_mapButtonR3.buttonMask : 0U);
    }

    /*
     * =========================================================================
     * ANALOGSTICK BERECHNEN (SOCD CLEANING)
     * =========================================================================
     */

    if (analogLeft && !analogRight) {
        _keyboard_host_state.lx = GAMEPAD_JOYSTICK_MIN;
    } else if (analogRight && !analogLeft) {
        _keyboard_host_state.lx = GAMEPAD_JOYSTICK_MAX;
    } else {
        _keyboard_host_state.lx = GAMEPAD_JOYSTICK_MID;
    }

    if (analogUp && !analogDown) {
        _keyboard_host_state.ly = GAMEPAD_JOYSTICK_MIN;
    } else if (analogDown && !analogUp) {
        _keyboard_host_state.ly = GAMEPAD_JOYSTICK_MAX;
    } else {
        _keyboard_host_state.ly = GAMEPAD_JOYSTICK_MID;
    }
}