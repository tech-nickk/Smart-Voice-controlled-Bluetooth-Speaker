#include <DFRobot_MAX98357A.h>
#include "DFRobot_DF2301Q.h"

// Create amplifier instance
DFRobot_MAX98357A amplifier;

// Configure voice recognition sensor on Serial1
#if defined(ESP32)
  DFRobot_DF2301Q_UART DF2301Q(/*hardSerial =*/&Serial2, /*rx =*/16, /*tx =*/17);
#else
  DFRobot_DF2301Q_UART DF2301Q(/*hardSerial =*/&Serial1);
#endif



// Voice command IDs 
const uint8_t CMD_PLAY = 92;
const uint8_t CMD_STOP = 93;
const uint8_t CMD_PREVIOUS = 94;
const uint8_t CMD_NEXT = 95;
const uint8_t CMD_REOEAT = 96;
const uint8_t CMD_VOLUME_UP = 97;
const uint8_t CMD_VOLUME_DOWN = 98;
const uint8_t CMD_VOLUME_MAX = 99;
const uint8_t CMD_VOLUME_MIN = 100;
const uint8_t CMD_VOLUME_MID = 101;


// Current volume level
int currentVolume = 5;

void setup() {
  Serial.begin(115200);

  // Initialize voice recognition sensor
  while (!DF2301Q.begin()) {
    Serial.println("Voice sensor initialization failed!");
    delay(3000);
  }
  Serial.println("Voice sensor initialized successfully!");

  // Initialize amplifier
  while (!amplifier.begin("Nick Smart Speaker", GPIO_NUM_25, GPIO_NUM_26, GPIO_NUM_14)) {
    Serial.println("Amplifier initialization failed!");
    delay(3000);
  }
  Serial.println("Amplifier initialized successfully!");

  // Set initial volume
  amplifier.setVolume(currentVolume);

  // Initial voice module settings
  DF2301Q.settingCMD(DF2301Q_UART_MSG_CMD_SET_MUTE, 0);  // Unmute
  DF2301Q.settingCMD(DF2301Q_UART_MSG_CMD_SET_VOLUME, 10); // Set voice recognition volume
  DF2301Q.settingCMD(DF2301Q_UART_MSG_CMD_SET_WAKE_TIME, 10); // Wake time in seconds
  
  // Play startup sound
  DF2301Q.playByCMDID(23);  // You can change this ID to any appropriate sound
}

void loop() {
  // Get voice command ID
  uint8_t commandID = DF2301Q.getCMDID();
  
  // Process voice commands
  if (commandID != 0) {
    Serial.print("Received command ID: ");
    Serial.println(commandID);
    
    // Execute command based on ID
    switch (commandID) {
      case CMD_PLAY:
        Serial.println("Command: Play");
        esp_avrc_ct_send_passthrough_cmd(0, ESP_AVRC_PT_CMD_PLAY, ESP_AVRC_PT_CMD_STATE_PRESSED);
        break;
        
      case CMD_STOP:
        Serial.println("Command: Stop");
        esp_avrc_ct_send_passthrough_cmd(0, ESP_AVRC_PT_CMD_STOP, ESP_AVRC_PT_CMD_STATE_PRESSED);
        break;
        
      case CMD_NEXT:
        Serial.println("Command: Next Track");
        esp_avrc_ct_send_passthrough_cmd(0, ESP_AVRC_PT_CMD_FORWARD, ESP_AVRC_PT_CMD_STATE_PRESSED);
        break;
        
      case CMD_PREVIOUS:
        Serial.println("Command: Previous Track");
        esp_avrc_ct_send_passthrough_cmd(0, ESP_AVRC_PT_CMD_BACKWARD, ESP_AVRC_PT_CMD_STATE_PRESSED);
        break;
        
      case CMD_VOLUME_UP:
        if (currentVolume < 9) {
          currentVolume++;
          amplifier.setVolume(currentVolume);
          Serial.print("Volume increased to: ");
          Serial.println(currentVolume);
        }
        break;
        
      case CMD_VOLUME_DOWN:
        if (currentVolume > 0) {
          currentVolume--;
          amplifier.setVolume(currentVolume);
          Serial.print("Volume decreased to: ");
          Serial.println(currentVolume);
        }
        break;

      case CMD_VOLUME_MAX:
        if (currentVolume < 9) {
          currentVolume = 9;
          amplifier.setVolume(currentVolume);
          Serial.print("Volume increased to: ");
          Serial.println(currentVolume);
        }
        break;

      case CMD_VOLUME_MIN:
        
          currentVolume = 1;
          amplifier.setVolume(currentVolume);
          Serial.print("Volume increased to: ");
          Serial.println(currentVolume);
        
        break;

      case CMD_VOLUME_MID:
        
          currentVolume = 5;
          amplifier.setVolume(currentVolume);
          Serial.print("Volume increased to: ");
          Serial.println(currentVolume);
        
        break;
    }
  }
  
  delay(100);  // Small delay to prevent overwhelming the system
}