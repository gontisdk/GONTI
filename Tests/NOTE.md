# Informacja o folderze `Tests/`

Folder `Tests/` zawiera pliki **niezwiązane z główną kompilacją silnika GONTI** (`GONTI.dll`).  

Wszystko, co umieszczone jest w tym folderze, **nie zostanie dołączone do biblioteki dynamicznej GONTI**, ani nie będzie obecne w finalnym produkcie silnika.

Pliki te są przeznaczone wyłącznie do:
- testowania jednostkowego,
- eksperymentowania z funkcjonalnościami silnika,
- uruchamiania **środowiska testowego** (`testbed.exe`), które służy do lokalnych testów i weryfikacji działania poszczególnych komponentów.

Użytkownicy rozwijający lub testujący silnik mogą tu dodawać własne przypadki testowe, jednak muszą pamiętać, że:
- **żaden z tych plików nie zostanie skompilowany do `GONTI.dll`**,  
- zawartość tego folderu **nie wpływa na główną logikę runtime silnika**,  
- folder ten służy wyłącznie jako zaplecze dewelopersko-testowe.

Jeśli chcesz, aby twój kod był częścią silnika — musi trafić poza `Tests/`, do odpowiednich modułów w strukturze `Source/`.

---
Plik utworzony w celu uniknięcia nieporozumień przy pracy z repozytorium silnika GONTI.