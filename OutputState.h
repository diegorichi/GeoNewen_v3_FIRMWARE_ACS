
#ifndef output_state__
#define output_state__
#include "vars.h"
#include "Pinout.h"

class OutputState {
  private:
  int valorDOBombas;
  int valorDOCalentador;
  int valorDOV4V;
  int valorDOCompressor;
  int valorDOVACS;
  Pinout* pinoutConfiguration;

#define ALLOWED_OUTPUT_VALUES ((_valor == HIGH) || (_valor == LOW))

  public:
  OutputState(Pinout* configuration) {
    this->pinoutConfiguration = configuration;
  }

  void writeDigitalOuputs() {
    digitalWrite(this->pinoutConfiguration->DO_BOMBAS, this->valorDOBombas);
    digitalWrite(this->pinoutConfiguration->DO_CALENTADOR, this->valorDOCalentador);
    digitalWrite(this->pinoutConfiguration->DO_COMPRESSOR, this->valorDOCompressor);
    digitalWrite(this->pinoutConfiguration->DO_VALVULA_4_VIAS, this->valorDOV4V);
    digitalWrite(this->pinoutConfiguration->DO_VALVULA_ACS, this->valorDOVACS);
  };

  void setDOBombas(int _valor) {
    if ALLOWED_OUTPUT_VALUES
      this->valorDOBombas = _valor;
  }

  void setDOCalentador(int _valor) {
    if ALLOWED_OUTPUT_VALUES
      this->valorDOCalentador = _valor;
  }
  void setDOV4V(int _valor) {
    if ALLOWED_OUTPUT_VALUES
      this->valorDOV4V = _valor;
  }
  void setDOVACS(int _valor) {
    if ALLOWED_OUTPUT_VALUES
      this->valorDOVACS = _valor;
  }
  void setDOCompressor(int _valor) {
    if ALLOWED_OUTPUT_VALUES
      this->valorDOCompressor = _valor;
  }
};

#endif
