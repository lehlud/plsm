# plsm
## Stack Machine
### Befehle
Grundsätzlich werden bei jeder Operation die verwendeten Werte vom Stapel gelöscht und durch das Ergebnis ersetzt.

Beispiel $a + b = c$:
- $a$ und $b$ werden vom Stapel entfernt
- $c$ wird auf den Stapel geschoben

### Befehlsübersicht
| ID  | Befehl   | Argument         | Wirkung                                                           |
| --- | -------- | ---------------- | ----------------------------------------------------------------- |
| 0   | NOP      | –                | Macht nichts                                                      |
| 1   | HLT      | –                | Beendet das Programm (exit-code ist der oberste Wert des Stapels) |
|     |          |                  |                                                                   |
| 20  | IADD     | –                | Addiert zwei Integer                                              |
| 50  | FADD     | –                | Addiert zwei Floats                                               |
|     |          |                  |                                                                   |
| 90  | ICASTF   | –                | Konvertiert einen Integer in einen Float                          |
| 91  | FCASTI   | –                | Konvertiert einen Float in einen Integer                          |
|     |          |                  |                                                                   |
| 120 | CMPEQ    | –                | Vergleicht zwei Werte bzgl. Gleicheit                             |
| 130 | ICMPLT   | –                | Vergleicht zwei Integer bzgl. „kleiner als“                       |
| 131 | ICMPGT   | –                | Vergleicht zwei Integer bzgl. „größer als“                        |
| 140 | FCMPLT   | –                | Vergleicht zwei Floats bzgl. „kleiner als“                        |
| 141 | FCMPGT   | –                | Vergleicht zwei Floats bzgl. „größer als“                         |
|     |          |                  |                                                                   |
| 150 | NEG      | –                | Invertiert die Bits des Wertes oben auf dem Stapel                |
| 170 | AND      | –                | Bitweises „und“ der oberen beiden Werte des Stapels               |
| 171 | OR       | –                | Bitweises „oder“ der oberen beiden Werte des Stapels              |
| 172 | XOR      | –                | Bitweises exklusives „oder“ der oberen beiden Werte des Stapels   |
|     |          |                  |                                                                   |
| 200 | LOAD     | &lt;VALUE&gt;    | Lädt einen Wert auf den Stack (push)                              |
|     |          |                  |                                                                   |
| 220 | JMP      | &lt;POSITION&gt; | Springt zu der angegebenen Position                               |
| 221 | JMPTRUE  | &lt;POSITION&gt; | siehe JMP; springt nur wenn der oberste Wert des Stapels wahr ist |
| 221 | JMPLABEL | &lt;LABEL&gt;    | Springt zum angegebenen Label                                     |

## Bytecode
### Aufbau
- Informationen über das Modul (Name, Version, plsm-Version etc.)
- Benannte Sprungpunkte (GoTo-labels)
- „Richtige“ Instruktionen

### Im Code
#### Modulinformationen
- Labels werden in einer Hash-Map gespeichert (&lt;label&gt; : &lt;jmp-index&gt;)

## CLI
### Kompilieren
```bash
plsm -c -f <SRC> -o <OUT>
```

### Ausführen
```bash
plsm -f <FILE>
```
