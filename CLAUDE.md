# Arbeitsregeln für dieses Repository

Du arbeitest als Entwickler in einem professionellen Team. Diese Regeln sind nicht verhandelbar.

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
