#include "degerler.h"


bool encoderOkuma() {
  long newPosition = myEnc.read() / 4; // Encoder hassasiyetini ayarlayın
  
  if (newPosition != oldPosition) {
    if (editMode) {
      // Edit modunda değer değiştirme
      editValue += (newPosition > oldPosition) ? 1 : -1;
      editValue = constrain(editValue, 0, 1000); // Değer aralığını sınırla
    } else {
      // Menü gezinme modu
      int direction = (newPosition > oldPosition) ? 1 : -1;
      menuIndex += direction;
      
      // Menü indeksini sınırla
      menuIndex = constrain(menuIndex, 0, currentMenu->itemCount - 1);
      
      // Görüntülenen öğeleri güncelle
      if (menuIndex < topItemIndex) {
        topItemIndex = menuIndex;
      } else if (menuIndex >= topItemIndex + 3) {
        topItemIndex = menuIndex - 2;
      }
    }
    
    oldPosition = newPosition;
    return true;
  }
  return false;
}

void butonOkuma() {
  // Buton ile alt menüye girme veya geri
  if (digitalRead(BTN) == LOW) {
    buttonPressCount++; // Buton basım sayacını artır
    lastUpdateTime = millis(); // Ekranı güncelle
    
    // BİLGİ EKRANI ÖZEL DURUMU - Bilgi ekranındayken butona basınca ana menüye git
    if (infoScreenMode) {
      infoScreenMode = false; // Bilgi ekranı modundan çık
      currentMenu = &mainMenu;
      menuIndex = 0;
      topItemIndex = 0;
      displayMenu();
      delay(300);
      return;
    }

    const char* selectedItem = currentMenu->items[menuIndex];

    if (editMode) {
      // Düzenlemeyi onayla ve EEPROM'a kaydet
      editMode = false;
      
      // Düzenlenen değeri ilgili değişkene ata ve EEPROM'a kaydet
      if (currentMenu == &xEkseni) {
        switch(editIndex) {
          case 1: 
            degerler.xHiz = editValue;
            degeriEEPROMaYaz(X_HIZ_ADDR, editValue);
            break;
          case 2: 
            degerler.xIvme = editValue;
            degeriEEPROMaYaz(X_IVME_ADDR, editValue);
            break;
          case 3: 
            degerler.xAdim = editValue;
            degeriEEPROMaYaz(X_ADIM_ADDR, editValue);
            break;
        }
      }
      else if (currentMenu == &zEkseni) {
        switch(editIndex) {
          case 1: 
            degerler.zHiz = editValue;
            degeriEEPROMaYaz(Z_HIZ_ADDR, editValue);
            break;
          case 2: 
            degerler.zIvme = editValue;
            degeriEEPROMaYaz(Z_IVME_ADDR, editValue);
            break;
          case 3: 
            degerler.zAdim = editValue;
            degeriEEPROMaYaz(Z_ADIM_ADDR, editValue);
            break;
        }
      }
      else if (currentMenu == &komut) {
        switch(editIndex) {
          case 1: 
            degerler.zInme = editValue;
            degeriEEPROMaYaz(Z_INME_ADDR, editValue);
            break;
          case 2: 
            degerler.zBekleme = editValue;
            degeriEEPROMaYaz(Z_BEKLEME_ADDR, editValue);
            break;
          case 3: 
            degerler.xCekme = editValue;
            degeriEEPROMaYaz(X_CEKME_ADDR, editValue);
            break;
          case 4: 
            degerler.xBekleme = editValue;
            degeriEEPROMaYaz(X_BEKLEME_ADDR, editValue);
            break;
          case 5: 
            degerler.zCikma = editValue;
            degeriEEPROMaYaz(Z_CIKMA_ADDR, editValue);
            break;
        }
      }
      
      currentMenu = editMenu;
      topItemIndex = 0;
      
      // Kayıt onay mesajı göster
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Kaydedildi!");
      delay(1000);
    }
    else {
      if (strcmp(selectedItem, "Geri") == 0) {
        if (currentMenu->parent != NULL) {
          currentMenu = currentMenu->parent;
        } else {
          // Ana menüden geriye basılırsa bilgi ekranına dön
          infoScreenMode = true;
          currentMenu = &mainMenu;
        }
      } 
      // ANA MENÜ SEÇENEKLERİ
      else if (currentMenu == &mainMenu) {
        if (strcmp(selectedItem, "Ayarlar") == 0) {
          currentMenu = &ayarlar;
        }
        else if (strcmp(selectedItem, "Manuel Kontrol") == 0) {
          // Manuel kontrol menüsüne git (eğer varsa)
          lcd.clear();
          lcd.setCursor(0, 1);
          lcd.print("Manuel Mod Aciladi");
          delay(1000);
        }
      }
      // AYARLAR MENÜSÜ SEÇENEKLERİ
      else if (currentMenu == &ayarlar) {
        if (strcmp(selectedItem, "Start Ayarlari") == 0) {
          currentMenu = &start;
        }
        else if (strcmp(selectedItem, "Reset Ayarlari") == 0) {
          varsayilanDegerleriYukle();
          lcd.clear();
          lcd.setCursor((20 - String("Varsayilanlar").length()) / 2, 1);
          lcd.print("Varsayilanlar");
          lcd.setCursor((20 - String("Yuklendi").length()) / 2, 2);
          lcd.print("Yuklendi");
          delay(2000);
        }
      }
      // START AYARLARI SEÇENEKLERİ
      else if (currentMenu == &start) {
        if (strcmp(selectedItem, "X Ekseni Ayarlari") == 0) {
          currentMenu = &xEkseni;
        }
        else if (strcmp(selectedItem, "Z Ekseni Ayarlari") == 0) {
          currentMenu = &zEkseni;
        }
        else if (strcmp(selectedItem, "Komut Ayarlari") == 0) {
          currentMenu = &komut;
        }
      }
      
      // Düzenleme modu için sayısal öğe seçildi
      else if ((currentMenu == &xEkseni || currentMenu == &zEkseni || currentMenu == &komut) && menuIndex > 0) {
        editMode = true;
        editMenu = currentMenu;
        editIndex = menuIndex;
        
        // Başlangıç değerlerini EEPROM'dan al
        if (currentMenu == &xEkseni) {
          switch(menuIndex) {
            case 1: editValue = degerler.xHiz; break;
            case 2: editValue = degerler.xIvme; break;
            case 3: editValue = degerler.xAdim; break;
          }
        }
        else if (currentMenu == &zEkseni) {
          switch(menuIndex) {
            case 1: editValue = degerler.zHiz; break;
            case 2: editValue = degerler.zIvme; break;
            case 3: editValue = degerler.zAdim; break;
          }
        }
        else if (currentMenu == &komut) {
          switch(menuIndex) {
            case 1: editValue = degerler.zInme; break;
            case 2: editValue = degerler.zBekleme; break;
            case 3: editValue = degerler.xCekme; break;
            case 4: editValue = degerler.xBekleme; break;
            case 5: editValue = degerler.zCikma; break;
          }
        }
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
  // Eğer bilgi ekranı modundaysak
  if (infoScreenMode) {
    // Tüm satırları temizle
    for (int i = 0; i < 4; i++) {
      lcd.setCursor(0, i);
      lcd.print("                    "); // 20 boşluk
    }
    
    // 1. satır: ==AY MAKINA==
    lcd.setCursor(0, 0);
    lcd.print("====AY MAKINA====");
    
    // 2. satır: Ana Menü (tıklanabilir)
    lcd.setCursor(0, 1);
    lcd.print("> Ana Menu");
    
    // 3. satır: Geçen süre
    lcd.setCursor(0, 2);
    unsigned long totalSeconds = millis() / 1000;
    unsigned long seconds = totalSeconds % 60;
    unsigned long minutes = (totalSeconds / 60) % 60;
    unsigned long hours = totalSeconds / 3600;
    
    lcd.print("Sure: ");
    if (hours < 10) lcd.print("0");
    lcd.print(hours);
    lcd.print(":");
    if (minutes < 10) lcd.print("0");
    lcd.print(minutes);
    lcd.print(":");
    if (seconds < 10) lcd.print("0");
    lcd.print(seconds);
    
    // 4. satır: Buton basım sayısı
    lcd.setCursor(0, 3);
    lcd.print("Urun: ");
    lcd.print(buttonPressCount);
    lcd.print(" adet");
    
    // Kalan boşluğu temizle
    int textLength = 7 + String(buttonPressCount).length() + 4;
    for (int i = textLength; i < 20; i++) {
      lcd.print(" ");
    }
  }
  else {
    // MENÜ MODU
    lcd.clear();
    
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
      
      // 3. satır: Değer
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
}
