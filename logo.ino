void kayan_logo(){
  String text = "AY MAKINA   "; // Sonuna boşluk ekledik ki kayarken boşluk gelsin
  int displayWidth = 20;
  for (int i = 0; i < text.length() + displayWidth; i++) {
    lcd.setCursor(0, 1); // 2. satır
    String toDisplay = "";

    // Kayan yazıyı oluştur
    for (int j = 0; j < displayWidth; j++) {
      int charIndex = i + j - displayWidth;
      if (charIndex >= 0 && charIndex < text.length()) {
        toDisplay += text[charIndex];
      } else {
        toDisplay += " "; // Boşluk doldur
      }
    }

    lcd.print(toDisplay);
    delay(300); // Kayma hızı, isteğe göre azaltıp çoğaltabilirsin
  }
}

void sabit_logo(){
  String text = "AY MAKINA";
  int totalColumns = 20;
  int startingCol = (totalColumns - text.length()) / 2;

  lcd.setCursor(startingCol, 1); // 1. satır, tam ortalanmış
  lcd.print(text);
}