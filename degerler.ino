#include "degerler.h"

Degerler degerler;  // Yeni yapı ismi ve değişken adı

void varsayilanDegerleriYukle() {
  degerler.xHiz = 500;
  degerler.xIvme = 500;
  degerler.xAdim = 500;
  degerler.zHiz = 500;
  degerler.zIvme = 500;
  degerler.zAdim = 500;
  degerler.zInme = 200;
  degerler.zBekleme = 2000;
  degerler.xCekme = 200;
  degerler.xBekleme = 1000;
  degerler.zCikma = 200;
}

void degerleriEEPROMdanOku() {
  // EEPROM'dan oku ve geçerli değerleri kontrol et
  EEPROM.get(X_HIZ_ADDR, degerler.xHiz);
  EEPROM.get(X_IVME_ADDR, degerler.xIvme);
  EEPROM.get(X_ADIM_ADDR, degerler.xAdim);
  EEPROM.get(Z_HIZ_ADDR, degerler.zHiz);
  EEPROM.get(Z_IVME_ADDR, degerler.zIvme);
  EEPROM.get(Z_ADIM_ADDR, degerler.zAdim);
  EEPROM.get(Z_INME_ADDR, degerler.zInme);
  EEPROM.get(Z_BEKLEME_ADDR, degerler.zBekleme);
  EEPROM.get(X_CEKME_ADDR, degerler.xCekme);
  EEPROM.get(X_BEKLEME_ADDR, degerler.xBekleme);
  EEPROM.get(Z_CIKMA_ADDR, degerler.zCikma);

  // Eğer okunan değerler geçersizse varsayılanları yükle
  if (degerler.xHiz <= 0 || degerler.xHiz > 1000 ||
      degerler.xIvme <= 0 || degerler.xIvme > 1000 ||
      degerler.xAdim <= 0 || degerler.xAdim > 1000 ||
      degerler.zHiz <= 0 || degerler.zHiz > 1000 ||
      degerler.zIvme <= 0 || degerler.zIvme > 1000 ||
      degerler.zAdim <= 0 || degerler.zAdim > 1000 ||
      degerler.zInme <= 0 || degerler.zInme > 1000 ||
      degerler.zBekleme <= 0 || degerler.zBekleme > 1000 ||
      degerler.xCekme <= 0 || degerler.xCekme > 1000 ||
      degerler.xBekleme <= 0 || degerler.xBekleme > 1000 ||
      degerler.zCikma <= 0 || degerler.zCikma > 1000) {
    varsayilanDegerleriYukle();
    // Varsayılan değerleri EEPROM'a yaz
    degeriEEPROMaYaz(X_HIZ_ADDR, degerler.xHiz);
    degeriEEPROMaYaz(X_IVME_ADDR, degerler.xIvme);
    degeriEEPROMaYaz(X_ADIM_ADDR, degerler.xAdim);
    degeriEEPROMaYaz(Z_HIZ_ADDR, degerler.zHiz);
    degeriEEPROMaYaz(Z_IVME_ADDR, degerler.zIvme);
    degeriEEPROMaYaz(Z_ADIM_ADDR, degerler.zAdim);
    degeriEEPROMaYaz(Z_INME_ADDR, degerler.zInme);
    degeriEEPROMaYaz(Z_BEKLEME_ADDR, degerler.zBekleme);
    degeriEEPROMaYaz(X_CEKME_ADDR, degerler.xCekme);
    degeriEEPROMaYaz(X_BEKLEME_ADDR, degerler.xBekleme);
    degeriEEPROMaYaz(Z_CIKMA_ADDR, degerler.zCikma);
  }
}

void degeriEEPROMaYaz(int adres, int yeniDeger) {
  int eskiDeger;
  EEPROM.get(adres, eskiDeger);
  
  if (eskiDeger != yeniDeger) {
    EEPROM.put(adres, yeniDeger);
  }
}
