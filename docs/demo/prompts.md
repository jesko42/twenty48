# Prompts und Befehle für die Live-Demo

Alles hier ist zum Kopieren gedacht. Nichts live tippen, was man auch einfügen kann.

## Vorbereitung im Terminal

```sh
cd ~/assistent/vibe-coding-event/twenty48
scripts/demo-reset.sh
claude
```

## Issue anlegen (Block „Wunsch“)

```sh
gh issue create --title "[R11] Letzten Zug rückgängig machen" --body-file docs/demo/issue-undo.md
```

## Prompt 1: Spec und rote Tests (stoppt bewusst vor der Implementierung)

```
Lies CLAUDE.md und SPEC.md. Wir setzen GitHub-Issue #1 um (Undo, Text: docs/demo/issue-undo.md).
Arbeite im Branch live/undo.

Schritt 1: Ergänze SPEC.md um R11 mit Beispielen und Änderungshistorie, ergänze R10.1 um die Taste u.
Zeig mir den Diff.

Schritt 2: Schreibe die Tests für R11 in tests/test_game.cpp, baue mit dem dev-Preset und
zeig mir die roten Tests.

Implementiere noch nichts. Warte auf mein OK.
```

Erwartung: Spec-Diff, dann Compile-Fehler oder rote Tests (beides ist „rot“). Hier anhalten
und mit dem Publikum lesen: Die Tests zitieren R11.1 bis R11.4.

## Prompt 2: Grün, Gate, PR

```
OK. Implementiere R11 in der Kernbibliothek (Game) und die Taste u in main.cpp.
Dann scripts/check.sh. Dann drei Commits nach CLAUDE.md: docs(spec), test(game), feat(game).
Push und PR mit gh pr create, Vorlage ausfüllen, Closes #1.
```

Erwartung: grüne Tests, `All checks passed.`, PR-Link.

## Falls Claude Code abweicht

- Tests gelöscht oder abgeschwächt: laut vorlesen, auf CLAUDE.md Regel 2 zeigen, zurückweisen.
  Das ist ein guter Moment, kein Unfall.
- Implementiert vor den Tests: „Stopp. Erst rot.“ Zurück auf Prompt 1.
- Hängt oder Netz weg: `git checkout demo/feature-undo && git log --oneline -4`.

## Gegenprobe (Block „Gegenprobe“)

```sh
gh pr create --head demo/vibe-bug --title "refactor(row): simplify merge loop" \
  --body "Kleines Refactoring, keine Verhaltensänderung. 8 Zeilen weniger."
```

Lokal, falls CI zu langsam:

```sh
git checkout demo/vibe-bug && cmake --build --preset dev && ctest --preset dev; git checkout main
```

Der Test „R3 no chain merge“ schlägt fehl (R3.2): aus `2 2 4 0` wird `8 0 0 0` statt `4 4 0 0`. Weitere Beispiele aus der Spec-Tabelle fallen ebenfalls.
