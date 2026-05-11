# InfDK
Implementação de DonkeyKong em C.

Como esse projeto usa raylib, para copilar é necessário usar as flags:
gcc main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11


(Para depois)
- [ ] Distribuir o projeto em diferentes arquivos:
  - [ ] main.c  -- Para a função principal e o fluxo geral do programa
  - [ ] dk.h  -- Arquivo contendo o protótipo das funções
  - [ ] dk.c  -- Desenvolvimento das funções (source files)


* Sobre Termux:
    1. Uma vez aberto Termux:X11 (aplicativo)
    2. Para abrir o servidor: termux-x11 :0
    3. source DISPLAY:=0 (no mesmo terminal da execussão)




    
