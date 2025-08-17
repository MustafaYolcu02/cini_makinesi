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
#define Z_BEKLEME_ADDR 28
#define X_CEKME_ADDR 32
#define X_BEKLEME_ADDR 36
#define Z_CIKMA_ADDR 40

// Yeni yapı ismi: Degerler
struct Degerler {
  int xHiz;
  int xIvme;
  int xAdim;
  int zHiz;
  int zIvme;
  int zAdim;
  int zInme;
  int zBekleme;
  int xCekme;
  int xBekleme;
  int zCikma;
};

extern Degerler degerler;  // Değişken adı da güncellendi

void varsayilanDegerleriYukle();  // Fonksiyon ismi güncellendi
void degerleriEEPROMdanOku();     // Fonksiyon ismi güncellendi
void degeriEEPROMaYaz(int adres, int deger);  // Fonksiyon ismi güncellendi

#endif