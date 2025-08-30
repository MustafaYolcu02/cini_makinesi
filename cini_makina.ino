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

const char* anaMenuItems[] = {"Geri", "Ayarlar", "Manuel Kontrol"};
const char* ayarlarItems[] = {"Geri", "Start Ayarlari", "Reset Ayarlari"};
const char* startItems[]   = {"Geri", "X Ekseni Ayarlari", "Z Ekseni Ayarlari", "Komut Ayarlari"};
const char* xEkseniItems[] = {"Geri", "X Ekseni Hiz", "X Ekseni Ivme", "X Ekseni Adim"};
const char* zEkseniItems[] = {"Geri", "Z Ekseni Hiz", "Z Ekseni Ivme", "Z Ekseni Adim"};
const char* komutItems[]   = {"Geri", "Z Inme Mesafesi", "Z Bekleme Mesafesi", "X Cekme Mesafesi", "X Bekleme Zamani", "Z Cikma Mesafesi"};

Menu mainMenu   = {"Ana Menu", anaMenuItems, 3, NULL}; // 3 öğe oldu
Menu ayarlar    = {"Ayarlar", ayarlarItems, 3, &mainMenu}; // Ana menü parent olacak
Menu start      = {"Start Ayarlari", startItems, 4, &ayarlar};
Menu xEkseni    = {"X Ekseni Ayarlari", xEkseniItems, 4, &start};
Menu zEkseni    = {"Z Ekseni Ayarlari", zEkseniItems, 4, &start};
Menu komut      = {"Komut Ayarlari", komutItems, 6, &start};

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
