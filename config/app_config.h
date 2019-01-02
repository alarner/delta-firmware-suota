// The version of the firmware
#define VERSION 16

// The number of ticks to wait for activity before turning the RFID reader on
#define IMU_DELAY_BEFORE_READ OS_MS_2_TICKS(500)

// THe number of ticks (after the IMU delay) that the RFID reader can be turned on
#define RFID_READABLE_TIMEFRAME OS_MS_2_TICKS(500)

// Any rfid reads of the same tag within this timeperiod will be considered duplicates
// The second read will not be written to memory and will not cause the haptic to buzz
#define RFID_DUPLICATE_TIMEFRAME OS_MS_2_TICKS(20000)

// The number of ticks to turn the LED red after an RFID read
#define RFID_READ_LED_TIMEFRAME OS_MS_2_TICKS(4000)

// The number of ticks that the button must be continually pressed down before memory is reset
#define BUTTON_DELAY_BEFORE_POWER_TOGGLE OS_MS_2_TICKS(1000)

// The maximum amount of data that can be store in an action
#define ACTION_DATA_LENGTH 30

// The number of milliseconds per day
#define MS_PER_DAY 86400000

// The maximum length that the name of the bluetooth device can be
#define MAX_BLUETOOTH_NAME 20

// The amount of time the haptic should buzz after a tag read
#define BUZZ_LENGTH_AFTER_READ 500

// The amount of time the haptic should buzz after a tag read
#define BUZZ_LENGTH_BEFORE_POWER_TOGGLE 100

// The power to set when the RFID reader is in normal reading mode
#define RFID_POWER_NORMAL 2600

// The maximum number of unique tags that can be stored per batch read from the rfid reader.
#define MAX_UNIQUE_TAGS_PER_BATCH 10

// The number of reads that a single tag must get over the second highest tag to establish
// domainance
#define DOMINANCE_THRESHOLD 5

// The number of bytes in an rfid uid
#define RFID_UID_LENGTH 12

// The number of tags we should try to read when scanning
#define RFID_NUM_TAG_SCAN 375

// The number of tags we should try to read when scanning
#define ACTION_QUEUE_SIZE 10

// The mvolt reading at which point the tracker should power off
#define POWER_OFF_MVOLT 3300
