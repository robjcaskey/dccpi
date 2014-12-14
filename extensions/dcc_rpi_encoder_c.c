/*
    dcc_rpi_encoder_c.c - Uses wiringPI delayMicrosecondsHard() to
    generate a DCC protocol signal.

    Copyright (C) 2014  Hector Sanjuan

    This file is part of "dccpi".

    "dccpi" is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    "dccpi" is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with "dccpi".  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Python.h>
#include <wiringPi.h>
int PIN_BREAK = 3;
int PIN_A = 1;
int PIN_B = 2;
//This one is not exposed
extern void delayMicrosecondsHard (unsigned int howLong);

static PyObject * dcc_rpi_encoder_c_set_pins(PyObject *self, PyObject *args){
    if (!PyArg_ParseTuple(args, "iii", &PIN_A,
                          &PIN_B,
                          &PIN_BREAK))
        return NULL;
    pinMode(PIN_A, OUTPUT);
    digitalWrite(PIN_A, LOW);
    pinMode(PIN_B, OUTPUT);
    digitalWrite(PIN_B, LOW);
    pinMode(PIN_BREAK, OUTPUT);
    digitalWrite(PIN_BREAK, LOW);
    Py_RETURN_NONE;
}

static PyObject * dcc_rpi_encoder_c_send_bit_array(PyObject *self, PyObject *args){
    char const *bit_array;
    char const *bit_array_pos;
    const int count;
    const unsigned int bit_one_part_duration;
    const unsigned int bit_zero_part_duration;
    const unsigned int packet_separation;
    int i;

    if (!PyArg_ParseTuple(args, "siIII", &bit_array, &count,
                          &bit_one_part_duration,
                          &bit_zero_part_duration,
                          &packet_separation))
        return NULL;

    for (i = 0; i < count; i++){
        bit_array_pos = bit_array;
        while (*bit_array_pos){ //string will be null terminated
            if (*bit_array_pos == '0'){
                //Encode 0 with 100us for each part
                digitalWrite(PIN_A, LOW);
                delayMicrosecondsHard(bit_zero_part_duration);
                digitalWrite(PIN_A, HIGH);
                delayMicrosecondsHard(bit_zero_part_duration);
            }
            else if (*bit_array_pos == '1'){
                //Encode 1 with 58us for each part
                digitalWrite(PIN_A, LOW);
                delayMicrosecondsHard(bit_one_part_duration);
                digitalWrite(PIN_A, HIGH);
                delayMicrosecondsHard(bit_one_part_duration);
            } else {
                // Interpret this case as packet end char.
                // Standard says we should wait 5ms at least
                // and 30ms max between packets.
                digitalWrite(PIN_A, LOW);
                delay(packet_separation); //ms
                digitalWrite(PIN_A, HIGH);
            }
            bit_array_pos++;
        }
    }

    Py_RETURN_NONE;
}

static PyObject * dcc_rpi_encoder_c_brake(PyObject *self, PyObject *args){
    const int brake;
    if (!PyArg_ParseTuple(args, "I", &brake))
        return NULL;

    if (brake == 1)
        digitalWrite(PIN_BREAK, LOW);
    else
        digitalWrite(PIN_BREAK, HIGH);

    Py_RETURN_NONE;
}

static PyMethodDef DCCRPiEncoderMethods[] = {
    {"send_bit_array", dcc_rpi_encoder_c_send_bit_array, METH_VARARGS,
     "Send some bits to the tracks"},
    {"set_pins", dcc_rpi_encoder_c_set_pins, METH_VARARGS,
     "Specify pins for bit banging"},
    {"brake", dcc_rpi_encoder_c_brake, METH_VARARGS,
     "Enable or disable a brake signal"},
    {NULL, NULL, 0, NULL} /* Sentinel - whatever that means */
};

PyMODINIT_FUNC initdcc_rpi_encoder_c(void){
    wiringPiSetup();
    digitalWrite(0, LOW);
    digitalWrite(1, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW); 
    digitalWrite(5, LOW); 
    digitalWrite(6, LOW);
    digitalWrite(7, LOW); //green
    digitalWrite(8, LOW); // yellow
    digitalWrite(9, LOW); // blue
    digitalWrite(10, LOW);
    digitalWrite(11, LOW); 
    digitalWrite(12, LOW); 
    digitalWrite(13, LOW); 
    digitalWrite(14, LOW); 
    digitalWrite(15, LOW); 
    digitalWrite(16, LOW); 
    digitalWrite(17, LOW); 
    digitalWrite(18, LOW); 
/*
    digitalWrite(19, LOW); 
    digitalWrite(20, LOW); 
*/
    Py_InitModule("dcc_rpi_encoder_c", DCCRPiEncoderMethods);
}
