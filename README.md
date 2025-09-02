Projeto-Bateria
=================

# Uso

## Compilação

Para usar este repositório é necessário instalar o repositório : 

```
https://github.com/RemeLards/Embarcados_II
```

Ou altere o diretório do Gecko_SDK (e talvez outros) no Makefile manualmente.

Para instalar este repositório basta (caso o git clone tenha sido no diretório ~):

```
cd ~/Embarcados_II/efm32gg-stk3700-gcc-cmsis/
git clone https://github.com/RemeLards/projeto-bateria.git
```

## Flash

```
cd ~/Embarcados_II/efm32gg-stk3700-gcc-cmsis/projeto-bateria/
make flash
```

## Novos Sons

Caso queira colocar outros sons, basta : 

```
cd ~/Embarcados_II/efm32gg-stk3700-gcc-cmsis/projeto-bateria/sound/
```

Altere o Makefile para incluir o '.wav' desejado, e use o programa Python 'canal_par.py' para quantizar o áudio para 7 bits. Cheque se o tamanho do novo vetor no '#define' está correto ( ((tamanho_vetor_original+1)/2) - 1 ), e altere o tipo do vetor para 'const uint8_t', 'uint8_t' para o tamanho de cada número ser de 8 bits e 'const' para o vetor ser salvo apenas na memória Flash (que possuí 1Mb).


# Bugs

## Flash
Quando se usa 'make flash', as vezes o microcontrolador se desconecta do JLink, e reinicia. Suponho que a porta que o PWM está configurado seja importante/usada no boot, e por estar circulando corrente (por conta de estar conectado no amplificador) nos meus testes esse problema acontecia de maneira frequente e parecia resolver caso eu deixasse o circuito do auto-falante aberto.

## Slider
Bugou uma vez e o LCD ficou com o fenômeno de 'flickering' e o PWM todo travado (perceptível pelo som), não consegui replicar, mas é um glitch possível de ocorrer.