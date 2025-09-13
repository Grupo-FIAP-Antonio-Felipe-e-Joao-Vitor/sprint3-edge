# Placar Eletrônico com ESP32 e FIWARE descomplicado

## Autores: 
- Antônio Jacinto de Andrade Neto (RM: 561777)
- Felipe Bicaletto (RM: 563524)
- João Vitor dos Santos Pereira (RM: 551695)
- Thayná Pereira Simões (RM: 566456) 
---

## Descrição do projeto
Este projeto implementa um placar eletrônico inteligente utilizando o ESP32, com interação via 4 botões físicos para adicionar ou remover pontos dos times A e B. O placar é exibido em um display LCD I2C (16x2) e os dados também são enviados em tempo real para um broker MQTT, permitindo integração com sistemas externos e aplicações IoT.

O sistema garante conectividade via Wi-Fi, reconexão automática ao broker e atualização imediata sempre que o placar é alterado.

## Requisitos Mínimos de Hardware para operação no Ubuntu Server LTS
- Núcleos de Processamento - 1vCPU
- Memória RAM - 1GB
- Armazenamento Secundário Mínimo - 20GB HD e/ou SSD

## Instruções de Instalação e Uso

### Preparação do Ambiente
1. Clone o projeto em uma máquina virtual com as especificações mínimas:
```sh
git clone https://github.com/Grupo-FIAP-Antonio-Felipe-e-Joao-Vitor/sprint3-edge.git
cd sprint3-edge
```

2. Atualize os pacotes do sistema:
```sh
sudo apt update
```

3. Instale o Docker e o Docker Compose:
```sh
sudo apt install docker.io
sudo apt install docker-compose
```

4. Suba os containers necessários para o FIWARE Descomplicado:
```sh
sudo docker-compose up -d
```

### Configuração no Postman

1. Baixe a collection do Postman disponível no repositório do GitHub.

2. No IoT Agent, siga os passos:
- Execute o Health Check.
- Provisione o Service Group.
- Provisione o Smart Device.

### Arquitetura do ESP32
![Arquitetura do ESP32](ArquiteturaESP32.png)

### Configuração do ESP32

1. Faça o upload do código para o ESP32.

2. No código, configure:
- O SSID e a senha da rede Wi-Fi.
- O IP da máquina virtual (VM) onde o FIWARE está rodando.

### Acessando os Resultados
No Postman, após a configuração e execução:
- Result of Score A → Retorna a pontuação do Time A.
- Result of Score B → Retorna a pontuação do Time B.
- Result of Score A and B → Retorna a pontuação de ambos os times.