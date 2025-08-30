// cini_makina.ino - Manuel Kontrol için stok değişkenleri eklendi
#include <LiquidCrystal.h>
#include <Encoder.h>
#include <EEPROM.h>
#include "degerler.h"

LiquidCrystal lcd(16, 17, 23, 25, 27, 29);
Encoder myEnc(31, 33);
#define BTN 35

struct Menu {
  const char* title;   // Menü ismi
  const char** items;  // Menüdeki seçenekler
  int itemCount;       // Kaç seçenek var
  Menu* parent;        // Bir üst menü
};

// Ana Menu Items
const char* anaMenuItems[] = {"Geri", "Ayarlar", "Manuel Kontrol"};

// Ayarlar Menu Items
const char* ayarlarItems[] = {"Geri", "Start Ayarlari", "Home Ayarlari", "Manuel Ayarlari"};

// Start Ayarlari Items
const char* startItems[]   = {"Geri", "X Ekseni Hiz", "X Ekseni Ivme", "X Ekseni Adim", 
                              "Z Ekseni Hiz", "Z Ekseni Ivme", "Z Ekseni Adim",
                              "Z Inme Mesafesi", "Z Yavaslama Mesafesi", "Z Yavaslama Hizi",
                              "Z Bekleme Suresi", "X Cekme Mesafesi", "X Cekme Suresi",
                              "Z Park Pozisyonu", "Fabrika Ayarlarina Don"};

// Home Ayarlari Items
const char* homeItems[]   = {"Geri", "X Ekseni Adim", "X Ekseni Ivme", "X Ekseni Hiz",
                             "Z Ekseni Adim", "Z Ekseni Ivme", "Z Ekseni Hiz",
                             "X Ekseni Park Pozisyonu", "Z Ekseni Park Pozisyonu",
                             "Fabrika Ayarlarina Don"};

// Manuel Ayarlari Items
const char* manuelItems[]   = {"Geri", "X Ekseni Adim", "X Ekseni Ivme", "X Ekseni Hiz",
                               "Z Ekseni Adim", "Z Ekseni Ivme", "Z Ekseni Hiz",
                               "Encoder Ilerleme Mesafesi", "Fabrika Ayarlarina Don"};

// Manuel Kontrol Items
const char* manuelKontrolItems[] = {"Geri", "X Ekseni Hareketi", "Z Ekseni Hareketi"};

// Fabrika Ayarlari Onay Items
const char* fabrikaOnayItems[] = {"Geri", "Evet", "Hayir"};

// Menu Definitions
Menu mainMenu         = {"Ana Menu", anaMenuItems, 3, NULL};
Menu ayarlar          = {"Ayarlar", ayarlarItems, 4, &mainMenu};
Menu start            = {"Start Ayarlari", startItems, 15, &ayarlar};
Menu home             = {"Home Ayarlari", homeItems, 10, &ayarlar};
Menu manuelAyarlari   = {"Manuel Ayarlari", manuelItems, 9, &ayarlar};
Menu manuelKontrol    = {"Manuel Kontrol", manuelKontrolItems, 3, &mainMenu};
Menu fabrikaOnay      = {"Fabrika Ayarlari", fabrikaOnayItems, 3, NULL}; // Parent will be set dynamically

Menu* currentMenu = &mainMenu;
int menuIndex = 0;
int topItemIndex = 0;

long oldPosition = 0;

bool editMode = false;   // Sayı düzenleme modunda mıyız?
int editValue = 0;       // Düzenlenen değer
Menu* editMenu = NULL;   // Düzenlenen menü
int editIndex = 0;       // Düzenlenen öğe indexi

unsigned long buttonPressCount = 0; // Buton basım sayacı
unsigned long lastUpdateTime = 0;
const unsigned long UPDATE_INTERVAL = 1000; // 1 saniye

bool infoScreenMode = true; // Bilgi ekranı modunda mıyız?
Menu* previousMenuBeforeFabrika = NULL; // Fabrika onayından önceki menüyü saklamak için

void setup() {
  lcd.begin(20, 4);
  pinMode(BTN, INPUT_PULLUP);

  degerleriEEPROMdanOku();
  // Ana menüyü bilgi ekranı olarak başlat (parent'ı NULL yap)
  infoScreenMode = true;
  currentMenu = &mainMenu;

  kayan_logo();
  lcd.clear();
  
  displayMenu();
}

void loop() {
  if (encoderOkuma()) {  // değişiklik olduysa
    displayMenu();
    lastUpdateTime = millis(); // Güncelleme zamanını sıfırla
  }

  butonOkuma();

  // Bilgi ekranı modundaysak ve belirli aralıklarla ekranı güncelle
  if (infoScreenMode) {
    unsigned long currentTime = millis();
    if (currentTime - lastUpdateTime >= UPDATE_INTERVAL) {
      displayMenu();
      lastUpdateTime = currentTime;
    }
  }
}
