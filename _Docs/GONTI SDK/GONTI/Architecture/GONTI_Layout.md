# ENGINE LAYOUT

Ten dokument opisuje aktualną **strukturę logiczną (folderów)** projektu `PhysicsEngine` – silnika fizyki 2D przeznaczonego do symulacji, testów oraz zastosowań edukacyjno-inżynierskich.

Pełni rolę mapy projektu oraz przewodnika przy jego dalszym rozwijaniu i utrzymaniu.  
Dokument powinien być aktualizowany po każdej istotnej zmianie w strukturze folderów lub dodaniu nowych modułów.

---

## 📚 Spis treści

- [Struktura folderów](#struktura-folderów)
  - [Collisions](#collisions)
  - [Constraints](#constraints)
  - [Dynamics](#dynamics)
  - [Integrators](#integrators)
  - [Materials](#materials)
  - [Math](#math)
  - [Objects](#objects)
  - [PhysicsObjects](#physicsobjects)
  - [Shapes](#shapes)
  - [Solvers](#solvers)
  - [SpatialPartitioning](#spatialpartitioning)
  - [Utilities](#utilities)
  - [Events](#events)
  - [Config](#config)
  - [Tests](#tests)
  - [Docs](#docs)

---

## Struktura folderów i plików

```
PhysicsEngine/
├── Collisions/ # Wykrywanie i rozwiązywanie kolizji
│ ├── Detection/ # Algorytmy detekcji (SAT, GJK, itd.)
│ ├── Resolution/ # Separacja i impulsy reakcji
│ ├── BroadPhase/ # Preselekcja (AABB, siatki)
│ └── NarrowPhase/ # Analiza punktów kontaktu
│
│
│
├── Constraints/ # Więzy fizyczne
│ ├── Distance/ # Odległości stałe
│ ├── Spring/ # Sprężyny z tłumieniem
│ └── Anchors/ # Zaczepy do przestrzeni
│
│
│
├── Dynamics/ # Ruch, siły i integracja
│ ├── Forces/ # Siły: grawitacja, opory, użytkownik
│ ├── Motion/ # Prędkości, rotacje, aktualizacja pozycji
│ └── MassProperties/ # Masa, środek masy, moment bezwładności
│
│
│
├── Integrators/ # Algorytmy całkowania numerycznego
│ ├── Euler/ # Prosty integrator (mała stabilność)
│ ├── Verlet/ # Stabilny, często stosowany w 2D
│ └── RungeKutta/ # Zaawansowany (np. RK4)
│
│
│
├── Materials/ # Właściwości materiałów
│ ├── Presets/ # Gotowe zestawy (np. drewno, guma)
│ └── Custom/ # Materiały definiowane przez użytkownika
│
│
│
├── Math/
<<<<<<< HEAD
│ ├── gontiVectors/
│ │ ├── gontiVectors.c
│ │ ├── gontiVectors.h
│ │
│ ├── gontiMatrices/
│ │ ├── gontiMatrices.c
│ │ ├── gontiMatrices.h
=======
│ ├── Vectors/
│ │ ├── Vectors.c
│ │ ├── Vectors.h
│ │
│ ├── Matrices/
│ │ ├── Matrices.c
│ │ ├── Matrices.h
>>>>>>> 475c679c815ea5f049414def8adf10b6b1ed375a
│ │
│ ├── Geometry/
│ │
│ ├── AES/
│ │ ├── Decryption/
│ │ ├── Encryption/
│ │
│ ├── Algebra/
│ │ ├── BigInteger/
│ │
│ ├── GraphTheory/
│ │ ├── MaxFlow/
│ │
│ 
│ 
│ 
├── Objects/ # Obiekty sceny symulacyjnej
│ ├── Defaults/ # Przykładowe obiekty
│ ├── Customs/ # Definicje użytkownika
│ 
│ 
│
├── PhysicsObjects/ # Obiekty fizyczne
│ ├── RigidBodies/ # Ciała sztywne
│ ├── Particles/ # Punkty masy (bez rotacji)
│ └── Base/ # Klasy bazowe i interfejsy wspólne
│
│
│
├── Shapes/ # Kształty kolizyjne
│ ├── Circles/ # Okręgi
│ ├── Boxes/ # Prostokąty / kwadraty
│ ├── Polygons/ # Wypukłe wielokąty
│ └── Base/ # Bazowy interfejs Shape
│
│
│
├── Solvers/ # Rozwiązywanie kolizji i więzów
│ ├── Impulse/ # Solver impulsów
│ └── Constraint/ # Solver więzów (np. PGS, Gauss-Seidel)
│
│
│
├── SpatialPartitioning/ # Struktury przyspieszające
│ ├── UniformGrid/ # Jednolita siatka
│ ├── QuadTree/ # Hierarchiczna dekompozycja
│ └── HashGrid/ # Haszowanie przestrzeni
│
│
│
├── Utilities/ # Narzędzia pomocnicze
│ ├── Debug/ # Debugowanie i wizualizacja
│ ├── Logging/ # Logowanie
│ ├── Profiling/ # Pomiar wydajności, licznik FPS
│ └── TypeHelpers/ # Szablony, ID, konwersje typów
│
│
│
├── Events/ # Zdarzenia fizyczne
│ ├── Collision/ # Kolizje (enter, stay, exit)
│ └── Dispatchers/ # Subskrypcje i kolejki zdarzeń
│
│
│
├── Config/ # Ustawienia symulacji
│ ├── Defaults/ # Domyślne: grawitacja, dt, marginesy
│ └── Overrides/ # Konfiguracje użytkownika
│
│
│
├── Tests/
│ ├── Window/
│ │ └── OpenGL/
│ │ └── Vulkan/
│ │
│ └── BenchWindow.c
│ └── BenchWindow.h



└── Docs/
│
├── Architecture/
│ ├── ENGINE_LAYOUT.md
│
└── API/

```
---

## Opis folderów i podfolderów

### TODO

---