// degerler.h - Manuel kontrol için EEPROM adresleri eklendi
// degerler.h - Updated with new parameters
#ifndef DEGERLER_H
#define DEGERLER_H

#include <EEPROM.h>

// EEPROM adresleri
#define EEPROM_SIZE 512

// X Ekseni Ayarları
#define X_HIZ_ADDR 0
#define X_IVME_ADDR 4
#define X_ADIM_ADDR 8

// Z Ekseni Ayarları
#define Z_HIZ_ADDR 12
#define Z_IVME_ADDR 16
#define Z_ADIM_ADDR 20

// Komut Ayarları
#define Z_INME_ADDR 24
#define Z_YAVASLAMA_MESAFE_ADDR 28
#define Z_YAVASLAMA_HIZ_ADDR 32
#define Z_BEKLEME_ADDR 36
#define X_CEKME_ADDR 40
#define X_CEKME_SURE_ADDR 44
#define Z_PARK_ADDR 48
#define X_PARK_ADDR 52
#define ENCODER_ILERLEME_ADDR 56

// Manuel Kontrol Stok Değerleri
#define X_MANUEL_ADDR 60
#define Z_MANUEL_ADDR 64

// Yeni yapı ismi: Degerler
struct Degerler {
  int xHiz;
  int xIvme;
  int xAdim;
  int zHiz;
  int zIvme;
  int zAdim;
  int zInme;
  int zYavaslamaMesafe;
  int zYavaslamaHiz;
  int zBekleme;
  int xCekme;
  int xCekmeSure;
  int zPark;
  int xPark;
  int encoderIlerleme;
  int xManuel;  // X ekseni stok değeri
  int zManuel;  // Z ekseni stok değeri
};

extern Degerler degerler;  // Değişken adı da güncellendi

void varsayilanDegerleriYukle();  // Fonksiyon ismi güncellendi
void degerleriEEPROMdanOku();     // Fonksiyon ismi güncellendi
void degeriEEPROMaYaz(int adres, int deger);  // Fonksiyon ismi güncellendi

#endif
