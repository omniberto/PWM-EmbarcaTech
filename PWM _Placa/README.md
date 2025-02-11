# PWM
1. Nesta pasta se encontra a simulação do Wokwi para VsCode e o código utilizado no funcionamento do LED.
2. O código pode ser enviado para a placa BitDogLab para testes.
3. Execução:
    1. Compile o código.
    2. Na simulação, basta apenas executar o diagrama com o código pré-determinado.
    3. Na placa BitDigLab, envie o programa através do VSCode, e teste com o usb conectado ou utilizando a bateria da placa.
4. O LED ligará nas intensidades pré determinadas com Duty Cycles de 0,12%, 0,075% e 0,025% respectivamente.
5. Após isso, ele executrá uma rotina de variação de intensidade periódica, variando entre 0,12% e 0,025% de Duty Cycles.
6. A variação continuará até o fim da execução ou desligamento da placa.

P.S. Nenhuma ação externa (além de iniciar a simulação, ou ligar a placa) é necessária para execução das rotinas.