const char ADDR_PINS[] = { 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52 };
const int ADDR_BITS = 16;

const char DATA_PINS[] = { 31, 33, 35, 37, 39, 41, 43, 45 };
const int DATA_BITS = 8;

const int RW_PIN = 3;

const int CLOCK_PIN = 2;

static void clock_tick(void);

void setup(){
    for(int i = 0; i < ADDR_BITS; i++){
        pinMode(ADDR_PINS[i], INPUT);
    }

    for(int i = 0; i < DATA_BITS; i++){
        pinMode(DATA_PINS[i], INPUT);
    }

    pinMode(RW_PIN, INPUT);

    pinMode(CLOCK_PIN, INPUT);

    attachInterrupt(digitalPinToInterrupt(CLOCK_PIN), clock_tick, RISING);

    Serial.begin(115200);
}

void loop(){

}

static uint16_t read_addr(void)
{
    uint16_t addr = 0;

    for(int i = 0; i < ADDR_BITS; i++){
        if(digitalRead(ADDR_PINS[i])){    
            addr |= ((uint16_t) 1 << i);
        }
    }

    return addr;
}

static uint8_t read_data(void)
{
    uint8_t data = 0;

    for(int i = 0; i < DATA_BITS; i++){
        if(digitalRead(DATA_PINS[i])){
            data |= ((uint8_t) (1 << i));
        }
    }

    return data;
}

static bool read_rw_mode(void)
{
    return digitalRead(RW_PIN);
}

static void clock_tick(void)
{
    uint16_t addr = read_addr();
    uint8_t data = read_data();

    char buf[256] = { 0 };
    sprintf(buf, "%04" PRIx16, addr);

    Serial.print(buf);
    Serial.print(": ");

    Serial.print(read_rw_mode() ? "R " : "W ");

    sprintf(buf, "%02" PRIx8, data);

    Serial.print(buf);
    Serial.println();
}
