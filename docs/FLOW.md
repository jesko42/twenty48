# Der komplette Flow aus dem leeren Repo

Variante zum Regieplan: Statt das fertige Repo zu zeigen, bringst du nur die „Verfassung“ mit
und lässt Claude Code alles andere vor Publikum erzeugen. Was du mitbringst, ist das Seed:

```
CLAUDE.md                 Arbeitsregeln und Soll-Struktur (Abschnitt 0 ist der Bauplan)
SPEC.md                   Anforderungen R1 bis R10
.clang-format, .clang-tidy, tests/.clang-tidy
.gitignore, .editorconfig, LICENSE
third_party/doctest/      Test-Framework, damit nichts aus dem Netz geladen werden muss
```

`scripts/make-seed.sh` erzeugt dieses Verzeichnis unter `../twenty48-seed` aus dem fertigen Repo.
So bleiben Seed und Repo garantiert synchron.

## Werkzeuge auf dem Vortragsrechner

Ubuntu 24.04:

```sh
sudo apt-get install -y build-essential cmake ninja-build git clang-format-18 clang-tidy-18 gcovr gh
```

Prüfen:

```sh
cmake --version        # >= 3.25
ninja --version
g++ --version          # >= 12
clang-format-18 --version
clang-tidy-18 --version
gcovr --version        # nur für scripts/check-coverage.sh, CI braucht es nicht lokal
gh auth status         # eingeloggt
claude --version       # Claude Code, eingeloggt (einmal `claude` starten)
```

Ohne Netz geht alles außer Claude Code selbst und GitHub. Deshalb bleibt das fertige Repo
`twenty48` mit seinen Branches der Fallback (siehe `docs/REGIEPLAN.md`).

## Ablauf in einer frischen Session

```sh
cp -r twenty48-seed twenty48-live && cd twenty48-live
git init -b main && git add -A && git commit -m "chore: project constitution (CLAUDE.md, SPEC.md, style)"
claude
```

### Prompt A: Gerüst und erstes Modul, mit rot und grün (ca. 8 Minuten)

```
Lies CLAUDE.md und SPEC.md. Das Repo enthält nur diese Verfassung. Baue das Projekt nach
CLAUDE.md Abschnitt 0 auf, aber in dieser Reihenfolge:

1. CMakeLists.txt, CMakePresets.json, cmake/ProjectOptions.cmake, tests/CMakeLists.txt mit doctest
   aus third_party. Noch keine Spiellogik.
2. tests/test_row.cpp mit allen Beispielen aus den Tabellen zu R2, R3 und R6 als Tabellentest
   gegen eine Funktion slideLeft in include/twenty48/row.hpp. Nur die Deklaration, kein Code.
3. Baue mit dem dev-Preset und zeig mir, dass die Tests nicht linken oder rot sind.

Dann stopp. Implementiere slideLeft erst, wenn ich OK sage.
```

Hier anhalten: Der Tabellentest ist die Spec in Code. Das Publikum vergleicht Tabelle und Test.

```
OK. Implementiere slideLeft, zeig mir grün, dann commit nach Regel 4.
```

### Prompt B: der Rest in einem Rutsch (ca. 10 Minuten)

```
Jetzt der Rest nach CLAUDE.md Abschnitt 0: Board (R1, R2, R3, R6, R9), Rng-Interface mit StdRng
(R4.3), Game (R4, R5, R6, R7, R8, R9), main.cpp (R10), Tests je Modul mit ScriptedRng, Skripte
check.sh / check-format.sh / check-tidy.sh / check-coverage.sh und die CI-Workflow-Datei.
Jeder Test trägt seine R-Nummer im Namen. Am Ende muss scripts/check.sh fehlerfrei laufen.
Commits in sinnvollen Schritten.
```

Erwartung: etwa 10 Minuten Arbeit, zwischendurch Rückfragen zulassen. Am Ende `scripts/check.sh`
grün, `git log --oneline` zeigt Spec-nahe Commits. Sollte etwas hängen, ist das der Moment für
„genau so sieht Review aus“: lesen, korrigieren lassen, weiter.

### Danach: Feature-Flow wie im Regieplan

Issue, Prompt 1 (Spec und rote Tests), Prompt 2 (grün, Gate, PR) aus `docs/demo/prompts.md`.
Dann die Gegenprobe. Im frischen Repo gibt es die Branches `demo/*` nicht; für die Gegenprobe
die Datei `src/row.cpp` aus dem Branch `demo/vibe-bug` des fertigen Repos kopieren oder dort zeigen.

## Zeitbudget für die 60 Minuten

| Block | Minuten |
|---|---|
| Einstieg, These, Seed zeigen (CLAUDE.md, SPEC.md) | 8 |
| Prompt A: Gerüst, Tabellentest rot, slideLeft grün | 10 |
| Prompt B: Rest, check.sh grün | 12 |
| Push, CI grün, Branch Protection | 5 |
| Feature-Flow Undo: Issue, rot, grün, PR, Merge | 15 |
| Gegenprobe, Takeaways, Fragen | 10 |

Das ist eng. Wenn es knapp wird: Prompt B schon vor dem Vortrag laufen lassen, Repo in diesem
Zustand mitbringen und live nur Prompt A auf einem Wegwerf-Branch zeigen. Der Rest wie im
Regieplan.

## Vor dem Event unbedingt einmal durchspielen

```sh
cd ~/assistent/vibe-coding-event && twenty48/scripts/make-seed.sh
cp -r twenty48-seed /tmp/probe && cd /tmp/probe && git init -b main && git add -A && git commit -qm "chore: seed"
claude      # Prompt A, OK, Prompt B
scripts/check.sh
```

Was dabei schiefgeht, wandert als Regel in CLAUDE.md oder als Beispiel in SPEC.md, dann
`make-seed.sh` erneut. Genau das ist die Botschaft des Vortrags: Die Verfassung wird besser,
nicht der Prompt.
