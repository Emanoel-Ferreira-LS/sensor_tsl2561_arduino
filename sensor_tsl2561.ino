#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

/* Este driver utiliza a biblioteca de sensores unificados Adafruit (Adafruit_Sensor),
   que fornece um 'tipo' comum para os dados do sensor e algumas funções auxiliares.
   
   Para usar este driver, você também precisará baixar a biblioteca Adafruit_Sensor
   e incluí-la na sua pasta de bibliotecas.

   Você também deve atribuir um ID único a este sensor para uso com
   a API do Sensor Adafruit, de modo que você possa identificar este sensor em qualquer
   registro de dados, etc. Para atribuir um ID único, basta
   fornecer um valor apropriado no construtor abaixo (12345
   é usado por padrão neste exemplo).
   
   Conexões
   ===========
   Conecte SCL ao Clock I2C SCL
   Conecte SDA aos Dados I2C SDA
   Conecte VCC/VDD a 3,3V ou 5V (dependendo do nível lógico do sensor, verifique a folha de dados)
   Conecte GROUND ao terra comum

   Endereço I2C
   ===========
   O endereço será diferente dependendo se você deixa
   o pino ADDR flutuante (endereço 0x39) ou o liga ao terra ou VCC. 
   O endereço padrão é 0x39, o que pressupõe que o pino ADDR está flutuante
   (não conectado a nada). Se você definir o pino ADDR como alto
   ou baixo, use TSL2561_ADDR_HIGH (0x49) ou TSL2561_ADDR_LOW
   (0x29), respectivamente.
    
   Histórico
   =======
   2013/JAN/31  - Primeira versão (KTOWN)
*/
   
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

/**************************************************************************/
/*
    Exibe algumas informações básicas sobre este sensor a partir do tipo de sensor
    sensor_t da API de sensor unificado (veja Adafruit_Sensor para mais informações)
*/
/**************************************************************************/
void displaySensorDetails(void)
{
  sensor_t sensor;
  tsl.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("ID Único:     "); Serial.println(sensor.sensor_id);
  Serial.print  ("Valor Máx:    "); Serial.print(sensor.max_value); Serial.println(" lux");
  Serial.print  ("Valor Mín:    "); Serial.print(sensor.min_value); Serial.println(" lux");
  Serial.print  ("Resolução:    "); Serial.print(sensor.resolution); Serial.println(" lux");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Configura o ganho e o tempo de integração para o TSL2561
*/
/**************************************************************************/
void configureSensor(void)
{
  /* Você também pode definir manualmente o ganho ou habilitar o suporte ao ganho automático */
  // tsl.setGain(TSL2561_GAIN_1X);      /* Sem ganho ... use em luz intensa para evitar saturação do sensor */
  // tsl.setGain(TSL2561_GAIN_16X);     /* Ganho de 16x ... use em pouca luz para aumentar a sensibilidade */
  tsl.enableAutoRange(true);            /* Ganho automático ... alterna automaticamente entre 1x e 16x */
  
  /* Alterar o tempo de integração proporciona uma melhor resolução do sensor (402ms = dados de 16 bits) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* rápido, mas baixa resolução */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* resolução e velocidade médias   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* dados de 16 bits, mas conversões mais lentas */

  /* Atualize esses valores dependendo do que você configurou acima! */  
  Serial.println("------------------------------------");
  Serial.print  ("Ganho:        "); Serial.println("Auto");
  Serial.print  ("Tempo:        "); Serial.println("13 ms");
  Serial.println("------------------------------------");
}

/**************************************************************************/
/*
    Função de configuração do Arduino (chamada automaticamente na inicialização)
*/
/**************************************************************************/
void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Teste de Sensor de Luz"); Serial.println("");
  
  /* Inicializa o sensor */
  //use tsl.begin() para usar o Wire padrão, 
  //tsl.begin(&Wire2) direciona a API para usar o Wire2, etc.
  if(!tsl.begin())
  {
    /* Houve um problema ao detectar o TSL2561 ... verifique suas conexões */
    Serial.print("Opa, nenhum TSL2561 detectado ... Verifique sua conexão ou o ADDR I2C!");
    while(1);
  }
  
  /* Exibe algumas informações básicas sobre este sensor */
  displaySensorDetails();
  
  /* Configura o ganho e o tempo de integração do sensor */
  configureSensor();
  
  /* Estamos prontos para começar! */
  Serial.println("");
}

/**************************************************************************/
/*
    Função de loop do Arduino, chamada uma vez que a 'setup' está completa
    (seu próprio código deve ser colocado aqui)
*/
/**************************************************************************/
void loop(void) 
{  
  /* Obter um novo evento do sensor */ 
  sensors_event_t event;
  tsl.getEvent(&event);
 
  /* Exibir os resultados (a luz é medida em lux) */
  if (event.light)
  {
    Serial.print(event.light); Serial.println(" lux");
  }
  else
  {
    /* Se event.light = 0 lux, o sensor está provavelmente saturado
       e nenhum dado confiável pôde ser gerado! */
    Serial.println("Sobrecarga do sensor");
  }
  delay(250);
}
