@echo off
for /L %%Y in (0,1,9)do (
type nul > out2.%%Y
for /L %%X in (1,1,50) do (
@echo off
call timed "CTSTRINGv2.exe < in2.%%Y >> out" >> out2.%%Y
)
)
::@echo off
::timeout /t 1 /nobreak
::)
::call timed ""CTSTRING(Duddin).exe" < in > outd"
::fc outy outd