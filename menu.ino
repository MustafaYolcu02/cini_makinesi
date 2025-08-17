
bool encoderOkuma() {
  long newPosition = myEnc.read() / 4;
  if (newPosition != oldPosition) {
    if (editMode) {
      editValue += (int)(newPosition - oldPosition);
      if (editValue < 0) editValue = 0;
      if (editValue > 1000) editValue = 1000;
    } else {
      oldPosition = newPosition;
      menuIndex = newPosition % currentMenu->itemCount;
      if (menuIndex < 0) menuIndex += currentMenu->itemCount;

      if (menuIndex < topItemIndex) topItemIndex = menuIndex;
      else if (menuIndex >= topItemIndex + 3) topItemIndex = menuIndex - 2;
    }
    oldPosition = newPosition;
    return true;
  }
  return false;
}

void butonOkuma(){
  // Buton ile alt menüye girme veya geri
  if (digitalRead(BTN) == LOW) {
    const char* selectedItem = currentMenu->items[menuIndex];

    if (editMode) {
      // Düzenlemeyi onayla
      editMode = false;
      currentMenu = editMenu;
      topItemIndex = 0;
    }
    else {
      if (strcmp(selectedItem, "Geri") == 0) {
        if (currentMenu->parent != NULL) currentMenu = currentMenu->parent;
      } 
      else if (currentMenu == &mainMenu && strcmp(selectedItem, "Ayarlar") == 0) {
        currentMenu = &ayarlar;  // Ana menüden Ayarlar'a gir
      }
      else if (currentMenu == &ayarlar && strcmp(selectedItem, "Start Ayarlari") == 0) {
        currentMenu = &start;
      }
      else if (currentMenu == &start && strcmp(selectedItem, "X Ekseni Ayarlari") == 0) {
        currentMenu = &xEkseni;
      }
      else if (currentMenu == &start && strcmp(selectedItem, "Z Ekseni Ayarlari") == 0) {
        currentMenu = &zEkseni;
      }
      else if (currentMenu == &start && strcmp(selectedItem, "Komut Ayarlari") == 0) {
        currentMenu = &komut;
      }
      
      // Düzenleme modu için sayısal öğe seçildi
      else if ((currentMenu == &xEkseni || currentMenu == &zEkseni || currentMenu == &komut) && menuIndex > 0) {
        editMode = true;
        editMenu = currentMenu;
        editIndex = menuIndex;
        // Başlangıç değerleri (örnek)
        if (currentMenu == &xEkseni) editValue = 300;
        else if (currentMenu == &zEkseni) editValue = 100;
        else if (currentMenu == &komut) editValue = 50;
      }
    }
    menuIndex = 0;
    topItemIndex = 0;
    displayMenu();
    delay(300); // debounce
  }
}


// Menü gösterme fonksiyonu
void displayMenu() {
  // 1. satır başlık
  lcd.setCursor(0, 0);
  String title = String(currentMenu->title);
  int titleLength = title.length();
  int totalEquals = 20 - titleLength;
  
  if (totalEquals >= 2) {
    int leftEquals = totalEquals / 2;
    int rightEquals = totalEquals - leftEquals;
    
    for (int i = 0; i < leftEquals; i++) lcd.print("=");
    lcd.print(title);
    for (int i = 0; i < rightEquals; i++) lcd.print("=");
  }

  // Edit modu aktifse
  if (editMode && currentMenu == editMenu) {
    // 2. satır: Seçilen öğenin adı
    lcd.setCursor(0, 1);
    lcd.print("> ");
    lcd.print(currentMenu->items[editIndex]);
    for (int j = strlen(currentMenu->items[editIndex]) + 2; j < 20; j++) lcd.print(" ");
    
    // 3. satır: Değer (yanıp sönme efekti olmadan)
    lcd.setCursor(0, 2);
    lcd.print("  ");
    lcd.print(editValue);
    lcd.print("  ");
    for (int j = String(editValue).length() + 4; j < 20; j++) lcd.print(" ");
    
    // 4. satırı temizle
    lcd.setCursor(0, 3);
    lcd.print("                    ");
  } 
  else {
    // Normal menü modu
    for (int i = 0; i < 3; i++) {
      int item = topItemIndex + i;
      lcd.setCursor(0, i + 1);
      
      if (item >= currentMenu->itemCount) {
        lcd.print("                    ");
      } else {
        if (item == menuIndex) lcd.print(">");
        else lcd.print(" ");
        
        lcd.print(currentMenu->items[item]);
        for (int j = strlen(currentMenu->items[item]) + 1; j < 20; j++) lcd.print(" ");
      }
    }
  }
}