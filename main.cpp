#include "mbed.h"
#include "DS1820.h"

#define DATA_PIN        PA_0 
#define MAX_PROBES      1
 
Thread      data_thread;
EventQueue          queue;
 
DS1820* probe[MAX_PROBES];

DigitalOut led1(LED1);

/*
    quene 3sec.
*/
void send_temperature()
{
    float c = 0x111;
    unsigned char cc = 0;
 
    cc = ((unsigned char) c);
    printf("Temperature_1=%d \r\n", cc);
}

/*
    Booting one time Thread.
*/
void data_thread_fun()
{
    printf("thread start\r\n");

    queue.call_every(3000, send_temperature);
    //queue.call_every(3000, send_temperature_2);
    //queue.call_every(3000, send_temperature_3);
    //queue.call_every(3000, send_temperature_4);    

    queue.dispatch();    
}

// main() runs in its own thread in the OS
int main() {
    int num_devices = 0;
    
    /* function to see if there are ds1820 devices left on a pin which do not have a corresponding ds1820 object */
    while(DS1820::unassignedProbe(DATA_PIN)) {
        probe[num_devices] = new DS1820(DATA_PIN);
        num_devices++;
        
        if (num_devices == MAX_PROBES)
            break;
    }    
 
    printf("Found %d device(s)\r\n\n", num_devices);
  
    data_thread.start(data_thread_fun); 
    
    while(1) {
        probe[0]->convertTemperature(true, DS1820::all_devices);                //Start temperature conversion, wait until ready
        
        for (int i = 0; i<num_devices; i++){
            printf("Device %d returns %3.1foC\r\n", i, probe[i]->temperature());
        }
        printf("\r\n");
        wait(1);
    }

//    while (true) {
//       led1 = !led1;
//        wait(0.5);
//    }
}

