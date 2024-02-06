//---------------------------------------------------------
//                      _                            _
//                     | |                          | |
//  ___   _   _   ___  | |_    ___   _ __ ___       | |__
// / __| | | | | / __| | __|  / _ \ | '_ ` _ \      | '_ \ 
// \__ \ | |_| | \__ \ | |_  |  __/ | | | | | |  _  | | | |
// |___/  \__, | |___/  \__|  \___| |_| |_| |_| (_) |_| |_|
//         __/ |
//        |___/
//
// Foundational functions like UART initialization

void init_serial(uint32_t baud_rate) {
	Serial.begin(baud_rate);
	Serial.println('\n');
	Serial.println("######################");
	Serial.printf( "EMOTISCOPE v%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
	Serial.println("######################");
}

void init_system() {
	extern void init_leds();
	extern void init_i2s_microphone();
	extern void init_window_lookup();
	extern void init_goertzel_constants_musical();
	extern void init_goertzel_constants_full_spectrum();
	extern void init_tempo_goertzel_constants();
	extern void init_wifi();
	extern void init_filesystem();

	// Artificial 5-second boot up wait time if needed
	//for(uint16_t i = 0; i < 5; i++){ printf("WAITING FOR %d SECONDS...\n", 5-i); delay(1000); }

	init_serial(2000000);					  // (system.h)
	init_filesystem();                        // (filesystem.h)
	init_configuration();                     // (configuration.h)
	init_leds();							  // (leds.h)
	init_i2s_microphone();					  // (microphone.h)
	init_window_lookup();					  // (goertzel.h)
	init_goertzel_constants_musical();		  // (goertzel.h)
	init_tempo_goertzel_constants();		  // (tempo.h)
	init_wifi();                              // (wireless.h)

	load_sliders_relevant_to_mode(configuration.current_mode);
	load_toggles_relevant_to_mode(configuration.current_mode);
}