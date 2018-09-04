#include "Arduino.h"
#include "PI_weight.h"
#include "misc.h"

float weight_integral = 0;
float weight_prev_i_signal;

uint16_t weightOut(float ref,float state){
    float error_p = PI_weight_wp*ref - state;
    float error_i = PI_weight_wi*ref - state;
    printVal("ep",error_p);
    printVal("ei",error_i);

    float out_p = PI_weight_kp*error_p;
    float signal_i = PI_weight_ki*error_i;
    float out_i = weight_integrate(weight_integral,weight_prev_i_signal,signal_i);
    weight_prev_i_signal=signal_i;
    printVal("op",out_p);
    printVal("oi",out_i);

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

float weight_integrate(float previous_int,float previous_signal,float current_signal){
    float period = (float)main_cycle_period/pow(10,6);  // Convert to seconds
    // Trapezoid integration
    float new_int=previous_int+(previous_signal+current_signal)/2*period;
    weight_integral=new_int;    // Update global variable
    return new_int;
}
