g++ "CTSTRINGtestGenerator.cpp" -o "CTSTRINGtestGenerator"
g++ "CTSTRINGv2.cpp" -o "CTSTRINGv2"
g++ "CTSTRING(Duddin).cpp" -o "CTSTRING(Duddin)"
"CTSTRINGtestGenerator.exe" > in
"CTSTRINGv2.exe" < in > outy
"CTSTRING(Duddin).exe" < in > outd
fc outy outd
pause