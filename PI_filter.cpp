#include "Arduino.h"
#include "PI_filter.h"

float filter_integral=0;
float filter_prev_i_signal;

uint16_t filterOut(float ref,float state){

    float out_p = PI_filter_kp*(ref-state);
    float signal_i = PI_filter_ki*(ref-state);
    float out_i=filter_integrate(filter_integral,filter_prev_i_signal,signal_i);
    filter_prev_i_signal=signal_i;

    float output_float=out_p+out_i;
    uint16_t output_percent;
    if (output_float>100)
        output_percent = 100;
    else if (output_float<0)
        output_percent = 0;
    else
        output_percent = (uint16_t)output_float;

    return output_percent;
}

float filter_integrate(float previous_int,float previous_signal,float current_signal){
    float period = (float)main_cycle_period/pow(10,6);  // Convert to seconds
    // Trapezoid integration
    float new_int = previous_int+(previous_signal+current_signal)/2*period;
    // Only integrate if within the limits (min,max)
    if (new_int>PI_filter_min && new_int<PI_filter_max){
        previous_int=new_int;
    }
    else{   // Otherwise integral stays constant
        new_int=previous_int;
    }
    filter_integral=new_int;    // Update global variable
    return new_int;
}
