# Spezifikation: twenty48

| | |
|---|---|
| Status | gültig |
| Version | 1.0 |
| Sprache | Spec und Doku auf Deutsch, Code, Tests und Commits auf Englisch |

Diese Datei ist die **einzige Quelle der Wahrheit** für das Verhalten des Spiels.
Jede Anforderung hat eine Nummer (R1, R2, ...). Tests referenzieren diese Nummern.
Keine Funktionalität ohne Anforderung, keine Anforderung ohne Test.

## Begriffe

- **Spielfeld**: quadratisches Raster aus Zellen.
- **Kachel**: eine Zelle mit einer Zahl (2, 4, 8, ...). Eine Zelle ohne Kachel ist **leer** (Wert 0).
- **Zug**: Schieben aller Kacheln in eine der vier Richtungen (links, rechts, oben, unten).
- **Linie**: eine Zeile oder Spalte, betrachtet in Zugrichtung. Index 0 ist die Wand, zu der geschoben wird.
- **Wirksamer Zug**: ein Zug, der mindestens eine Zelle verändert (siehe R6).

## Funktionale Anforderungen

### R1 Spielfeld

- R1.1 Das Spielfeld ist quadratisch mit N × N Zellen. Standard ist N = 4.
- R1.2 N ist konfigurierbar, mindestens 2. Kleinere Werte werden mit einem Fehler abgelehnt.
- R1.3 Zugriffe auf Zellen außerhalb des Spielfelds werden mit einem Fehler abgelehnt.
- R1.4 Ein neues Spielfeld ist vollständig leer.

### R2 Schieben

Beim Zug rutschen alle Kacheln einer Linie bis zur Wand. Die Reihenfolge der Kacheln bleibt erhalten, Lücken werden geschlossen.

| Linie vorher | Linie nachher |
|---|---|
| `0 0 0 2` | `2 0 0 0` |
| `0 2 0 4` | `2 4 0 0` |

### R3 Verschmelzen

- R3.1 Zwei gleiche Kacheln, die nach dem Schieben direkt nebeneinander liegen, verschmelzen zu einer Kachel mit der Summe.
- R3.2 Jede Kachel nimmt pro Zug an **höchstens einer** Verschmelzung teil. Eine durch Verschmelzen entstandene Kachel verschmilzt im selben Zug nicht erneut.
- R3.3 Bei drei gleichen Kacheln verschmilzt das Paar, das der Wand näher ist.
- R3.4 Lücken zwischen gleichen Kacheln verhindern das Verschmelzen nicht.

| Linie vorher | Linie nachher | Punkte | Regel |
|---|---|---|---|
| `2 2 0 0` | `4 0 0 0` | 4 | R3.1 |
| `2 0 2 4` | `4 4 0 0` | 4 | R3.4 |
| `2 2 2 2` | `4 4 0 0` | 8 | R3.2 |
| `2 2 4 0` | `4 4 0 0` | 4 | R3.2 (kein Kettenverschmelzen) |
| `2 2 2 0` | `4 2 0 0` | 4 | R3.3 |
| `2 2 4 4` | `4 8 0 0` | 12 | R3.1 |
| `4 2 2 0` | `4 4 0 0` | 4 | R3.1 |

### R4 Neue Kachel

- R4.1 Nach jedem wirksamen Zug erscheint genau **eine** neue Kachel auf einer zufälligen leeren Zelle.
- R4.2 Die neue Kachel hat den Wert 2 mit Wahrscheinlichkeit 9/10 und den Wert 4 mit Wahrscheinlichkeit 1/10.
- R4.3 Der Zufall wird von außen bereitgestellt (injiziert), damit Tests und Replays deterministisch sind. Reihenfolge der Abfragen: zuerst die Zelle (Index in die Liste der leeren Zellen, zeilenweise von oben links), dann der Wert (Wurf aus 10, Ergebnis 0 bedeutet 4).
- R4.4 Ist keine Zelle leer, erscheint keine Kachel und der Zufall wird nicht befragt.

### R5 Spielstart

Ein neues Spiel beginnt mit einem leeren Spielfeld, auf dem zwei Kacheln nach R4 platziert werden. Der Punktestand ist 0.

### R6 Wirksamer Zug

- R6.1 Ein Zug ist wirksam, wenn er mindestens eine Zelle verändert.
- R6.2 Ein unwirksamer Zug verändert nichts: keine neue Kachel, keine Punkte, der Zufall wird nicht befragt.

| Linie vorher | Zug nach links | wirksam |
|---|---|---|
| `2 4 2 4` | `2 4 2 4` | nein |
| `2 4 0 0` | `2 4 0 0` | nein |
| `0 0 0 2` | `2 0 0 0` | ja |

### R7 Punkte

Für jede Verschmelzung wird der Wert der entstandenen Kachel zum Punktestand addiert. Der Punktestand ist kumulativ über das ganze Spiel.

### R8 Gewinn

Das Spiel ist gewonnen, sobald eine Kachel mit dem Wert 2048 (oder höher) existiert. Das Spiel darf danach weitergespielt werden.

### R9 Spielende

Das Spiel ist beendet, wenn keine Zelle leer ist und kein Paar gleicher Kacheln waagerecht oder senkrecht nebeneinander liegt. Dann ist kein wirksamer Zug mehr möglich.

### R10 Bedienung im Terminal

- R10.1 Tasten: `a` links, `d` rechts, `w` oben, `s` unten, `q` beenden. Jede Eingabe wird mit Enter bestätigt.
- R10.2 `--seed N` macht die Kachelplatzierung reproduzierbar.
- R10.3 `--moves LRUD...` spielt die angegebenen Züge ohne Interaktion und gibt das Endfeld aus (für Tests und Demos).
- R10.4 Die Ausgabe zeigt immer den aktuellen Punktestand und das Spielfeld; bei Gewinn und Spielende erscheint ein Hinweis.
- R10.5 Exit-Code 0 bei normalem Ende, 2 bei ungültigen Argumenten.

## Nicht-funktionale Anforderungen

- NF1 Die Spiellogik (`twenty48::core`) enthält keine Ein-/Ausgabe und keinen eigenen Zufall.
- NF2 Mit gleichem Seed und gleichen Zügen ist der Spielverlauf identisch.
- NF3 Qualitätsgates vor jedem Merge: Build ohne Warnungen (`-Werror`), alle Tests grün, clang-format, clang-tidy, AddressSanitizer und UndefinedBehaviorSanitizer, Zeilenabdeckung der Kernbibliothek mindestens 90 %.
- NF4 C++17, CMake ≥ 3.25, Linux und macOS. Keine externen Abhängigkeiten außer dem mitgelieferten Test-Framework.

## Änderungshistorie

| Datum | Änderung | Anlass |
|---|---|---|
| 2026-11-06 | Version 1.0 | Erstfassung |
