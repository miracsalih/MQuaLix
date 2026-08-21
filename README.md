# MQuaLix Derleyici

MQuaLix dilinin ön uç (front-end) derleyicisi. Bu araç, MQuaLix kaynak kod dosyalarını okuyarak sözcüksel analiz (lexical analysis) yapar ve tokenları bir çıktı dosyasına yazar.

## 🚀 Hızlı Başlangıç

### Derleme
```bash
g++ -o compiler compiler.cpp
```

### Kullanım
```bash
./compiler <girdi_dosyası> <çıktı_dosyası>
```

### Örnek
```bash
./compiler kaynak.micrap cikti.txt
```

## 📝 Desteklenen Dil Yapıları

### Değişken Tanımlama (`var`)
```cpp
var myVar ;
var sayi ;
```

### Değişken Silme (`delete`)
```cpp
delete myVar ;
```

### Operatörler
Desteklenen operatörler (token olarak tanınır):
- Atama: `=`
- Eşitlik: `==`
- Bileşik atamalar: `+=`, `-=`, `/=`, `*=`

### Sabit Değerler
- **Kesin Değerler**: `#DEGER`
- **Onaltılık**: `0xFF`, `0x1A2B`
- **Onluk**: `123`, `456`
- **Sekizlik**: `077`, `0123`
- **İkilik**: `0b1010`, `0b110011`

### İfade Sonlandırıcı
Her ifade noktalı virgül (`;`) ile sonlandırılmalıdır:
```cpp
var x ;
delete x ;
0xFF ;
#SABIT ;
```

## 📤 Çıktı Formatı

Başarılı çalışma durumunda, çıktı dosyasına her token için şu formatta satır yazılır:
```
İfade: <ifade_no> <token_türü> | <token_değeri>
```

**Örnek** (girdi: `var x;`):
```
İfade: 1 KEYWORD | var
İfade: 1 UNKNOW | x
```

## ⚠️ Hata Kodları

| Kod | Hata | Açıklama |
|:---|:---|:---|
| -1 | `DERLEME::YETERSİZ_ARGÜMAN` | Eksik argüman |
| -2 | `DERLEME::GİRDİ_DOSYA_AÇILAMADI` | Girdi dosyası açılamadı |
| -3 | `DERLEME::ÇIKTI_DOSYA_AÇILAMADI` | Çıktı dosyası açılamadı |
| -4 | `SAYI::ONALTILIK_GEÇERSİZ` | Onaltılıkta geçersiz karakter |
| -5 | `SAYI::ONLUK_GEÇERSİZ` | Onlukta geçersiz karakter |
| -6 | `SAYI::SEKİZLİK_GEÇERSİZ` | Sekizlikte geçersiz karakter |
| -7 | `SAYI::İKİLİK_GEÇERSİZ` | İkilikte geçersiz karakter |
| -8 | `DEĞİŞKEN::ZATEN_TANIMLANMIŞ` | Değişken zaten tanımlı |
| -9 | `DEĞİŞKEN::ONALTILIK_OLAMAZ` | Değişken ismi onaltılık olamaz |
| -10 | `DEĞİŞKEN::ONLUK_OLAMAZ` | Değişken ismi onluk olamaz |
| -11 | `DEĞİŞKEN::SEKİZLİK_OLAMAZ` | Değişken ismi sekizlik olamaz |
| -12 | `DEĞİŞKEN::İKİLİK_OLAMAZ` | Değişken ismi ikilik olamaz |
| -13 | `ANAHTAR::KULLANIM_ANLAŞILAMADI` | Geçersiz anahtar kelime kullanımı |
| -14 | (Belirtilmemiş) | Tanınmayan değişken silinemez |

## 📋 Örnek Kullanım Senaryoları

### Geçerli Kod
```cpp
var x ;
var y ;
x ;
#DEGER ;
0xFF ;
123 ;
077 ;
0b1010 ;
delete x ;
```

### Geçersiz Kod ve Hatalar
```cpp
var 123 ;     // HATA -9: Değişken ismi onluk olamaz
var x ;       // Geçerli
var x ;       // HATA -8: Değişken zaten tanımlanmış
delete y ;    // HATA -14: Tanınmayan değişken silinemez
0xGG ;        // HATA -4: Onaltılıkta geçersiz karakter
```

## ⚙️ Teknik Detaylar

- **Dil**: C++
- **Lisans**: LGPL-2.1
- **Durum**: Sadece sözcüksel analiz (lexical analysis) aşaması
- **Sınırlamalar**:
  - Sözdizimsel (syntax) analiz yapılmaz
  - Anlamsal (semantic) analiz yapılmaz
  - Negatif sayılar desteklenmez
  - Tokenlar boşluk/tab/satır sonu ile ayrılmalıdır

## 🔗 Bağlantılar

- [Proje Ana Sayfası](https://github.com/miracsalih/MQuaLix)
- [Kaynak Kod](https://github.com/miracsalih/MQuaLix/blob/V0.1/compiler.cpp)

## 📄 Lisans

Bu proje [LGPL-2.1](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html) lisansı altında lisanslanmıştır.

Telif Hakkı (c) 2026 Miraç Salih İşler
