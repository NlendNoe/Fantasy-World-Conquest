g++ -I"C:\SFML\include" -L"C:\SFML\lib" main.cpp -o game -lsfml-graphics -lsfml-window -lsfml-system
if %errorlevel% equ 0 (
    .\game.exe
)