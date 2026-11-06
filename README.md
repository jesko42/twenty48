# twenty48

2048 im Terminal, geschrieben in C++17 mit CMake. Das Projekt ist die Bühne für den Vortrag
**„Professionelle KI-gestützte Softwareentwicklung“**: Spec-Driven Development, GitHub,
Tests, Versionskontrolle und Qualitätsstandards.

Die Regeln des Spiels stehen in [SPEC.md](SPEC.md). Die Regeln für die Entwicklung
(für Menschen und KI) stehen in [CLAUDE.md](CLAUDE.md).

## Bauen und spielen

```sh
cmake --preset dev
cmake --build --preset dev
ctest --preset dev
./build/dev/twenty48            # spielen: a/d/w/s + Enter, q beendet
./build/dev/twenty48 --seed 42 --moves lrud   # deterministisch, ohne Interaktion
```

Voraussetzungen: CMake ≥ 3.25, Ninja, GCC ≥ 12 oder Clang ≥ 16, clang-format und clang-tidy 18.

## Qualitätsgate

```sh
scripts/check.sh        # Format, Build (-Werror), Tests mit ASan/UBSan, clang-tidy
git config core.hooksPath .githooks   # einmalig: Format-Check vor jedem Commit
```

Dasselbe läuft in GitHub Actions (`.github/workflows/ci.yml`) für jeden Pull Request,
zusätzlich mit GCC und Clang sowie einer Abdeckungsschwelle von 90 % für die Kernbibliothek.
`main` ist geschützt: Änderungen kommen nur über Pull Requests mit grünen Checks.

## Struktur

```
SPEC.md               Verhalten (R1 ... R10, NF1 ... NF4)
CLAUDE.md             Arbeitsregeln für KI und Mensch
include/twenty48/     Kernbibliothek: row, board, rng, game
src/main.cpp          Terminal-Oberfläche
tests/                doctest-Tests, benannt nach Spec-Nummern
cmake/, CMakePresets.json
scripts/              check.sh, check-format.sh, check-tidy.sh, check-coverage.sh,
                      github-setup.sh, demo-reset.sh, make-seed.sh
docs/                 REGIEPLAN.md (Ablauf des Vortrags), FLOW.md (Aufbau aus dem leeren Repo),
                      GITHUB-SETUP.md, demo/
third_party/doctest   Test-Framework (MIT), eingebettet, keine Netzabhängigkeit beim Bauen
```
