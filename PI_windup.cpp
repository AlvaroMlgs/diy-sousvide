#include "Arduino.h"
#include "PI_windup.h"
#include "misc.h"

float windup_integral = 0;
float windup_prev_i_signal;
float output_float;         // Output from the controller before saturation
uint16_t output_percent;    // Output after saturation

uint16_t windupOut(float ref,float state){

    float out_p = PI_windup_kp*(ref-state);
    float signal_i = PI_windup_ki*(ref-state);
    float out_i = windup_integrate(windup_integral,windup_prev_i_signal,signal_i);
    windup_prev_i_signal=signal_i;
    printVal("op",out_p);
    printVal("oi",out_i);

    output_float=out_p+out_i;
    output_percent;
    if (output_float>100)
        output_percent = 100;
    else if (output_float<0)
        output_percent = 0;
    else
        output_percent = (uint16_t)output_float;

    return output_percent;
}

float windup_integrate(float previous_int,float previous_signal,float current_signal){
    float period = (float)main_cycle_period/pow(10,6);  // Convert to seconds
    // Trapezoid integration
    float new_int=previous_int+(previous_signal+current_signal)/2*period;
    /* Correct windup with the output values from last loop
       Only add negative values, so that cool down is not affected */
    new_int=new_int+PI_windup_kw*min((float)0,output_percent-output_float);
    windup_integral=new_int;    // Update global variable
    return new_int;
}
