## Gewünschtes Verhalten

Als Spielerin möchte ich meinen letzten Zug rückgängig machen können, weil ich mich bei
schnellen Eingaben regelmäßig vertippe und ein einziger falscher Zug ein gutes Spiel ruiniert.

## Vorschlag für die Spec

Neue Anforderung R11 „Rückgängig“:

- Nach einem wirksamen Zug kann der Zug rückgängig gemacht werden. Spielfeld und Punktestand sind
  danach exakt wie vor dem Zug. Die nach dem Zug erschienene Kachel verschwindet wieder.
- Mehrere Züge können nacheinander rückgängig gemacht werden, bis zum Spielstart.
- Gibt es nichts rückgängig zu machen, passiert nichts und die Aktion meldet das.
- Unwirksame Züge (R6) erzeugen keinen Eintrag.
- R10.1 bekommt die Taste `u`.

## Akzeptanzkriterien

- [ ] R11 steht mit Beispielen in `SPEC.md`, Änderungshistorie ergänzt
- [ ] Tests für alle vier Punkte, benannt mit R11.x
- [ ] `u` im Terminal funktioniert
- [ ] `scripts/check.sh` grün
