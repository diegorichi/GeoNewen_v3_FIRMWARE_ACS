# GeoNewen_v3_ACS_DiegoRichi_DR2.0

Calentar Agua:
Valor_DO_V4V = HIGH; // si la temp ACS es baja, abre la valvula de 3v para calentar el agua
Valor_DO_Bombas = HIGH;
Valor_DO_VACS = HIGH; // modo calor


ModoFrioCalor == true (modo frio)        
        Valor_DO_VACS = LOW; //frio


Si no anda && no ACS && frio -> VACS = LOW
Si no anda && no ACS && Calor -> VACS = HIGH


Valor_DO_VACS = LOW -> losa
Valor_DO_VACS = HIGH -> ACS

machine control ok

measurement and calculation

connectivity ok

stateMachine

alarm ok
