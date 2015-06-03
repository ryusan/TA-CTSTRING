@echo off
type nul > log.txt
type nul > logre.txt
for /L %%X in (1,1,10) do (
"CTSTRINGtestGenerator.exe" > in
call timed "CTSTRINGv2.exe < in > outy"
type in >> log.txt
type outy >> logre.txt
@echo off
timeout /t 1 /nobreak
)
::call timed ""CTSTRING(Duddin).exe" < in > outd"
::fc outy outd