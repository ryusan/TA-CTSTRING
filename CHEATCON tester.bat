g++ "CHEATCONtestGenerator.cpp" -o "CHEATCONtestGenerator"
g++ "CHEATCONV2.cpp" -o "CHEATCONV2"
g++ "CHEATCON(Duddin).cpp" -o "CHEATCON(Duddin)"
"CHEATCONtestGenerator.exe" > in
"CHEATCONV2.exe" < in > outy
"CHEATCON(Duddin).exe" < in > outd
fc outy outd
pause