@echo off
for /L %%x in (0,1,9) do (
CTSTRINGtestGenerator.exe %%x > in2.%%x
)
::@echo off
::timeout /t 1 /nobreak
::)
::call timed ""CTSTRING(Duddin).exe" < in > outd"
::fc outy outd