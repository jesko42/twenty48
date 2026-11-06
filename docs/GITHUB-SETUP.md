# GitHub einrichten (einmalig, vor dem Event)

```sh
gh auth login                       # Browser-Login
scripts/github-setup.sh <owner>/twenty48        # oder: ... --private
```

Das Skript legt das Repo an (oder verwendet es, wenn es schon existiert), pusht alle Branches,
setzt den Code Owner, schützt `main` (PR-Pflicht, alle fünf CI-Checks erforderlich, kein
Force-Push, lineare Historie), erlaubt nur Rebase-Merges (die Commits `docs(spec)` → `test` → `feat` bleiben auf `main` sichtbar, Squash würde sie zu einem Commit zusammenfassen) und löscht gemergte Branches automatisch.
Es ist beliebig oft wiederholbar.

Remotes: `origin` ist GitHub (SSH), `nas` ist der Spiegel auf dem eigenen Server. Für den Spiegel
nach dem Vortrag: `git push nas --all`.

Stolpersteine, die schon passiert sind:

- `gh` ohne `workflow`-Scope darf keine Workflow-Dateien über HTTPS pushen. Das Skript nimmt
  deshalb SSH, wenn ein Schlüssel bei GitHub hinterlegt ist. Sonst: `gh auth refresh -s workflow`.
- Direkt auf `main` pushen scheitert nach dem Setup an der Branch Protection. Das ist gewollt,
  auch für Doku-Änderungen: Branch, PR, grüne Checks, Merge.
- Nach dem ersten Push erscheinen Actions-Läufe mit Verzögerung. `gh pr checks <nr>` zeigt sie
  zuerst. `gh workflow run ci.yml` startet CI bei Bedarf von Hand.

Danach:

1. Auf GitHub unter Actions prüfen, dass der erste Lauf grün ist.
2. Issue für die Demo anlegen (Text in `docs/demo/issue-undo.md`), oder live im Vortrag:
   ```sh
   gh issue create --title "[R11] Letzten Zug rückgängig machen" --body-file docs/demo/issue-undo.md
   ```
3. Fallback-PR aus `demo/vibe-bug` schon einmal öffnen und wieder schließen, damit die rote CI
   als Screenshot existiert:
   ```sh
   gh pr create --head demo/vibe-bug --title "refactor(row): simplify merge loop" --body "Kleines Refactoring, keine Verhaltensänderung."
   ```
4. Screenshots von grüner und roter CI unter `docs/demo/screenshots/` ablegen.

Die Namen der erforderlichen Checks müssen den Job-Namen in `.github/workflows/ci.yml`
entsprechen: `build-test (gcc)`, `build-test (clang)`, `format`, `tidy`, `coverage`.
