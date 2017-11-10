#include "rotary_encoder.h"

void InitializeRotaryEncoderPortPins(void)
{
    SET_ROTARY_ENCODER_A_AS_AN_INPUT;
    SET_ROTARY_ENCODER_B_AS_AN_INPUT;
}

EncoderState RotaryEncoder(EncoderDefinitions *myRotorEncoder, EncoderState *myEncoderState)
{
    EncoderState NextEncoderState;

    switch (*myEncoderState) {
    case Initialize:
        if(myRotorEncoder->Reset == High){
            NextEncoderState = Initialize;
        }
        else if(myRotorEncoder->SwitchA == High && myRotorEncoder->SwitchB == High){
            NextEncoderState = AhighBhigh;
        }
        else if(myRotorEncoder->SwitchA == High && myRotorEncoder->SwitchB == Low){
            NextEncoderState = AhighBlow;
        }
        else if(myRotorEncoder->SwitchA == Low && myRotorEncoder->SwitchB == High){
            NextEncoderState = AlowBhigh;
        }
        else if(myRotorEncoder->SwitchA == Low && myRotorEncoder->SwitchB == Low){
            NextEncoderState = AlowBlow;
        }
        myRotorEncoder->RotaryEncoderStateCount=0;
        break;

    case AlowBlow:
       if(myRotorEncoder->Reset == High){
            NextEncoderState = Initialize;
            myRotorEncoder->RotaryEncoderStateCount=0;
       }
     else if(myRotorEncoder->SwitchA == High && myRotorEncoder->SwitchB == High){
            NextEncoderState = AlowBlow;
     }
     else if(myRotorEncoder->SwitchA == High && myRotorEncoder->SwitchB == Low){
            NextEncoderState = AhighBlow;
            myRotorEncoder->RotaryEncoderStateCount = myRotorEncoder->RotaryEncoderStateCount+1;
        }
     else if(myRotorEncoder->SwitchA == Low && myRotorEncoder->SwitchB == High){
            NextEncoderState = AlowBhigh;
            myRotorEncoder->RotaryEncoderStateCount = myRotorEncoder->RotaryEncoderStateCount-1;
        }
     else if(myRotorEncoder->SwitchA == Low && myRotorEncoder->SwitchB == Low){
            NextEncoderState = AlowBlow;
        }
        break;

    case AlowBhigh:
        if(myRotorEncoder->Reset == High){
            NextEncoderState = Initialize;
            myRotorEncoder->RotaryEncoderStateCount=0;
       }
        else if(myRotorEncoder->SwitchA == High && myRotorEncoder->SwitchB == High){
            NextEncoderState = AhighBhigh;
            myRotorEncoder->RotaryEncoderStateCount = myRotorEncoder->RotaryEncoderStateCount-1;
        }
        else if(myRotorEncoder->SwitchA == Low && myRotorEncoder->SwitchB == High){
            NextEncoderState = AlowBhigh;
        }
        else if(myRotorEncoder->SwitchA == Low && myRotorEncoder->SwitchB == High){
            NextEncoderState = AlowBhigh;
        }
        else if(myRotorEncoder->SwitchA == Low && myRotorEncoder->SwitchB == Low){
            NextEncoderState = AlowBlow;
            myRotorEncoder->RotaryEncoderStateCount = myRotorEncoder->RotaryEncoderStateCount+1;
        }
        break;

    case AhighBlow:
        if(myRotorEncoder->Reset == High){
            NextEncoderState = Initialize;
            myRotorEncoder->RotaryEncoderStateCount=0;
       }
        else if(myRotorEncoder->SwitchA == High && myRotorEncoder->SwitchB == High){
            NextEncoderState = AhighBhigh;
            myRotorEncoder->RotaryEncoderStateCount = myRotorEncoder->RotaryEncoderStateCount+1;
        }
        else if(myRotorEncoder->SwitchA == High && myRotorEncoder->SwitchB == Low){
            NextEncoderState = AhighBlow;
        }
        else if(myRotorEncoder->SwitchA == High && myRotorEncoder->SwitchB == Low){
            NextEncoderState = AhighBlow;
        }
        else if(myRotorEncoder->SwitchA == Low && myRotorEncoder->SwitchB == Low){
            NextEncoderState = AlowBlow;
            myRotorEncoder->RotaryEncoderStateCount = myRotorEncoder->RotaryEncoderStateCount-1;
        }
        break;

    case AhighBhigh:
        if(myRotorEncoder->Reset == High){
            NextEncoderState = Initialize;
            myRotorEncoder->RotaryEncoderStateCount=0;
       }
        else if(myRotorEncoder->SwitchA == High && myRotorEncoder->SwitchB == High){
            NextEncoderState = AhighBhigh;
        }
        else if(myRotorEncoder->SwitchA == High && myRotorEncoder->SwitchB == Low){
            NextEncoderState = AhighBlow;
            myRotorEncoder->RotaryEncoderStateCount = myRotorEncoder->RotaryEncoderStateCount-1;
        }
        else if(myRotorEncoder->SwitchA == Low && myRotorEncoder->SwitchB == High){
            NextEncoderState = AlowBhigh;
            myRotorEncoder->RotaryEncoderStateCount = myRotorEncoder->RotaryEncoderStateCount+1;
        }
        else if(myRotorEncoder->SwitchA == High && myRotorEncoder->SwitchB == High){
            NextEncoderState = AhighBhigh;
        }
        break;

    default:
        NextEncoderState = Initialize;
    }

    return NextEncoderState;
}
