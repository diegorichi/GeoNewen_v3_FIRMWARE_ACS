---

- Questions

---

- Go here:
  https://vscode.dev/github/diegorichi/GeoNewen_v3_FIRMWARE_ACS/blob/bb5e5cde767af0e8343cd77663365a1945331f94/functionsLCDMenu.cpp#L124
  Why the Temp_out_Tacu doen't exist?
  Should we calculate as average of measures?

- Go here:
  https://vscode.dev/github/diegorichi/GeoNewen_v3_FIRMWARE_ACS/blob/bb5e5cde767af0e8343cd77663365a1945331f94/thingspeak.cpp#L52
  Why we are publishing the instant values instead of average values.
  (Temp_in_H instead of Temp_in_Hacu)
  Take a look into lcd Functions and see that we are printing the average values.

---

- Changes

---

- Clean code
- remove unnecesary == true/false sentences of boolean flags
- remove duplicate code and move to routines.
- move to routine a code realted to WIFI stuff
- improvements to old keyboard implementation
- add menu class to move to new menu implementation.
- remove unused menu
- create menu structure and behavior

---

- Pending

---

- replace MenuActual by menuActivo
- test
- remove old menu references.
- refactor Estado_Maquina
- refactor alarm messages
