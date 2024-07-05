#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "driver/gpio.h"

// Define motor control pins
#define MOTOR1_STEP_PIN  1
#define MOTOR1_DIR_PIN   2
#define MOTOR2_STEP_PIN  41
#define MOTOR2_DIR_PIN   40



TaskHandle_t motorTask1Handle = NULL;
TaskHandle_t motorTask2Handle = NULL;

// Timer handle
esp_timer_handle_t sync_timer_handle;

extern "C" {
    void app_main(void);
}

// Function to initialize motors
void initMotors() {
  pinMode(MOTOR1_STEP_PIN, OUTPUT);
  pinMode(MOTOR1_DIR_PIN, OUTPUT);
  pinMode(MOTOR2_STEP_PIN, OUTPUT);
  pinMode(MOTOR2_DIR_PIN, OUTPUT);

  // Initialize direction pins to a known state
  digitalWrite(MOTOR1_DIR_PIN, LOW);
  digitalWrite(MOTOR2_DIR_PIN, LOW);
}

// Function for precise microsecond delay
void precise_delay_us(uint64_t delay) {
    uint64_t start = esp_timer_get_time();
    while (esp_timer_get_time() - start < delay) {
        // wait
    }
}

// Motor task for Motor 1
void motorTask1(void *pvParameters) {
  for (;;) {
    // Wait for notification to step
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    
    // Step the motor
    digitalWrite(MOTOR1_STEP_PIN, HIGH);
    precise_delay_us(50);  // Precise short delay for stepping in microseconds
    digitalWrite(MOTOR1_STEP_PIN, LOW);
    precise_delay_us(50); 
  }
}

// Motor task for Motor 2
void motorTask2(void *pvParameters) {
  for (;;) {
    // Wait for notification to step
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    
    // Step the motor
    digitalWrite(MOTOR2_STEP_PIN, HIGH);
    precise_delay_us(50);  // Precise short delay for stepping in microseconds
    digitalWrite(MOTOR2_STEP_PIN, LOW);
    precise_delay_us(50); 
  }
}

// Timer callback to notify both motor tasks
void IRAM_ATTR onSyncTimer(void* arg) {
  // Notify both motor tasks to step
  xTaskNotifyGive(motorTask1Handle);      // Sends control signals to both tasks.
  xTaskNotifyGive(motorTask2Handle);      // The tasks will not start until they receive a notify signal.
}

void app_main(void) {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("Starting synchronized motor control with high-resolution timer");

  // Initialize motors
  initMotors();

  // Create the motor tasks
  xTaskCreate(motorTask1, "Motor 1 Task", 4096, NULL, 2, &motorTask1Handle);
  xTaskCreate(motorTask2, "Motor 2 Task", 4096, NULL, 2, &motorTask2Handle);

  // Configure the synchronized timer
  const esp_timer_create_args_t sync_timer_args = {    // Timer setup
      .callback = &onSyncTimer,                       // When the timer expires, it calls the onSyncTimer function
      .name = "SyncTimer"
  };
  esp_timer_create(&sync_timer_args, &sync_timer_handle);  // Create the timer

  // Calculate the interval for each step to complete one revolution per second
  const uint64_t interval_per_step = 2000000; // 5000 microsecods to achieven 1 revol per second
  esp_timer_start_periodic(sync_timer_handle, interval_per_step); // Assigning Interval per step to the timer
}
