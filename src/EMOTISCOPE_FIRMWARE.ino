// ############################################################################
// ############################################################################
//                                  __  _
//            ___  ____ ___  ____  / /_(_)_____________  ____  ___
//           / _ \/ __ `__ \/ __ \/ __/ / ___/ ___/ __ \/ __ \/ _ \
//          /  __/ / / / / / /_/ / /_/ (__  ) /__/ /_/ / /_/ /  __/
//          \___/_/ /_/ /_/\____/\__/_/____/\___/\____/ .___/\___/
//              Audio-visual engine by @lixielabs    /_/
//                 Released under the MIT Licence
//
// ############################################################################
// ## VERSION #################################################################

#define VERSION_MAJOR ( 1 )
#define VERSION_MINOR ( 0 )
#define VERSION_PATCH ( 0 )

// ############################################################################
// ## DEPENDENCIES ############################################################

// External dependencies
#include <FastLED.h> // ........... Blasting data to the LEDs
#include <PsychicHttp.h> // ....... Handling the web-app HTTP and WS
#include <HTTPClient.h> // ........ Used to make POST requests to the discovery server
#include <ESPmDNS.h> // ........... Used for "emotiscope.local" domain name
#include <Ticker.h> // ............ For timing functions
#include <WiFi.h> // .............. WiFi connection library
#include <driver/i2s.h> // ........ I2S microphone driver
#include <esp_dsp.h> // ........... Fast SIMD-style array math
#include <esp_wifi.h> // .......... WiFi, but like - the hardware side of it

// Internal dependencies
#include "secrets.h" // ........... You don't get to have this, rename "YOUR_secrets.h" to use it
#include "types.h" // ............. typedefs for things like CRGBFs
#include "profiler.h" // .......... Developer tools, measures the execution of functions
#include "sliders.h" // ........... Handles sliders that appear in the web app
#include "toggles.h" // ........... Handles toggles that appear in the web app
#include "filesystem.h" // ........ LittleFS functions
#include "configuration.h" // ..... Storing and retreiving your settings
#include "utilities.h" // ......... Custom generic math functions
#include "system.h" // ............ Lowest-level firmware functions
#include "led_driver.h" // ........ Low-level LED communication, (ab)uses RMT for non-blocking output
#include "leds.h" // .............. LED dithering, effects, filters
#include "microphone.h" // ........ For gathering audio chunks from the microphone
#include "goertzel.h" // .......... GDFT or God Damn Fast Transform is implemented here
#include "tempo.h" // ............. Comupation of (and syncronization) to the music tempo
#include "audio_debug.h" // ....... Print audio data over UART
#include "lightshow_modes.h" // ... Definition and handling of lightshow modes
#include "commands.h" // .......... Queuing and parsing of commands recieved
#include "wireless.h" // .......... Communication with your network and the web-app

// Dual-core loops
#include "cpu_core.h"
#include "gpu_core.h"

// Development Notes
#include "notes.h"

// Not for you, you don't get to have my WiFi, SORRY
#include "secrets.h"

// ############################################################################
// ## CODE ####################################################################

// One core to run audio (CPU) ------------------------------------------------
void loop() {
	run_cpu();	// (cpu_core.h)
}

// One core to run graphics (GPU) ---------------------------------------------
void loop_gpu(void *param) {
	for (;;) {
		run_gpu();	// (gpu_core.h)
	}
}

// EVERYTHING BEGINS HERE ON BOOT ---------------------------------------------
void setup() {
	// (system.h) Initialize all peripherals
	init_system();

	// Start the second core as a dedicated GPU for the LEDs/color math
	(void)xTaskCreatePinnedToCore(loop_gpu, "loop_gpu", 8192, NULL, 0, NULL, 0);
}