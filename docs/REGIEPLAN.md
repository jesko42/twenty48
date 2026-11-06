# Regieplan: „Professionelle KI-gestützte Softwareentwicklung“ (60 Minuten)

Roter Faden: **Die KI ist der schnellste Junior, den ihr je hattet. Die Frage ist nicht, ob sie
Code schreibt, sondern ob euer Prozess sie hält.** Alles, was wir zeigen, ist Prozess:
Spec → Tests → Code → Gate → PR → Merge. Das Spiel ist nur die Bühne.

Prinzip für geringes Risiko: **Nichts wird live zum ersten Mal gemacht.** Jeder Live-Schritt hat
einen vorbereiteten Branch als Fallback. Der Build ist warm. Die Prompts stehen in
`docs/demo/prompts.md`.

## Ablauf

| Zeit | Block | Was passiert | Fallback |
|---|---|---|---|
| 00:00 | Einstieg (5 min) | Die These. 30 Sekunden spielen: `./build/dev/twenty48`. „Das hat die KI gebaut. Interessant ist nicht das Ergebnis, sondern wie.“ | – |
| 00:05 | Leitplanken (10 min) | Tour durchs Repo: `SPEC.md` (Beispiele! R3), `CLAUDE.md` (Regeln für die KI), `tests/test_row.cpp` (Tests zitieren die Spec), `CMakePresets.json`, `.github/workflows/ci.yml`, Branch Protection auf GitHub zeigen. Dann `scripts/check.sh` laufen lassen: grün in unter 30 Sekunden. | – |
| 00:15 | Wunsch (5 min) | Publikum fragen: „Was fehlt dem Spiel?“ Auf **Undo** steuern (siehe Tabelle unten). Issue auf GitHub anlegen, Text aus `docs/demo/issue-undo.md`. | Issue schon vorher angelegt lassen |
| 00:20 | Live mit Claude Code (15 min) | Prompt 1: Spec ergänzen, Tests schreiben, **rot zeigen**, stoppen. Kurz innehalten: „Das ist der Moment, der Vibe Coding von Engineering trennt.“ Prompt 2: implementieren, grün, `check.sh`, Commits, Push, PR. | `git checkout demo/feature-undo`, `git log --oneline -4` zeigt die drei Schritte; PR daraus öffnen |
| 00:35 | CI und Merge (7 min) | PR auf GitHub: fünf Checks laufen (gcc, clang, format, tidy, coverage). Checkliste in der PR-Vorlage durchgehen. Merge. | Screenshots in `docs/demo/screenshots/` (vor dem Event anfertigen) |
| 00:42 | Gegenprobe (8 min) | PR aus `demo/vibe-bug` öffnen: „Refactoring, sieht harmlos aus, 8 Zeilen weniger.“ CI wird rot. Den fehlgeschlagenen Test zeigen: er nennt R3.2 und das Beispiel `2 2 4 0 → 4 4 0 0` (die KI hat `8 0 0 0` daraus gemacht). „Der Test hat die Spec verteidigt, nicht der Mensch.“ Dann `scripts/check.sh` lokal zeigen: hätte man vor dem Push gesehen. | lokal: `git checkout demo/vibe-bug && ctest --preset dev` |
| 00:50 | Takeaways und Fragen (10 min) | Fünf Sätze, siehe unten. | – |

## Fünf Takeaways

1. Die Spec ist das Produkt. Code ist die Übersetzung, und die kann die KI.
2. Tests zuerst und rot sehen. Ein Test, der nie rot war, beweist nichts.
3. Das Gate ist unverhandelbar: Warnungen als Fehler, Format, Tidy, Sanitizer, Abdeckung. Lokal und in CI dasselbe Skript.
4. `main` ist heilig. Alles geht durch einen PR, auch das, was die KI schreibt. Vor allem das.
5. Die Regeln für die KI stehen im Repo (`CLAUDE.md`), nicht im Kopf. Dann gelten sie für jeden Agenten und jeden Kollegen.

## Wahrscheinliche Publikumswünsche

| Wunsch | Aufwand live | Empfehlung |
|---|---|---|
| Undo | 10 min | **Ziel.** Vorbereitet in `demo/feature-undo`. |
| Spielfeldgröße per `--size` | 5 min | Zweite Wahl. Kern kann es schon (R1.2), fehlt nur in `main.cpp` und R10. |
| Züge zählen | 5 min | Einfach, aber unspektakulär. |
| Ziel 4096 statt 2048 | 3 min | Zu klein, als Zusatz nach Undo denkbar. |
| Highscore speichern | 20 min | Zu groß, Datei-I/O. Als Issue anlegen: „nächster Sprint“. Guter Moment für Backlog-Disziplin. |
| Farben im Terminal | 10 min | Nicht testbar im Kern. Gute Diskussion: UI vs. Logik (NF1). Ablehnen oder als Issue parken. |
| KI-Gegner / Autoplay | 30+ min | Ablehnen, Issue anlegen. |

Steuersatz, falls niemand Undo sagt: „Wer hat gerade einen Zug bereut? Genau. Undo.“

## Checkliste am Vortag

- [ ] GitHub-Repo angelegt (`docs/GITHUB-SETUP.md`), CI einmal grün auf `main`
- [ ] Branch Protection aktiv, alle fünf Checks als „required“ sichtbar
- [ ] Branches `demo/feature-undo` und `demo/vibe-bug` gepusht
- [ ] Screenshots von grüner und roter CI in `docs/demo/screenshots/`
- [ ] Claude Code eingeloggt, einmal den kompletten Prompt-Ablauf geprobt und gestoppt
- [ ] `gh auth status` grün, `gh issue create` einmal getestet
- [ ] Terminal: Schrift ≥ 18 pt, helles Theme, Fenster auf Beamer-Auflösung
- [ ] Browser: GitHub-Repo, Actions-Tab und PR-Liste als Tabs offen, eingeloggt

## Checkliste 30 Minuten vorher

- [ ] `scripts/demo-reset.sh` (auf `main`, Build warm, Tests grün)
- [ ] WLAN testen: `gh api user` und `claude --version`
- [ ] Hotspot am Handy als Reserve aktiv
- [ ] Issue-Text und Prompts in einem Editor-Tab geöffnet

## Was kann schiefgehen

| Risiko | Vorsorge |
|---|---|
| Kein Netz | Fallback-Branches lokal, Screenshots der CI, lokal `check.sh` statt Actions |
| Claude Code macht etwas Unerwartetes | Prompt 1 endet bewusst bei den roten Tests. Dort stoppen, bewerten, notfalls auf `demo/feature-undo` wechseln. „Genau deshalb reviewen wir.“ |
| CI dauert zu lang | Jobs laufen parallel, etwa 2 bis 3 Minuten. Die Zeit mit der PR-Checkliste füllen. Alternativ PR aus `demo/feature-undo` früh öffnen, dann ist CI beim Zeigen schon fertig. |
| Publikum will ein großes Feature | Tabelle oben: als Issue anlegen, „Backlog ist auch Prozess“. |
| Terminal-Tippfehler | Alle Befehle stehen in `docs/demo/prompts.md`, Copy and Paste ist erlaubt. |
