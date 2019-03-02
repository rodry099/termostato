void guarda_config(){
  /*
  creando bufer para archivo Json
  */
  const int capacidad = JSON_OBJECT_SIZE(18);
  StaticJsonBuffer<capacidad> jb;
  //crea un objeto JsonObject
  JsonObject& configuracion = jb.createObject();

  server.send(200, "text/html", "Recibidos los datos correctamente");

  Serial.print("escribiendo datos");
  SPIFFS.remove("/configuracion.txt"); //borra archivo. sino se añadirian los datos
  File setup = SPIFFS.open("/configuracion.txt", "w");
  if(!setup){
    Serial.print("error al abrir archivo setup");
  }
  //guarda datos de configuracion en el JsonObject
  configuracion["ssid"] = server.arg("ssid");
  configuracion["password"] = server.arg("password");
  configuracion["ip"] = server.arg("ip");
  configuracion["id"] = server.arg("id");
  configuracion["canales"] = server.arg("canal");
  configuracion["servidor"] = server.arg("Server");
  configuracion["categoria"] = server.arg("categoria");
  configuracion["servidorNodeRed"] = "";
  configuracion["registrado"] = 0;
  //*************************************************
  configuracion.prettyPrintTo(setup);
  setup.close();
  ESP.restart();
}
