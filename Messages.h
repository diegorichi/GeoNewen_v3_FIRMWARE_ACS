#ifndef messages__
#define messages__
#include <Arduino.h>

typedef struct {
    uint16_t code;
    char* lang;
    char* value;
} LangDictionary;


#define ENGLISH "EN"
#define SPANISH "ES"

const PROGMEM LangDictionary dictionary [ ]{
    {100, SPANISH, "     KUME NEWEN     "},
    {101, SPANISH, "   BOMBA DE CALOR   "},
    {102, SPANISH, "    GeoNewen v3.0   "},
    {103, SPANISH, "   Presione ENTER   "},

    //uno
    {111, SPANISH, "MONITOR DE TEMPS Y"},
    {112, SPANISH, "CONSUMO DE ENERGIA"},

    //dos
    {113, SPANISH, "CONFIGURACION DEL"},
    {114, SPANISH, "SISTEMA"},

    //cuatro
    {116, SPANISH, "MONITOR DE ALARMAS "},
    //cinco
    {117, SPANISH, "REGISTRO DE ALARMAS"},

    //dos cero
    {115, SPANISH, "MODO FRIO / CALOR  "},

    //dos uno
    {131, SPANISH, "CONFIGURACION DE"},
    {132, SPANISH, "TEMPERATURA DE ACS "},

    // dos tres
    {133, SPANISH, "HABILITACION DE    "},
    {134, SPANISH, "ALARMAS DE CAUDAL  "},

    // dos cuatro
    {135, SPANISH, "ENCENDIDO/APAGADO  "},

    //dos ocho
    {136, SPANISH, "HABILITACION DE ACS"},

    //dos nueve
    {137, SPANISH, "ACS ELECTRICO      "},

    //dos uno cero
    {151, SPANISH, "MODIFIQUE VALOR    "},

    //dos ocho uno
    {152, SPANISH, "ACS CON DELTA ELECT"},

    //cuatro cero
    {153, SPANISH, "ALARMA ACTIVA:"},
    {154, SPANISH, "ENTER resetea"},

    //cinco cero
    {155, SPANISH, "ULT. ALARMA ACTIVA"},
    {156, SPANISH, "ENTER para borrar"},


    {161, SPANISH, "Valor Actual:"},
    {162, SPANISH, "Nuevo Valor: "},

    {163, SPANISH, "Enter modifica"},

    //unocero
    {170, SPANISH, "MODO:              "},
    {171, SPANISH, "Com:"},
    {172, SPANISH, "Adm:"},
    {173, SPANISH, "Des:"},
    {174, SPANISH, "ACS:"},
    {175, SPANISH, "TiH:"},
    {176, SPANISH, "ToH:"},
    {177, SPANISH, "TiT:"},
    {178, SPANISH, "ToT:"},
    {179, SPANISH, "CauT:"},
    {180, SPANISH, "CauH:"},


    {181, SPANISH, "     "},
    {182, SPANISH, "FRIO "},
    {183, SPANISH, "CALOR"},
    {184, SPANISH, "ALTA/O"},
    {185, SPANISH, "BAJA/O"},
    {186, SPANISH, "ON "},
    {187, SPANISH, "OFF"},

    //Alarmas
    {190, SPANISH, "No hay alarmas act."},

    {191, SPANISH, "La temperatura del "},
    {192, SPANISH, "compresor es       "},

    {193, SPANISH, "El caudal de       "},
    {194, SPANISH, "HOGAR es           "},
    {195, SPANISH, "TIERRA es          "},

    {196, SPANISH, "Presion Pres.HI es "},
    {197, SPANISH, "Presion Pres.LOW es"},

    {198, SPANISH, "Temp ADM elevada"},
    {199, SPANISH, "Temp DES elevada"},



};

class Messages {
    public:
    char* getMessage(uint16_t _code, char* _lang) {
        for (uint16_t i = 0; i < sizeof(dictionary); ++i) {
            if ((dictionary [i].code == _code) &&
                (dictionary [i].lang == _lang)
                ) {
                return dictionary [i].value;
            }
        }
        return "";

    }
};

#endif
