# MQuaLix Derleyici (compiler.cpp) Kullanım Kılavuzu

Bu belge, MQuaLix dil derleyicisinin (`compiler.cpp`) nasıl kullanılacağını, hangi argümanlarla çalıştırılacağını ve olası hata durumlarını açıklar.

## 1. Çalıştırma

Derleyici, komut satırından aşağıdaki formatta çağrılır:
```bash
./compiler <girdi_dosyası> <çıktı_dosyası>
```

### Argümanlar:

- `<girdi_dosyası>`: İşlenecek MQuaLix kaynak kod dosyasının yolu.
- `<çıktı_dosyası>`: Token analiz sonuçlarının ve işlem çıktılarının yazılacağı dosyanın yolu.

### Örnek Kullanım:
```bash
./compiler kaynak.mqlx cikti.txt
```

## 2. Desteklenen Dil Yapıları

Derleyici, aşağıda açıklanan sınırlı bir dil kümesini desteklemektedir.

### 2.1. Değişken Tanımlama (`var`)

Yeni bir değişken tanımlamak için `var` anahtar kelimesi kullanılır. Tanımlama sırasında değişkene herhangi bir ilk değer atanmaz; dahili olarak `"n"` (null) değeri atanır.
```mlx
var x ;
```

- Değişken ismi sayı ile başlayamaz.
- Daha önce tanımlanmış bir değişken tekrar tanımlanamaz (hata kodu `-8`).

### 2.2. Değişken Silme (`delete` / `del`)

Tanımlanmış bir değişkeni silmek için `delete` (veya `del`) anahtar kelimesi kullanılır.
```mlx
delete x ;
del y ;
```

- Sadece daha önce tanımlanmış değişkenler silinebilir (hata kodu `-14`).

### 2.3. Değişken Atama ve İşlemler

Değişkenlere değer atamak ve aritmetik işlemler yapmak için aşağıdaki operatörler desteklenir:

| Operatör | Açıklama | Örnek |
|----------|----------|-------|
| `=` | Atama | `x = 10 ;` |
| `+` | Toplama ve atama | `x += 5 ;` |
| `-` | Çıkarma ve atama | `x -= 3 ;` |
| `/` | Bölme ve atama | `x /= 2 ;` |
| `*` | Çarpma ve atama | `x *= 4 ;` |

İşlemler, sağ taraftaki ifade (sayı veya değişken) ile soldaki değişkenin mevcut değeri üzerinde gerçekleştirilir ve sonuç tekrar aynı değişkene atanır.

### 2.4. Koşullu İfadeler (`if` / `else` / `endif`)

Derleyici, iç içe geçebilen koşullu ifadeleri destekler:

```mlx
if x == 10 ;
    print "x 10'dur" ;
else ;
    print "x 10 değildir" ;
endif ;
```

- `if`'den sonra gelen ifade `;` ile bitmelidir.
- `else` ve `endif` kendi başlarına birer ifadedir ve `;` ile bitirilir.
- İç içe `if`/`else` blokları desteklenir (stack mantığı).
- Koşul olarak sadece `==` operatörü kullanılabilir.

### 2.5. Çıktı (`print`)

`print` anahtar kelimesi ile değişken veya sabit değerler ekrana yazdırılabilir:

```mlx
print x ;
print "Merhaba" ;
print x + 5 ;   # Desteklenmez! Sadece değişken veya sabit yazdırılır.
```

### 2.6. Sabit Değerler

- **Kesin Değerler (DEFINITE)**: `#` işareti ile başlayan tanımlayıcılar.
```mlx
#MY_CONST
```

- **Sayısal Sabitler**:
  - **Onaltılık**: `0xFF`, `0x1A2B` (prefix `0x`)
  - **Onluk**: `123`, `456` (rakamla başlayan normal sayılar)
  - **Sekizlik**: `077`, `0123` (başında `0` olan sayılar)
  - **İkilik**: `0b1010`, `0b110011` (prefix `0b`)

Tüm sayılar pozitif olarak kabul edilir; negatif sayılar (`-5`) desteklenmez.

### 2.7. İfade Sonlandırıcı

Her ifade (cümle) noktalı virgül (`;`) ile sonlandırılmalıdır. Noktalı virgül, token akışında bir ifadenin bittiğini belirtir ve derleyicinin `tokenize()` fonksiyonunu çalıştırmasını tetikler.

**Not**: Noktalı virgül ile önceki token arasında boşluk olması zorunludur (örnek: `x = 5 ;` şeklinde).

## 3. Çıktı Formatı

Başarılı bir çalışma durumunda, belirtilen çıktı dosyasına her token için aşağıdaki formatta satır yazılır:
```
İfade: <ifade_no> <token_türü> | <token_değeri>
```

İfade sonlandırıcı (`;`) görüldüğünde, `tokenize()` fonksiyonunun döndürdüğü değer (varsa) ayrıca bir satır olarak eklenir:
```
MQuaLix DBG: (dönmedi)          // tokenize() boş döndüyse
MQuaLix DBG: x = 42             // tokenize() bir değişkenin değerini gösteriyorsa
```

### Örnek Çıktı (girdi: `var x ; x = 5 ; print x ;` için):
```
İfade: 1 KEYWORD | var
İfade: 1 UNKNOW | x
MQuaLix DBG: (dönmedi)

İfade: 2 VARNAME | x
İfade: 2 OPERATOR | =
İfade: 2 DECNUMBER | 5
MQuaLix DBG: (dönmedi)

İfade: 3 KEYWORD | print
İfade: 3 VARNAME | x
MQuaLix DBG: (dönmedi)
```
`print` komutu ekrana `5` yazdırır.

## 4. Hata Kodları ve Anlamları

Bir hata oluştuğunda, program standart hata çıktısına (`stderr`) hata mesajı yazar ve aşağıdaki hata kodlarından biriyle sonlanır.

| Kod | Spesifik Hata | Açıklama |
|-----|---------------|----------|
| -1 | DERLEME::YETERSİZ_ARGÜMAN | Programa eksik argüman verildi. |
| -2 | DERLEME::GİRDİ_DOSYA_AÇILAMADI | Belirtilen girdi dosyası açılamadı. |
| -3 | DERLEME::ÇIKTI_DOSYA_AÇILAMADI | Belirtilen çıktı dosyası açılamadı (yazma izni vs.). |
| -4 | SAYI::ONALTILIK_GEÇERSİZ | Onaltılık sayıda geçersiz karakter (`A-F`, `0-9` dışı) bulundu. |
| -5 | SAYI::ONLUK_GEÇERSİZ | Onluk sayıda geçersiz karakter (`0-9` dışı) bulundu. |
| -6 | SAYI::SEKİZLİK_GEÇERSİZ | Sekizlik sayıda geçersiz karakter (`0-7` dışı) bulundu. |
| -7 | SAYI::İKİLİK_GEÇERSİZ | İkilik sayıda geçersiz karakter (`0,1` dışı) bulundu. |
| -8 | DEĞİŞKEN::ZATEN_TANIMLANMIŞ | `var` ile daha önce tanımlanmış bir değişken yeniden tanımlanmaya çalışıldı. |
| -9 | DEĞİŞKEN::ONALTILIK_OLAMAZ | Değişken ismi olarak onaltılık sayı kullanılamaz. |
| -10 | DEĞİŞKEN::ONLUK_OLAMAZ | Değişken ismi olarak onluk sayı kullanılamaz. |
| -11 | DEĞİŞKEN::SEKİZLİK_OLAMAZ | Değişken ismi olarak sekizlik sayı kullanılamaz. |
| -12 | DEĞİŞKEN::İKİLİK_OLAMAZ | Değişken ismi olarak ikilik sayı kullanılamaz. |
| -13 | ANAHTAR::KULLANIM_ANLAŞILAMADI | Geçersiz anahtar kelime kullanımı. |
| -14 | DEĞİŞKEN::TANIMLANMAMIŞ | Silinmeye çalışılan değişken daha önce tanımlanmamış. |
| -15 | TANIM::TANIMSIZ | Bir işlemde tanımsız bir değişken veya değer kullanılmaya çalışıldı. |
| -16 | SAYI::DÖNÜŞTÜRME_HATASI | Sayı dönüştürme sırasında beklenmeyen bir hata oluştu. |
| -17 | KOŞUL::ATAMA_YAPILAMAZ | Koşul ifadesinde `=` operatörü kullanılamaz. |
| -18 | KOŞUL::TOPLAMA_YAPILAMAZ | Koşul ifadesinde `+` operatörü kullanılamaz. |
| -19 | KOŞUL::ÇIKARMA_YAPILAMAZ | Koşul ifadesinde `-` operatörü kullanılamaz. |
| -20 | KOŞUL::BÖLME_YAPILAMAZ | Koşul ifadesinde `/` operatörü kullanılamaz. |
| -21 | KOŞUL::ÇARPMA_YAPILAMAZ | Koşul ifadesinde `*` operatörü kullanılamaz. |
| -22 | KOŞUL::KOŞUL_SAĞLANMAMIŞ | `endif` için eşleşen bir `if` bulunamadı. |
| -23 | KOŞUL::ELSE_HATASI | `else` için eşleşen bir `if` bulunamadı. |

## 5. Önemli Notlar

- Derleyici, sözcüksel analiz (lexical analysis) ve basit bir yorumlayıcı içerir. Kapsamlı sözdizimsel (syntax) veya anlamsal (semantic) analiz henüz uygulanmamıştır.
- Tüm sayısal sabitler pozitif olarak kabul edilir. Negatif sayılar (`-5` gibi) şu an için desteklenmemektedir.
- Kaynak kod dosyasındaki her bir token boşluk, tab veya satır sonu ile ayrılmış olmalıdır. `;` işareti de ayrı bir token olarak ele alınır.
- Değişkenler tanımlandıklarında değer olarak `"n"` (null) alırlar. Bu değer üzerinde işlem yapılmaya çalışılırsa `-15` hatası oluşur.
- İç içe `if`/`else` blokları stack mantığı ile çalışır. Her `if` yeni bir koşul ekler, `else` mevcut koşulu ters çevirir, `endif` ise en son koşulu kaldırır.
- Program, dosya okuma veya yazma sırasında oluşabilecek diğer sistem hatalarını yakalamaz.
- Derleyici, `MQuaLix DBG:` ön ekiyle hata ayıklama mesajları üretir; bu mesajlar hem çıktı dosyasına hem de standart hata akışına yazılabilir.
- `print` ifadesi sadece değişken veya sabit değer yazdırabilir. Aritmetik ifadeler `print` içinde desteklenmez.
