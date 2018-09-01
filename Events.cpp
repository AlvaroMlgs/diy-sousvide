#include "Arduino.h"
#include "Events.h"

float last_error = 0;
uint32_t current_time = 0;
uint32_t last_time = 0;    // milliseconds
float events_integral = 0;
float events_prev_i_signal;

bool eventsTrigger(float ref,float state,uint32_t current_micros){
    float delta_error = (ref-state) - last_error;
    current_time = current_micros/1000;
    uint32_t delta_time = current_time - last_time;
    if ( abs(delta_error)>EVENTS_elim || delta_time>EVENTS_tlim ){
        return true;
    }
    else{
        return false;
    }
}

uint16_t eventsOut(float ref,float state){

    float out_p = EVENTS_kp*(ref-state);
    float signal_i = EVENTS_ki*(ref-state);
    float out_i = eventsIntegrate(events_integral,events_prev_i_signal,signal_i,millis()-last_time);
    events_prev_i_signal=signal_i;
    //Serial.print("p");
    //Serial.print(out_p);
    //Serial.print(",");
    //Serial.print("i");
    //Serial.print(out_i);
    //Serial.print(",");

    float output_float=out_p+out_i;
    uint16_t output_percent;
    if (output_float>100)
        output_percent = 100;
    else if (output_float<0)
        output_percent = 0;
    else
        output_percent = (uint16_t)output_float;

    last_error = ref-state;
    last_time = current_time;

    return output_percent;
}

float eventsIntegrate(float previous_int,float previous_signal,float current_signal,uint32_t delta_time){
    float period = (float)delta_time/1000;  // Convert to seconds
    // Trapezoid integration
    float new_int = previous_int+(previous_signal+current_signal)/2*period;
    // Only integrate if within the limits (min,max)
    if (new_int>EVENTS_imin && new_int<EVENTS_imax){
        previous_int=new_int;
    }
    else{   // Otherwise integral stays constant
        new_int=previous_int;
    }
    events_integral=new_int;    // Update global variable
    return new_int;

}

