# Arbeitsregeln für dieses Repository

Du arbeitest als Entwickler in einem professionellen Team. Diese Regeln sind nicht verhandelbar.

## 0. Soll-Zustand des Projekts

Diese Struktur gilt. Fehlt etwas davon (etwa in einem frischen Repo), wird es genau so angelegt.

```
CMakeLists.txt            C++17, CMake >= 3.25, Optionen TWENTY48_WARNINGS_AS_ERRORS (an),
                          TWENTY48_SANITIZERS, TWENTY48_COVERAGE, TWENTY48_BUILD_TESTS
CMakePresets.json         Presets dev (Debug + Sanitizer), release, ci, coverage; Ninja; build/<preset>
cmake/ProjectOptions.cmake twenty48_apply_options(target): strenge Warnungen, -Werror, Sanitizer, Coverage
include/twenty48/*.hpp    Kernbibliothek twenty48::core: row (slideLeft), board, rng (Interface Rng,
src/*.cpp                 StdRng), game. Keine Ein-/Ausgabe.
src/main.cpp              Terminal-Oberfläche: Argumente --seed, --moves; Tasten nach R10
tests/                    doctest: test_main.cpp, je Modul eine test_*.cpp, helpers.hpp mit
                          makeBoard, toGrid und ScriptedRng. Smoke-Test der CLI über CTest.
third_party/doctest/      doctest.h liegt bei. Nichts herunterladen, keine FetchContent.
scripts/check.sh          Gate: check-format.sh, Build dev, ctest, check-tidy.sh
scripts/check-coverage.sh gcovr, Kernbibliothek ohne main.cpp, Schwelle 90 % Zeilen
.github/workflows/ci.yml  Jobs: build-test (gcc), build-test (clang), format, tidy, coverage
```

## 1. Die Spec ist die Wahrheit

- `SPEC.md` beschreibt das Verhalten vollständig. Keine Funktionalität ohne Anforderung in der Spec.
- Neue Anforderung oder Änderungswunsch: **zuerst** `SPEC.md` ändern (neue Nummer vergeben, Beispiele ergänzen, Änderungshistorie pflegen). Erst danach Tests, erst danach Code.
- Ist eine Anforderung unklar oder widersprüchlich, frage nach, statt zu raten.

## 2. Tests vor Code

- Für jede Anforderung gibt es Tests in `tests/`, die die Spec-Nummer im Namen tragen (`R3`, `R4.2`, ...).
- Reihenfolge: Test schreiben, laufen lassen, **rot sehen**, implementieren, **grün sehen**. Zeige beide Zustände.
- Tests werden niemals gelöscht, abgeschwächt oder mit Ausnahmen versehen, damit etwas grün wird. Wenn ein Test falsch wirkt, prüfe die Spec und sprich es an.

## 3. Qualitätsgate

Vor jedem Commit muss `scripts/check.sh` fehlerfrei durchlaufen. Es prüft Format (clang-format), Build mit `-Werror`, Tests mit Sanitizern und clang-tidy.

```sh
cmake --preset dev && cmake --build --preset dev && ctest --preset dev   # schnell
scripts/check.sh                                                          # vollständig
```

## 4. Git und GitHub

- Niemals direkt auf `main`. Für jede Aufgabe ein Branch: `feat/<thema>`, `fix/<thema>`, `docs/<thema>`.
- Conventional Commits auf Englisch: `feat(game): add undo`, `test(game): cover undo (R11)`, `docs(spec): add R11 undo`.
- Kleine Commits in der Reihenfolge des Prozesses: `docs(spec)` → `test(...)` → `feat(...)`. Der Git-Log soll den Weg zeigen.
- Pull Request mit der Vorlage aus `.github/PULL_REQUEST_TEMPLATE.md`, Issue verlinken (`Closes #N`).

## 5. Architektur

- `include/twenty48/` und `src/*.cpp` außer `main.cpp`: Kernbibliothek, keine Ein-/Ausgabe, kein eigener Zufall. Zufall kommt über das Interface `Rng` herein.
- `src/main.cpp`: nur Terminal-Oberfläche und Argumente. Keine Spielregeln.
- Fehler werden durch Exceptions aus der Standardbibliothek gemeldet, nicht durch Rückgabecodes.

## 6. Sprache und Stil

- Spec, Doku und Issues auf Deutsch. Code, Kommentare, Tests und Commits auf Englisch.
- Stil kommt aus `.clang-format` und `.clang-tidy`, nicht aus Geschmack. Keine Formatdiskussionen.
