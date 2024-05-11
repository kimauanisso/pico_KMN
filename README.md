# Programação do microcontrolador RP2040 usando C/C++ SDK

A implementação desse metodo de programação é mais eficiente, usando ao máximo seus recursos. As seguintes instalações foram baseasdas no datasheet da Raspberry C/C++ SDK.

Existe diversos metodos para programa desta forma, linux, windows e mac, porém como a maioria usa windows, e para evitar futuros problemas, será utilizado o WSL (Windows Subsystem for Linux)

## Instalação
### Instalndo WSL

Abra o PowerShell e digite o seguinte comando:
```powershell
wsl --install
```
Após a instalação abra o WSL.

### Ambiente WSL
No ambiente WSL, como o proprio nome diz é um linux, porém não existe interface gráfica para esse "Computador", somente o terminal, então você usará comandos linux, são poucos, para isso siga os seguintes passos:

```powershell
cd
mkdir pico
cd pico

git clone https://github.com/raspberrypi/pico-sdk.git --branch master
cd pico-sdk
git submodule update --init
cd ..

git clone https://github.com/kimauanisso/pico_SDK.git

sudo apt update
sudo apt upgrade
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential libstdc++-arm-none-eabi-newlib
```

Neste momento o ambiente está pronto para ser utilizado
OBS.: Necessário instalar VSCode.

### Programando
Para programar basta fazer os seguintes comandos:

```powershell
cd pico_KMN
code .
```

No Terminal do VSCode:


```powershell
mkdir build 
cd build
export PICO_SDK_PATH=../../pico-sdk
cmake ..
make
```
