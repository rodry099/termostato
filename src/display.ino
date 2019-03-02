void displayTemp(){
  String Ubicacion = id_disp;
  Ubicacion.toUpperCase();
  u8g2.firstPage();
    do {
      u8g2.drawXBM(1, 1, wifiOn_width, wifiOn_height, wifiOn_bits);
      u8g2.setFont(u8g2_font_lastapprenticebold_tr);
      u8g2.drawStr(45, 15, Ubicacion.c_str());
      u8g2.drawXBM(5, 28, termometro_width, termometro_height, termometro_bits);
      u8g2.setFont(u8g2_font_fub20_t_symbol);
      u8g2.drawStr(30, 50, temperatura.c_str());
      u8g2.setFont(u8g2_font_helvB12_tf);
      u8g2.drawStr(90, 43, "*C");
    } while ( u8g2.nextPage() );
}
