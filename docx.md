# MQuaLix Derleyici (compiler.cpp) Kullanım Kılavuzu

Bu belge, MQuaLix dil derleyicisinin (`compiler.cpp`) nasıl kullanılacağını, hangi argümanlarla çalıştırılacağını ve olası hata durumlarını açıklar.

## 1. Çalıştırma

Derleyici, komut satırından aşağıdaki formatta çağrılır:

```bash
./compiler <girdi_dosyası> <çıktı_dosyası>
```

### Argümanlar:
*   `<girdi_dosyası>`: İşlenecek MQuaLix kaynak kod dosyasının yolu.
*   `<çıktı_dosyası>`: Token analiz sonuçlarının yazılacağı çıktı dosyasının yolu.

### Örnek Kullanım:
```bash
./compiler kaynak.micrap cikti.txt
```

## 2. Desteklenen Dil Yapıları

Derleyici şu an sınırlı bir dil kümesini desteklemektedir:

### 2.1. Değişken Tanımlama (`var`)
Yeni bir değişken tanımlamak için `var` anahtar kelimesi kullanılır.
```
var myVar ;
```
*   Değişken ismi sayı ile başlayamaz.
*   Daha önce tanımlanmış bir değişken tekrar tanımlanamaz.

### 2.2. Değişken Silme (`delete`)
Tanımlanmış bir değişkeni silmek için `delete` anahtar kelimesi kullanılır.
```
delete myVar ;
```
*   Sadece daha önce tanımlanmış değişkenler silinebilir.

### 2.3. Operatörler
Desteklenen operatörler (şu an sadece token olarak tanınır, işlevsellikleri yoktur):
*   Atama: `=`
*   Eşitlik kontrolü: `==`
*   Bileşik atamalar: `+=`, `-=`, `/=`, `*=`

### 2.4. Sabit Değerler
*   **Kesin Değerler**: `#` işareti ile başlayan tanımlayıcılar.
    ```
    #DEGER
    ```
*   **Sayısal Sabitler**:
    *   Onaltılık: `0xFF`, `0x1A2B`
    *   Onluk: `123`, `456`
    *   Sekizlik: `077`, `0123` (sıfır ile başlar)
    *   İkilik: `0b1010`, `0b110011`

### 2.5. İfade Sonlandırıcı
Her ifade (cümle) **noktalı virgül (`;`)** ile sonlandırılmalıdır. İfade sonlandırılması ifade ile arasında boşluk olmalıdır.

## 3. Çıktı Formatı

Başarılı bir çalışma durumunda, belirtilen çıktı dosyasına her token için aşağıdaki formatta satır yazılır:

```
İfade: <ifade_no> <token_türü> | <token_değeri>
```

**Örnek Çıktı (girdi: `var x;` için):**
```
İfade: 1 KEYWORD | var
İfade: 1 UNKNOW | x
```

## 4. Hata Kodları ve Anlamları

Bir hata oluştuğunda, program standart hata çıktısına (`stderr`) hata mesajı yazar ve aşağıdaki hata kodlarından biriyle sonlanır.

| Kod | Spesifik Hata | Açıklama |
| :--- | :--- | :--- |
| **-1** | `DERLEME::YETERSİZ_ARGÜMAN` | Programa eksik argüman verildi. |
| **-2** | `DERLEME::GİRDİ_DOSYA_AÇILAMADI` | Belirtilen girdi dosyası açılamadı. |
| **-3** | `DERLEME::ÇIKTI_DOSYA_AÇILAMADI` | Belirtilen çıktı dosyası açılamadı (yazma izni vs.). |
| **-4** | `SAYI::ONALTILIK_GEÇERSİZ` | Onaltılık sayıda geçersiz karakter (A-F, 0-9 dışı) bulundu. |
| **-5** | `SAYI::ONLUK_GEÇERSİZ` | Onluk sayıda geçersiz karakter (0-9 dışı) bulundu. |
| **-6** | `SAYI::SEKİZLİK_GEÇERSİZ` | Sekizlik sayıda geçersiz karakter (0-7 dışı) bulundu. |
| **-7** | `SAYI::İKİLİK_GEÇERSİZ` | İkilik sayıda geçersiz karakter (0,1 dışı) bulundu. |
| **-8** | `DEĞİŞKEN::ZATEN_TANIMLANMIŞ` | `var` ile daha önce tanımlanmış bir değişken yeniden tanımlanmaya çalışıldı. |
| **-9** | `DEĞİŞKEN::ONALTILIK_OLAMAZ` | Değişken ismi olarak onaltılık sayı kullanılamaz. |
| **-10** | `DEĞİŞKEN::ONLUK_OLAMAZ` | Değişken ismi olarak onluk sayı kullanılamaz. |
| **-11** | `DEĞİŞKEN::SEKİZLİK_OLAMAZ` | Değişken ismi olarak sekizlik sayı kullanılamaz. |
| **-12** | `DEĞİŞKEN::İKİLİK_OLAMAZ` | Değişken ismi olarak ikilik sayı kullanılamaz. |
| **-13** | `ANAHTAR::KULLANIM_ANLAŞILAMADI` | `var` veya `delete` dışında bir anahtar kelimenin kullanımı. |
| **-14** | (Belirtilmemiş) | Tanınmayan bir değişken silinmeye çalışıldı. |

## 5. Önemli Notlar

*   Derleyici şu an **yalnızca token çıkarımı (lexical analysis)** yapmaktadır. Sözdizimsel (syntax) veya anlamsal (semantic) analiz henüz uygulanmamıştır.
*   Tüm sayısal sabitler **pozitif** olarak kabul edilir. Negatif sayılar (`-5` gibi) şu an için desteklenmemektedir.
*   Kaynak kod dosyasındaki her bir token boşluk, tab veya satır sonu ile ayrılmış olmalıdır.
*   Program, dosya okuma veya yazma sırasında oluşabilecek diğer hataları yakalamaz.
