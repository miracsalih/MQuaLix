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
./compiler kaynak.mqlx cikti.txt
```

## 📝 Desteklenen Dil Yapıları

### Değişken Tanımlama (`var`)
```mlx
var myVar ;
var sayi ;
```

### Değişken Silme (`delete` veya `del`)
```mlx
delete myVar ;
del sayi ;
```

### Değişken Atama ve İşlemler
```mlx
x = 10 ;
y = x + 5 ;
z = x - 3 ;
a = x / 2 ;
b = x * 4 ;
```

### Koşullu İfadeler (`if` / `else` / `endif`)
```mlx
if x == 10 ;
    print x ;
else ;
    print "x 10 değil" ;
endif ;
```

### Çıktı (`print`)
```mlx
print x ;
print "Merhaba" ;
```

### Operatörler

Desteklenen operatörler:

| Operatör | Açıklama |
|----------|----------|
| `=` | Atama |
| `==` | Eşitlik karşılaştırması |
| `+` | Toplama |
| `-` | Çıkarma |
| `/` | Bölme |
| `*` | Çarpma |

### Sabit Değerler

- **Onaltılık**: `0xFF`, `0x1A2B`
- **Onluk**: `123`, `456`
- **Sekizlik**: `077`, `0123`
- **İkilik**: `0b1010`, `0b110011`
- **Kesin Değerler**: `#DEGER`

### İfade Sonlandırıcı

Her ifade noktalı virgül (`;`) ile sonlandırılmalıdır:
```mlx
var x ;
x = 10 ;
print x ;
```

## 📤 Çıktı Formatı

Başarılı çalışma durumunda, çıktı dosyasına her token için şu formatta satır yazılır:
```
İfade: <ifade_no> <token_türü> | <token_değeri>
```

Örnek (girdi: `var x ;`):
```
İfade: 1 KEYWORD | var
İfade: 1 UNKNOW | x
```

Debug mesajları için:
```
MQuaLix DBG: (dönmedi)    # İfade işlendi ama değer döndürmedi
MQuaLix DBG: x = 10       # Değişken değer sorgulandı
```

## ⚠️ Hata Kodları

| Kod | Hata | Açıklama |
|-----|------|----------|
| -1 | DERLEME::YETERSİZ_ARGÜMAN | Eksik argüman |
| -2 | DERLEME::GİRDİ_DOSYA_AÇILAMADI | Girdi dosyası açılamadı |
| -3 | DERLEME::ÇIKTI_DOSYA_AÇILAMADI | Çıktı dosyası açılamadı |
| -4 | SAYI::ONALTILIK_GEÇERSİZ | Onaltılıkta geçersiz karakter |
| -5 | SAYI::ONLUK_GEÇERSİZ | Onlukta geçersiz karakter |
| -6 | SAYI::SEKİZLİK_GEÇERSİZ | Sekizlikte geçersiz karakter |
| -7 | SAYI::İKİLİK_GEÇERSİZ | İkilikte geçersiz karakter |
| -8 | DEĞİŞKEN::ZATEN_TANIMLANMIŞ | Değişken zaten tanımlı |
| -9 | DEĞİŞKEN::ONALTILIK_OLAMAZ | Değişken ismi onaltılık olamaz |
| -10 | DEĞİŞKEN::ONLUK_OLAMAZ | Değişken ismi onluk olamaz |
| -11 | DEĞİŞKEN::SEKİZLİK_OLAMAZ | Değişken ismi sekizlik olamaz |
| -12 | DEĞİŞKEN::İKİLİK_OLAMAZ | Değişken ismi ikilik olamaz |
| -13 | ANAHTAR::KULLANIM_ANLAŞILAMADI | Geçersiz anahtar kelime kullanımı |
| -14 | DEĞİŞKEN::TANIMLANMAMIŞ | Tanımsız değişken silinemez |
| -15 | TANIM::TANIMSIZ | Tanımsız bir değer kullanıldı |
| -16 | SAYI::DÖNÜŞTÜRME_HATASI | Sayı dönüştürme hatası |
| -17 | KOŞUL::ATAMA_YAPILAMAZ | Koşulda atama yapılamaz |
| -18 | KOŞUL::TOPLAMA_YAPILAMAZ | Koşulda toplama yapılamaz |
| -19 | KOŞUL::ÇIKARMA_YAPILAMAZ | Koşulda çıkarma yapılamaz |
| -20 | KOŞUL::BÖLME_YAPILAMAZ | Koşulda bölme yapılamaz |
| -21 | KOŞUL::ÇARPMA_YAPILAMAZ | Koşulda çarpma yapılamaz |
| -22 | KOŞUL::KOŞUL_SAĞLANMAMIŞ | endif için koşul sağlanmamış |
| -23 | KOŞUL::ELSE_HATASI | Else eşleşmesi hatası |

## 📋 Örnek Kullanım Senaryoları

### Geçerli Kod
```mlx
var x ;
var y ;
x = 10 ;
y = x + 5 ;
print y ;
x ;
#DEGER ;
0xFF ;
123 ;
077 ;
0b1010 ;
delete x ;
del y ;

if x == 10 ;
    print "x 10'dur" ;
else ;
    print "x 10 değildir" ;
endif ;
```

### Geçersiz Kod ve Hatalar
```mlx
var 123 ;          # HATA -9: Değişken ismi onluk olamaz
var x ;            # Geçerli
var x ;            # HATA -8: Değişken zaten tanımlanmış
delete y ;         # HATA -14: Tanımsız değişken silinemez
0xGG ;             # HATA -4: Onaltılıkta geçersiz karakter
if x + 10 ;        # HATA -18: Koşulda toplama yapılamaz
```

## ⚙️ Teknik Detaylar

- **Dil**: C++
- **Lisans**: LGPL-2.1
- **Durum**: Sözcüksel analiz (lexical analysis) ve basit yorumlama
- **Sınırlamalar**:
  - İç içe `if`/`else` blokları desteklenir (stack mantığı ile)
  - Negatif sayılar desteklenmez
  - Tokenlar boşluk/tab/satır sonu ile ayrılmalıdır
  - Her ifade `;` ile bitmeli
  - `else` blokları `endif` ile kapatılmalıdır

## 🔗 Bağlantılar

- [Proje Sayfası](https://github.com/miracsalih/MQuaLix)
- [LGPL-2.1 Lisans](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html)

## 📄 Lisans

Bu proje LGPL-2.1 lisansı altında lisanslanmıştır.

Telif Hakkı (c) 2026 Miraç Salih İşler

## Yapılan Değişiklikler:

1. **Hata Kodları Düzeltildi**: Tüm kodlar (1-23 arası) tutarlı hale getirildi
2. **`if` Koşul Hataları Eklendi**: `-17`'den `-21`'e kadar tüm koşul hataları
3. **`else` Hatası Eklendi**: `-23` kodu ile
4. **Örnekler Güncellendi**: Koşul hataları örnekleri eklendi
5. **Tutarlılık Sağlandı**: Kod ve README artık birebir eşleşiyor
