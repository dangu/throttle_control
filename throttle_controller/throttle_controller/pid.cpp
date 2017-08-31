#include "Arduino.h"
#include "pid.hpp"

PID::PID()
{
	  _P = 0;
	  _I = 0;
	  _D = 0;
	  _eOld = 0;
	  _u = 0;
	  _uLimited = 0;

	  _P_gain = 0;
	  _I_gain = 0;
	  _D_gain = 0;

	  _uMax = 0;
	  _uMin = -_uMax;
	  _millisOld = millis();
	  _sampleTime = 10;
}

void PID::init()
{
  _P = 0;
  _I = 0;
  _D = 0;
  _eOld = 0;

  _P_gain = 1;
  _I_gain = 0;
  _D_gain = 0;

  _uMax = 100;
  _uMin = -_uMax;
  _millisOld = millis();
  _sampleTime = 10;
}

bool PID::calculate(double ref, double in)
{
  double e;
  int millisNow;
  int T;           //!< [ms] Actual sample time

  millisNow = millis();
  T = millisNow - _millisOld;
  // Tsample is the desired sample time
  if(T >= _sampleTime)
    {
      e = in - ref;

      _P = e * _P_gain;
      _I = _I + e * _I_gain*T/1000.0;
      _D = _D_gain*(e-_eOld);

      _u = _P + _I +_D;
      if (_u > _uMax)
	{
	  _uLimited = _uMax;
	}
      else if (_u < _uMin)
	{
	  _uLimited = _uMin;
	}
      else
	{
	  _uLimited = _u;
	}

      if(_I<_uMin)
      {
    	  _I=_uMin;
      }
      else if(_I>_uMax)
      {
    	  _I = _uMax;
      }
      //_I = _I - (_u - _uLimited);
      
      //  Serial.print("T ");
      //  Serial.println(T);
      // debugPrint(ref, in);


      _millisOld = millisNow;
      _eOld = e;
      return true; 
    }
  else
    {
      return false;
    }
}

/** @brief Returns the output value */
double PID::getOutput()
{
  return _uLimited;
}

/** @brief Print some values for debugging */
void PID::debugPrint(double ref, double in)
{
  Serial.print(ref);
  Serial.print(" ");
  Serial.print(in);
  Serial.print(" ");
  Serial.print(_P);
  Serial.print(" ");
  Serial.print(_I);
  Serial.print(" ");
  Serial.print(_D);
  Serial.print(" ");
  Serial.print(_u);
  Serial.print(" ");
  Serial.print(_uLimited);
  Serial.print("\n");
}
