// menu.ino - Manuel Kontrol için düzeltilmiş versiyon
#include "degerler.h"

bool encoderOkuma() {
  long newPosition = myEnc.read() / 4; // Encoder hassasiyetini ayarlayın
  
  if (newPosition != oldPosition) {
    if (editMode) {
      // Edit modunda değer değiştirme
      editValue += (newPosition > oldPosition) ? 1 : -1;
      
      // Değer aralığını sınırla (farklı değerler için farklı sınırlar)
      if (currentMenu == &start || currentMenu == &home || currentMenu == &manuelAyarlari || currentMenu == &manuelKontrol) {
        
        // Manuel Kontrol stok değerleri (X ve Z stok)
        if (currentMenu == &manuelKontrol) {
          if (editIndex == 1 || editIndex == 2) { // X veya Z stok değerleri
            editValue = constrain(editValue, 0, 1000);
          }
        }
        // Diğer menüler için sınırlamalar
        else {
          // Hız, ivme, adım, mesafe değerleri
          if (editIndex >= 1 && editIndex <= 6) {
            editValue = constrain(editValue, 1, 1000);
          }
          // Z yavaşlama hızı
          else if (editIndex == 9) {
            editValue = constrain(editValue, 1, 1000);
          }
          // Bekleme süreleri
          else if (editIndex == 10 || editIndex == 12) {
            editValue = constrain(editValue, 0, 10000);
          }
          // Park pozisyonları
          else if (editIndex == 13 || editIndex == 14 || editIndex == 7 || editIndex == 8) {
            editValue = constrain(editValue, 0, 1000);
          }
          // Encoder ilerleme mesafesi
          else if (editIndex == 8 && currentMenu == &manuelAyarlari) {
            editValue = constrain(editValue, 1, 100);
          }
        }
      }
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
      if (currentMenu == &start) {
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
          case 4: 
            degerler.zHiz = editValue;
            degeriEEPROMaYaz(Z_HIZ_ADDR, editValue);
            break;
          case 5: 
            degerler.zIvme = editValue;
            degeriEEPROMaYaz(Z_IVME_ADDR, editValue);
            break;
          case 6: 
            degerler.zAdim = editValue;
            degeriEEPROMaYaz(Z_ADIM_ADDR, editValue);
            break;
          case 7: 
            degerler.zInme = editValue;
            degeriEEPROMaYaz(Z_INME_ADDR, editValue);
            break;
          case 8: 
            degerler.zYavaslamaMesafe = editValue;
            degeriEEPROMaYaz(Z_YAVASLAMA_MESAFE_ADDR, editValue);
            break;
          case 9: 
            degerler.zYavaslamaHiz = editValue;
            degeriEEPROMaYaz(Z_YAVASLAMA_HIZ_ADDR, editValue);
            break;
          case 10: 
            degerler.zBekleme = editValue;
            degeriEEPROMaYaz(Z_BEKLEME_ADDR, editValue);
            break;
          case 11: 
            degerler.xCekme = editValue;
            degeriEEPROMaYaz(X_CEKME_ADDR, editValue);
            break;
          case 12: 
            degerler.xCekmeSure = editValue;
            degeriEEPROMaYaz(X_CEKME_SURE_ADDR, editValue);
            break;
          case 13: 
            degerler.zPark = editValue;
            degeriEEPROMaYaz(Z_PARK_ADDR, editValue);
            break;
        }
      }
      else if (currentMenu == &home) {
        switch(editIndex) {
          case 1: 
            degerler.xAdim = editValue;
            degeriEEPROMaYaz(X_ADIM_ADDR, editValue);
            break;
          case 2: 
            degerler.xIvme = editValue;
            degeriEEPROMaYaz(X_IVME_ADDR, editValue);
            break;
          case 3: 
            degerler.xHiz = editValue;
            degeriEEPROMaYaz(X_HIZ_ADDR, editValue);
            break;
          case 4: 
            degerler.zAdim = editValue;
            degeriEEPROMaYaz(Z_ADIM_ADDR, editValue);
            break;
          case 5: 
            degerler.zIvme = editValue;
            degeriEEPROMaYaz(Z_IVME_ADDR, editValue);
            break;
          case 6: 
            degerler.zHiz = editValue;
            degeriEEPROMaYaz(Z_HIZ_ADDR, editValue);
            break;
          case 7: 
            degerler.xPark = editValue;
            degeriEEPROMaYaz(X_PARK_ADDR, editValue);
            break;
          case 8: 
            degerler.zPark = editValue;
            degeriEEPROMaYaz(Z_PARK_ADDR, editValue);
            break;
        }
      }
      else if (currentMenu == &manuelAyarlari) {
        switch(editIndex) {
          case 1: 
            degerler.xAdim = editValue;
            degeriEEPROMaYaz(X_ADIM_ADDR, editValue);
            break;
          case 2: 
            degerler.xIvme = editValue;
            degeriEEPROMaYaz(X_IVME_ADDR, editValue);
            break;
          case 3: 
            degerler.xHiz = editValue;
            degeriEEPROMaYaz(X_HIZ_ADDR, editValue);
            break;
          case 4: 
            degerler.zAdim = editValue;
            degeriEEPROMaYaz(Z_ADIM_ADDR, editValue);
            break;
          case 5: 
            degerler.zIvme = editValue;
            degeriEEPROMaYaz(Z_IVME_ADDR, editValue);
            break;
          case 6: 
            degerler.zHiz = editValue;
            degeriEEPROMaYaz(Z_HIZ_ADDR, editValue);
            break;
          case 7: 
            degerler.encoderIlerleme = editValue;
            degeriEEPROMaYaz(ENCODER_ILERLEME_ADDR, editValue);
            break;
        }
      }
      // MANUEL KONTROL DÜZELTMESİ - HATA BURADA
      else if (currentMenu == &manuelKontrol) {
        switch(editIndex) {
          case 1: 
            degerler.xManuel = editValue;  // X stok değerini kaydet
            degeriEEPROMaYaz(X_MANUEL_ADDR, editValue);
            break;
          case 2: 
            degerler.zManuel = editValue;  // Z stok değerini kaydet
            degeriEEPROMaYaz(Z_MANUEL_ADDR, editValue);
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
      // FABRİKA AYARLARI ONAY MENÜSÜ
      else if (currentMenu == &fabrikaOnay) {
        if (strcmp(selectedItem, "Evet") == 0) {
          varsayilanDegerleriYukle();
          // Tüm değerleri EEPROM'a yaz
          degeriEEPROMaYaz(X_HIZ_ADDR, degerler.xHiz);
          degeriEEPROMaYaz(X_IVME_ADDR, degerler.xIvme);
          degeriEEPROMaYaz(X_ADIM_ADDR, degerler.xAdim);
          degeriEEPROMaYaz(Z_HIZ_ADDR, degerler.zHiz);
          degeriEEPROMaYaz(Z_IVME_ADDR, degerler.zIvme);
          degeriEEPROMaYaz(Z_ADIM_ADDR, degerler.zAdim);
          degeriEEPROMaYaz(Z_INME_ADDR, degerler.zInme);
          degeriEEPROMaYaz(Z_YAVASLAMA_MESAFE_ADDR, degerler.zYavaslamaMesafe);
          degeriEEPROMaYaz(Z_YAVASLAMA_HIZ_ADDR, degerler.zYavaslamaHiz);
          degeriEEPROMaYaz(Z_BEKLEME_ADDR, degerler.zBekleme);
          degeriEEPROMaYaz(X_CEKME_ADDR, degerler.xCekme);
          degeriEEPROMaYaz(X_CEKME_SURE_ADDR, degerler.xCekmeSure);
          degeriEEPROMaYaz(Z_PARK_ADDR, degerler.zPark);
          degeriEEPROMaYaz(X_PARK_ADDR, degerler.xPark);
          degeriEEPROMaYaz(ENCODER_ILERLEME_ADDR, degerler.encoderIlerleme);
          degeriEEPROMaYaz(X_MANUEL_ADDR, degerler.xManuel);  // Manuel değerleri de kaydet
          degeriEEPROMaYaz(Z_MANUEL_ADDR, degerler.zManuel);
          
          lcd.clear();
          lcd.setCursor((20 - String("Fabrika Ayarlari").length()) / 2, 1);
          lcd.print("Fabrika Ayarlari");
          lcd.setCursor((20 - String("Yuklendi").length()) / 2, 2);
          lcd.print("Yuklendi");
          delay(2000);
        }
        // "Hayır" veya "Geri" seçeneği
        currentMenu = previousMenuBeforeFabrika;
        previousMenuBeforeFabrika = NULL;
      }
      // ANA MENÜ SEÇENEKLERİ
      else if (currentMenu == &mainMenu) {
        if (strcmp(selectedItem, "Ayarlar") == 0) {
          currentMenu = &ayarlar;
        }
        else if (strcmp(selectedItem, "Manuel Kontrol") == 0) {
          currentMenu = &manuelKontrol;
        }
      }
      // AYARLAR MENÜSÜ SEÇENEKLERİ
      else if (currentMenu == &ayarlar) {
        if (strcmp(selectedItem, "Start Ayarlari") == 0) {
          currentMenu = &start;
        }
        else if (strcmp(selectedItem, "Home Ayarlari") == 0) {
          currentMenu = &home;
        }
        else if (strcmp(selectedItem, "Manuel Ayarlari") == 0) {
          currentMenu = &manuelAyarlari;
        }
      }
      // START, HOME VE MANUEL AYARLARI MENÜLERİNDE FABRİKA AYARLARI SEÇENEĞİ
      else if ((currentMenu == &start || currentMenu == &home || currentMenu == &manuelAyarlari) && 
               strcmp(selectedItem, "Fabrika Ayarlarina Don") == 0) {
        previousMenuBeforeFabrika = currentMenu;
        currentMenu = &fabrikaOnay;
        fabrikaOnay.parent = currentMenu; // Geçici olarak parent'ı ayarla
      }
      // MANUEL KONTROL MENÜSÜ SEÇENEKLERİ
      else if (currentMenu == &manuelKontrol) {
        if (strcmp(selectedItem, "X Ekseni Hareketi") == 0) {
          // X ekseni stok değerini düzenleme moduna geç
          editMode = true;
          editMenu = currentMenu;
          editIndex = 1;
          editValue = degerler.xManuel;  // Doğru değişken
        }
        else if (strcmp(selectedItem, "Z Ekseni Hareketi") == 0) {
          // Z ekseni stok değerini düzenleme moduna geç
          editMode = true;
          editMenu = currentMenu;
          editIndex = 2;
          editValue = degerler.zManuel;  // Doğru değişken
        }
      }
      
      // Düzenleme modu için sayısal öğe seçildi
      else if ((currentMenu == &start || currentMenu == &home || currentMenu == &manuelAyarlari || currentMenu == &manuelKontrol) && menuIndex > 0) {
        // Fabrika ayarları öğesi düzenlenemez
        if (strcmp(selectedItem, "Fabrika Ayarlarina Don") != 0) {
          editMode = true;
          editMenu = currentMenu;
          editIndex = menuIndex;
          
          // Başlangıç değerlerini EEPROM'dan al
          if (currentMenu == &start) {
            switch(menuIndex) {
              case 1: editValue = degerler.xHiz; break;
              case 2: editValue = degerler.xIvme; break;
              case 3: editValue = degerler.xAdim; break;
              case 4: editValue = degerler.zHiz; break;
              case 5: editValue = degerler.zIvme; break;
              case 6: editValue = degerler.zAdim; break;
              case 7: editValue = degerler.zInme; break;
              case 8: editValue = degerler.zYavaslamaMesafe; break;
              case 9: editValue = degerler.zYavaslamaHiz; break;
              case 10: editValue = degerler.zBekleme; break;
              case 11: editValue = degerler.xCekme; break;
              case 12: editValue = degerler.xCekmeSure; break;
              case 13: editValue = degerler.zPark; break;
            }
          }
          else if (currentMenu == &home) {
            switch(menuIndex) {
              case 1: editValue = degerler.xAdim; break;
              case 2: editValue = degerler.xIvme; break;
              case 3: editValue = degerler.xHiz; break;
              case 4: editValue = degerler.zAdim; break;
              case 5: editValue = degerler.zIvme; break;
              case 6: editValue = degerler.zHiz; break;
              case 7: editValue = degerler.xPark; break;
              case 8: editValue = degerler.zPark; break;
            }
          }
          else if (currentMenu == &manuelAyarlari) {
            switch(menuIndex) {
              case 1: editValue = degerler.xAdim; break;
              case 2: editValue = degerler.xIvme; break;
              case 3: editValue = degerler.xHiz; break;
              case 4: editValue = degerler.zAdim; break;
              case 5: editValue = degerler.zIvme; break;
              case 6: editValue = degerler.zHiz; break;
              case 7: editValue = degerler.encoderIlerleme; break;
            }
          }
          // MANUEL KONTROL DÜZELTMESİ - HATA BURADA
          else if (currentMenu == &manuelKontrol) {
            switch(menuIndex) {
              case 1: editValue = degerler.xManuel; break;  // Doğru değişken
              case 2: editValue = degerler.zManuel; break;  // Doğru değişken
            }
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
