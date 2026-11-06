# GitHub einrichten (einmalig, vor dem Event)

```sh
gh auth login                       # Browser-Login
scripts/github-setup.sh <owner>/twenty48        # oder: ... --private
```

Das Skript legt das Repo an, pusht alle Branches, setzt den Code Owner, schützt `main`
(PR-Pflicht, alle fünf CI-Checks erforderlich, kein Force-Push, lineare Historie),
aktiviert Squash-Merges und löscht gemergte Branches automatisch.

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
