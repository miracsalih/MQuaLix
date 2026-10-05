# MQuaLix Derleyici

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
./compiler kaynak.micrap cikti.micrab
```

## 2. UYARI:

Bu derleyici, 2.0.0-alpha sürümündedir. O yüzden bu dallanma (branch) içinde hata olabilir ve her şey değişebilir.
Ama olabildiğince kısa sürede hataları düzeltip 2.0.0 sürümünü yayınlayacağız!
